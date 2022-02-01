#include <string>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "quirc++/decoder.h"
#include "quirc++/data.h"

namespace py = pybind11;

using qr::Decoder;
using qr::Data;
using qr::ECI;
using qr::ECCLevel;
using qr::DataType;


Decoder Decoder_from_image(py::buffer buffer) {
    py::buffer_info info = buffer.request();
    if (info.ndim != 2) {
        throw std::invalid_argument("Image must be 2d uint8 (8-bit grayscale)");
    }

    if (info.format != py::format_descriptor<uint8_t>::format()) {
        throw std::invalid_argument("Image must be 2d uint8 (8-bit grayscale)");
    }

    auto height = static_cast<size_t>(info.shape[0]);
    auto width = static_cast<size_t>(info.shape[1]);
    return Decoder(static_cast<uint8_t*>(info.ptr), width, height);
}

Decoder Decoder_from_bytes(py::buffer buffer, size_t width, size_t height) {
    py::buffer_info info = buffer.request();
    if (info.ndim != 1) {
        throw std::invalid_argument("Image must be a bytes-like object");
    }

    auto expected_size = static_cast<py::ssize_t>(width * height);
    if (info.size != expected_size) {
        throw std::invalid_argument("Size of buffer does not match image dimensions");
    }

    return Decoder(static_cast<uint8_t*>(info.ptr), width, height);
}

py::object Data_payload(const Data& self) {
    return py::bytes(std::string(self.payload.begin(), self.payload.end()));
}


std::vector<std::vector<bool>> Code_matrix(const qr::quirc_code& self) {
    size_t size = static_cast<size_t>(self.size);
    std::vector<std::vector<bool>> matrix(size);
    for (size_t row=0; row < size; row++) {
        matrix[row].resize(size);
        for (size_t col=0; col < size; col++) {
            size_t i = row * size + col;
	        matrix[row][col] = self.cell_bitmap[i >> 3] & (1 << (i & 0b111));
        }
    }
    return matrix;
}


PYBIND11_MODULE(quirc, m) {
    m.doc() = "Python bindings for quirc";

    py::class_<qr::quirc_point>(m, "Point")
        .def_readonly("x", &qr::quirc_point::x)
        .def_readonly("y", &qr::quirc_point::y)
        .def("__repr__", [](const qr::quirc_point& self){
            std::stringstream s;
            s << "Point(" << self.x << ", " << self.y << ")";
            return s.str();
        })
        ;

    py::class_<qr::quirc_code>(m, "Code")
        .def_property_readonly("corners", [](const qr::quirc_code& self) {
            return std::vector<qr::quirc_point>(self.corners, self.corners + 4);
        })
        .def_readonly("size", &qr::quirc_code::size)
        .def_property_readonly("matrix", &Code_matrix)
        ;

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
        .def_readonly("eci", &Data::eci)
        .def_property_readonly("payload", &Data_payload)
        ;

    py::class_<Decoder>(m, "Decoder")
        .def(py::init(&Decoder_from_image), py::arg("img"))
        .def_static("from_bytes", &Decoder_from_bytes, py::arg("bytes"), py::arg("width"), py::arg("height"))
        .def("__len__", &Decoder::count)
        .def("__getitem__", &Decoder::decode_index, py::arg("index"))
        .def("extract", &Decoder::extract)
        .def("decode", &Decoder::decode)
        ;
}
