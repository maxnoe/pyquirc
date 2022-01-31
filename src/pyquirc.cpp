#include <string>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "quirc++/qr.h"

namespace py = pybind11;

enum class DataType {
    NUMERIC = QUIRC_DATA_TYPE_NUMERIC,
    ALPHA = QUIRC_DATA_TYPE_ALPHA,
    BYTE = QUIRC_DATA_TYPE_BYTE,
    KANJI = QUIRC_DATA_TYPE_KANJI,
};


enum class ECCLevel {
    M = QUIRC_ECC_LEVEL_M,
    L = QUIRC_ECC_LEVEL_L,
    H = QUIRC_ECC_LEVEL_H,
    Q = QUIRC_ECC_LEVEL_Q,
};


enum class ECI {
    UNDEFINED = 0,
    ISO_8859_1 = QUIRC_ECI_ISO_8859_1,
    IBM437 = QUIRC_ECI_IBM437,
    ISO_8859_2 = QUIRC_ECI_ISO_8859_2,
    ISO_8859_3 = QUIRC_ECI_ISO_8859_3,
    ISO_8859_4 = QUIRC_ECI_ISO_8859_4,
    ISO_8859_5 = QUIRC_ECI_ISO_8859_5,
    ISO_8859_6 = QUIRC_ECI_ISO_8859_6,
    ISO_8859_7 = QUIRC_ECI_ISO_8859_7,
    ISO_8859_8 = QUIRC_ECI_ISO_8859_8,
    ISO_8859_9 = QUIRC_ECI_ISO_8859_9,
    WINDOWS_874 = QUIRC_ECI_WINDOWS_874,
    ISO_8859_13 = QUIRC_ECI_ISO_8859_13,
    ISO_8859_15 = QUIRC_ECI_ISO_8859_15,
    SHIFT_JIS = QUIRC_ECI_SHIFT_JIS,
    UTF_8 = QUIRC_ECI_UTF_8,
};


std::vector<quirc_data> decode(const py::array_t<uint8_t>& buffer) {
    py::buffer_info info = buffer.request();
    if (info.ndim != 2) {
        throw std::invalid_argument("Must be a 2d buffer");
    }
    int height = info.shape[0];
    int width = info.shape[1];
    return qr::decode(static_cast<uint8_t*>(info.ptr), width, height);
}

py::object Data_payload(const quirc_data& self) {
    return py::bytes(std::string(self.payload, self.payload + self.payload_len));
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

    py::class_<quirc_data>(m, "Data")
        .def_readonly("version", &quirc_data::version)
        .def_property_readonly("ecc_level", [](const quirc_data& self) {return ECCLevel(self.ecc_level);})
        .def_readonly("mask", &quirc_data::mask)
        .def_property_readonly("data_type", [](const quirc_data& self) {return DataType(self.data_type);})
        .def_property_readonly("eci", [](const quirc_data& self) {return ECI(self.eci);})
        .def_property_readonly("payload", &Data_payload)
        ;

    m.def("decode", &decode, "Decode qr codes in image", py::arg("img"));
}
