import pytest
from PIL import ImageOps, Image
import qrcode


@pytest.fixture(scope='session')
def hello_world():
    img = Image.open('resources/helloworld.png')
    return ImageOps.grayscale(img)


@pytest.fixture(scope='session')
def ecc_level_Q():
    qr = qrcode.QRCode(
        version=1,
        error_correction=qrcode.constants.ERROR_CORRECT_Q,
    )

    qr.add_data('https://example.org')
    qr.make(fit=True)
    img = qr.make_image()
    return ImageOps.grayscale(img)


@pytest.fixture(scope='session')
def two_codes():
    code1 = qrcode.make('I am the first QR Code').get_image()
    code2 = qrcode.make('I am the second QR Code').get_image()

    width = code1.width + code2.width
    height = max(code1.height, code2.height)

    img = Image.new('L', (width, height))

    img.paste(code1, (0, 0))
    img.paste(code2, (code1.width, 0))
    return img


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

    codes = quirc.decode(ecc_level_Q)
    assert len(codes) == 1

    code = codes[0]
    assert code.ecc_level == quirc.ECCLevel.Q
    assert code.payload == b'https://example.org'


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
