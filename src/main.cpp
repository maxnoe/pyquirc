#include <quirc.h>
#include <iostream>
#include <fstream>
#include <quirc++/qr.h>

using qr::QR;
using qr::decode;


int main() {
    std::ifstream file("./resources/helloworld.dat", std::ios::binary | std::ios::in);
    if (!file) {
        std::cerr << "Could not open file\n";
        return 1;
    }
    const int width = 290;
    const int height = 290;
    uint8_t image[height * width];
    file.read(reinterpret_cast<char*>(image), width * height);

    auto results = decode(image, width, height);
    std::cout << "Found " << results.size() << " qr code(s) in image" << "\n";
    for (const auto& result: results) {
        std::cout << "==== Payload ====\n";
        for (auto byte: result.payload) {
            std::cout << static_cast<char>(byte);
        }
        std::cout << '\n';
    }

}
