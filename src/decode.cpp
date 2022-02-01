#include "quirc++/decoder.h"
#include "quirc++/data.h"
#include <iostream>

namespace qr {

std::vector<Data> decode(const uint8_t* image, size_t width, size_t height) {
    Decoder decoder;
    decoder.fill_image(image, width, height);

    auto num_codes = static_cast<size_t>(decoder.count());
    std::vector<Data> parsed_codes;
    parsed_codes.reserve(num_codes);

    for (size_t i=0; i < num_codes; i++) {
        try {
            parsed_codes.push_back(decoder.decode_index(i));
        } catch(std::runtime_error&) {
            continue;
        }
    }

    return parsed_codes;
}

}
