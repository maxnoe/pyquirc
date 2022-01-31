#include <string>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "quirc++/qr.h"
#include "quirc++/data.h"

namespace py = pybind11;

using qr::Data;
using qr::ECI;
using qr::ECCLevel;
using qr::DataType;


std::vector<Data> decode(const py::array_t<uint8_t>& buffer) {
    py::buffer_info info = buffer.request();
    if (info.ndim != 2) {
        throw std::invalid_argument("Must be a 2d buffer");
    }
    size_t height = info.shape[0];
    size_t width = info.shape[1];
    return qr::decode(static_cast<uint8_t*>(info.ptr), width, height);
}

py::object Data_payload(const Data& self) {
    return py::bytes(std::string(self.payload.begin(), self.payload.end()));
}


PYBIND11_MODULE(quirc, m) {
    m.doc() = "Python bindings for quirc";

    py::enum_<DataType>(m, "DataType")
        .value("NUMERIC", DataType::NUMERIC)
        .value("ALPHA", DataType::ALPHA)
        .value("BYTE", DataType::BYTE)
        .value("KANJI", DataType::KANJI)
        ;

    py::enum_<ECCLevel>(m, "ECCLevel")
        .value("M", ECCLevel::M)
        .value("L", ECCLevel::L)
        .value("H", ECCLevel::H)
        .value("Q", ECCLevel::Q)
        ;

    py::enum_<ECI>(m, "ECI")
        .value("UNDEFINED", ECI::UNDEFINED)
        .value("ISO_8859_1", ECI::ISO_8859_1)
        .value("IBM437", ECI::IBM437)
        .value("ISO_8859_2", ECI::ISO_8859_2)
        .value("ISO_8859_3", ECI::ISO_8859_3)
        .value("ISO_8859_4", ECI::ISO_8859_4)
        .value("ISO_8859_5", ECI::ISO_8859_5)
        .value("ISO_8859_6", ECI::ISO_8859_6)
        .value("ISO_8859_7", ECI::ISO_8859_7)
        .value("ISO_8859_8", ECI::ISO_8859_8)
        .value("ISO_8859_9", ECI::ISO_8859_9)
        .value("WINDOWS_874", ECI::WINDOWS_874)
        .value("ISO_8859_13", ECI::ISO_8859_13)
        .value("ISO_8859_15", ECI::ISO_8859_15)
        .value("SHIFT_JIS", ECI::SHIFT_JIS)
        .value("UTF_8", ECI::UTF_8)
        ;

    py::class_<Data>(m, "Data")
        .def_readonly("version", &Data::version)
        .def_readonly("ecc_level", &Data::ecc_level)
        .def_readonly("mask", &Data::mask)
        .def_readonly("data_type", &Data::data_type)
        .def_readonly("eci", &Data::data_type)
        .def_property_readonly("payload", &Data_payload)
        ;

    m.def("decode", &decode, "Decode qr codes in image", py::arg("img"));
}
