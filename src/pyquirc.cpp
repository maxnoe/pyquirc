#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "quirc++/qr.h"

namespace py = pybind11;

std::vector<quirc_data> decode(const py::array_t<uint8_t>& buffer) {
    py::buffer_info info = buffer.request();
    if (info.ndim != 2) {
        throw std::invalid_argument("Must be a 2d buffer");
    }
    int height = info.shape[0];
    int width = info.shape[1];
    return qr::decode(static_cast<uint8_t*>(info.ptr), width, height);
}

py::object payload(const quirc_data& self) {
    return py::bytes(std::string(self.payload, self.payload + self.payload_len));
}


PYBIND11_MODULE(quirc, m) {
    py::class_<quirc_data>(m, "data")
        .def_property_readonly("payload", &payload)
        ;
    m.doc() = "Python bindings for quirc";
    m.def("decode", &decode, "Decode qr codes in image");
}
