#ifndef QUIRCPP_QR
#define QUIRCPP_QR

#include <cstdint>
#include <new>
#include <stdexcept>
#include <utility>
#include <vector>
#include <memory>

#include "quirc++/data.h"


namespace qr {

#include <quirc.h>

struct QuircDeleter {
    void operator()(quirc* q) {
        quirc_destroy(q);
    }
};
using quirc_ptr = std::unique_ptr<quirc, QuircDeleter>;

class Decoder {
    public:
        Decoder();
        Decoder(const uint8_t* image, size_t width, size_t height);

        void fill_image(const uint8_t* image, size_t width, size_t height);
        size_t count();
        quirc_code extract(size_t index);
        Data decode(const quirc_code& code);
        Data decode_index(size_t index);

        //  explicitly deleted since we are wrapping
        //  C code that allocates
        Decoder& operator=(const Decoder&) = delete;
        Decoder(const Decoder&) = delete;

        // Could be implemented but not for now
        Decoder& operator=(Decoder&&) = delete;
        Decoder(Decoder&& other) : ptr(std::exchange(other.ptr, nullptr)) {}

    private:
        quirc_ptr ptr;

        void resize(size_t width, size_t height);
        uint8_t* begin();
        void end();
};

std::vector<Data> decode(const uint8_t* image, size_t width, size_t height);

}
#endif /* ifndef QUIRCPP_QR */
