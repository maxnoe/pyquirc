import pytest


def test_simple(hello_world):
    import quirc

    codes = quirc.decode(hello_world)
    assert len(codes) == 1

    code = codes[0]
    assert code.payload == b'Hello, World!'
    assert code.eci == quirc.ECI.UNDEFINED
    assert code.data_type == quirc.DataType.BYTE
    assert code.version == 1


def test_ecc_level_Q(ecc_level_Q):
    import quirc
    _, img, content = ecc_level_Q

    codes = quirc.decode(img)
    assert len(codes) == 1

    code = codes[0]
    assert code.ecc_level == quirc.ECCLevel.Q
    assert code.payload == content


def test_two(two_codes):
    import quirc

    # Somehow quirc finds 3 qr codes in the image with only two
    # and decoding # the fake one fails
    with pytest.warns(quirc.DecodingFailedWarning):
        codes = quirc.decode(two_codes)

    assert len(codes) == 2
    payloads = {c.payload for c in codes}
    assert b'I am the first QR Code' in payloads
    assert b'I am the second QR Code' in payloads


def test_numpy(hello_world):
    np = pytest.importorskip("numpy")
    import quirc

    img = np.array(hello_world)
    codes = quirc.decode(img)

    assert len(codes) == 1
    assert codes[0].payload == b'Hello, World!'
