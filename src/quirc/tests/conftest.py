import pytest
import qrcode
from PIL import ImageOps, Image


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

    content = 'https://example.org'
    qr.add_data(content)
    qr.make(fit=True)
    img = qr.make_image()
    return qr, ImageOps.grayscale(img), content.encode('ascii')


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
