#include "quirc++/decoder.h"
#include <sstream>

namespace qr {

Decoder::Decoder() {
    ptr = quirc_new();
    if (!ptr) {
        throw std::bad_alloc();
    }
}

Decoder::~Decoder() {
    if (ptr) {
        quirc_destroy(ptr);
    }
}

Decoder::Decoder(const uint8_t* buffer, size_t width, size_t height) : Decoder() {
    fill_image(buffer, width, height);
}

void Decoder::fill_image(const uint8_t* buffer, size_t width, size_t height) {
    resize(width, height);
    uint8_t* qr_buffer = begin();

    for (size_t row = 0; row < height; row ++) {
        for (size_t col=0; col < width; col++) {
            qr_buffer[row * width + col] = buffer[row * width + col];
        }
    }
    end();
}


void Decoder::resize(size_t width, size_t height) {
    auto ret = quirc_resize(ptr, static_cast<int>(width), static_cast<int>(height));
    if (ret < 0) {
        throw std::bad_alloc();
    }
}

size_t Decoder::count() {
    return static_cast<size_t>(quirc_count(ptr));
}

quirc_code Decoder::extract(size_t index) {
    if (index >= count()) {
        std::stringstream msg;
        msg << "Index " << index << " is out of bounds, found " << count() << " codes";
        throw std::out_of_range(msg.str());
    }
    quirc_code code;
    quirc_extract(ptr, static_cast<int>(index), &code);
    return code;
}


Data Decoder::decode_index(size_t index) {
    quirc_code code = extract(index);
    return decode(code);
}


Data Decoder::decode(const quirc_code& code) {
    quirc_data data;
    auto err = quirc_decode(&code, &data);

    // support flipped images
    if (err == QUIRC_ERROR_DATA_ECC) {
        quirc_code flipped = code;
        quirc_flip(&flipped);
        err = quirc_decode(&flipped, &data);
    }

    if (err) {
        throw std::runtime_error(quirc_strerror(err));
    }

    return Data(data);
}

uint8_t* Decoder::begin() {
    int int_width;
    int int_height;
    uint8_t* buffer = quirc_begin(ptr, &int_width, &int_height);
    width = static_cast<size_t>(int_width);
    height = static_cast<size_t>(int_height);
    return buffer;
}

void Decoder::end() {
    quirc_end(ptr);
}

}
