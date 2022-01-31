#include "quirc++/qr.h"
#include "quirc++/data.h"

namespace qr {

std::vector<Data> decode(const uint8_t* image, int width, int height) {
    QR qr;
    qr.fill_image(image, width, height);

    auto num_codes = qr.count();
    std::vector<Data> parsed_codes;
    parsed_codes.reserve(num_codes);
    for (int i=0; i < num_codes; i++) {
        quirc_code code = qr.extract(i);
        parsed_codes.push_back(qr.decode(code));
    }

    return parsed_codes;
}

}
