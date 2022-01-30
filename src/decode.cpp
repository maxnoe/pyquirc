#include "quirc++/qr.h"

namespace qr {

std::vector<quirc_data> decode(const uint8_t* image, int width, int height) {
    QR qr;
    qr.fill_image(image, width, height);

    auto num_codes = qr.count();
    std::vector<quirc_data> parsed_codes;
    parsed_codes.reserve(num_codes);
    for (int i=0; i < num_codes; i++) {
        quirc_code code = qr.extract(i);
        quirc_data data = qr.decode(code);
        parsed_codes.push_back(data);
    }

    return parsed_codes;
}

}
