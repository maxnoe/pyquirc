#include "quirc++/qr.h"

namespace qr {

QR::QR() {
    ptr = quirc_new();
    if (!ptr) {
        throw std::bad_alloc();
    }
}

QR::~QR() {
    if (ptr) {
        quirc_destroy(ptr);
    }
}

void QR::fill_image(const uint8_t* buffer, size_t width, size_t height) {
    resize(width, height);
    uint8_t* qr_buffer = begin();

    for (size_t row = 0; row < height; row ++) {
        for (size_t col=0; col < width; col++) {
            qr_buffer[row * width + col] = buffer[row * width + col];
        }
    }
    end();
}


void QR::resize(size_t width, size_t height) {
    auto ret = quirc_resize(ptr, width, height);
    if (ret < 0) {
        throw std::bad_alloc();
    }
}

int QR::count() {
    return quirc_count(ptr);
}

quirc_code QR::extract(int index) {
    quirc_code code;
    quirc_extract(ptr, index, &code);
    return code;
}

Data QR::decode(const quirc_code& code) {
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

uint8_t* QR::begin() {
    return quirc_begin(ptr, &width, &height);
}

void QR::end() {
    quirc_end(ptr);
}

}
