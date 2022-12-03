import pytest


def test_simple(hello_world):
    import quirc

    codes = quirc.decode(hello_world)
    assert len(codes) == 1

    code, data = codes[0]
    assert isinstance(code, quirc.Code)
    assert isinstance(data, quirc.Data)
    assert data.payload == b'Hello, World!'
    assert data.eci == quirc.ECI.UNDEFINED
    assert data.data_type == quirc.DataType.BYTE
    assert data.version == 1


def test_ecc_level_Q(ecc_level_Q):
    import quirc
    _, img, content = ecc_level_Q

    codes = quirc.decode(img)
    assert len(codes) == 1

    code, data = codes[0]
    assert data.ecc_level == quirc.ECCLevel.Q
    assert data.payload == content


def test_two(two_codes):
    import quirc

    # Somehow quirc finds 3 qr codes in the image with only two
    # and decoding # the fake one fails
    with pytest.warns(quirc.DecodingFailedWarning):
        codes = quirc.decode(two_codes)

    assert len(codes) == 2
    payloads = {d.payload for _, d in codes}
    assert b'I am the first QR Code' in payloads
    assert b'I am the second QR Code' in payloads


def test_numpy(hello_world):
    np = pytest.importorskip("numpy")
    import quirc

    img = np.array(hello_world)
    codes = quirc.decode(img)

    assert len(codes) == 1
    _, data = codes[0]
    assert data.payload == b'Hello, World!'
