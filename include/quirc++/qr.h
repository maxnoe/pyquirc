#ifndef QUIRCPP_QR
#define QUIRCPP_QR

#include <cstdint>
#include <new>
#include <stdexcept>
#include <vector>

#include "quirc++/data.h"



namespace qr {

#include <quirc.h>

class QR {
    public:
        QR();

        void fill_image(const uint8_t* image, size_t width, size_t height);
        int count();
        quirc_code extract(int index);
        Data decode(const quirc_code& code);

        //  explicitly deleted since we are wrapping
        //  C code that allocates
        QR& operator=(const QR&) = delete;
        QR(const QR&) = delete;

        // Could be implemented but not for now
        QR& operator=(QR&&) = delete;
        QR(QR&&) = delete;

        ~QR();

    private:
        quirc* ptr = nullptr;
        int width = 0;
        int height = 0;

        void resize(size_t width, size_t height);
        uint8_t* begin();
        void end();
};

std::vector<Data> decode(const uint8_t* image, size_t width, size_t height);

}
#endif /* ifndef QUIRCPP_QR */
