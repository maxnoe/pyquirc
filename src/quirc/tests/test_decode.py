import pytest
from PIL import ImageOps, Image
import qrcode


@pytest.fixture(scope='session')
def hello_world():
    img = Image.open('resources/helloworld.png')
    return ImageOps.grayscale(img)



def test_simple(hello_world):
    import quirc

    codes = quirc.decode(hello_world)
    assert len(codes) == 1

    code = codes[0]
    assert code.payload == b'Hello, World!'
