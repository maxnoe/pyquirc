#ifndef QUIRCPP_QR
#define QUIRCPP_QR

#include <quirc.h>
#include <cstdint>
#include <new>
#include <stdexcept>
#include <vector>



namespace qr {

class QR {
    public:
        QR();

        void fill_image(const uint8_t* image, int width, int height);
        int count();
        quirc_code extract(int index);
        quirc_data decode(const quirc_code& code);

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

        void resize(int width, int height);
        uint8_t* begin();
        void end();
};

std::vector<quirc_data> decode(const uint8_t* image, int width, int height);

}
#endif /* ifndef QUIRCPP_QR */
