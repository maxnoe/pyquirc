#ifndef QUIRCPP_DATA
#define QUIRCPP_DATA

#include <vector>

namespace qr {

#include <quirc.h>

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

struct Data {
    int version = 0;
    ECCLevel ecc_level = ECCLevel::M;
    int mask = 0;
    DataType data_type = DataType::BYTE;
    std::vector<uint8_t> payload = {};
    ECI eci = ECI::UNDEFINED;

    Data() {};
    Data(quirc_data data) : 
        version(data.version),
        ecc_level(ECCLevel(data.ecc_level)),
        mask(data.mask),
        data_type(DataType(data.data_type)),
        eci(ECI(data.eci))
    {
        payload = std::vector<uint8_t>(data.payload, data.payload + data.payload_len);
    }
};

}


#endif /* ifndef QUIRCPP_DATA */
