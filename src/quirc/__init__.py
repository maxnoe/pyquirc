'''
Python bindings for the quirc C-library using pybind11
'''
import warnings

from .quirc import Decoder, Data, Code, DataType, ECI, ECCLevel


__all__ = [
    'Code',
    'Decoder',
    'Data',
    'DataType',
    'ECI',
    'ECCLevel',
    'decode',
    'DecodingFailedWarning',
]


class DecodingFailedWarning(UserWarning):
    pass


def _has_pil_image_interface(img):
    '''Check if an object has the PIL image interface for accessing the raw data'''
    attrs = ['tobytes', 'width', 'height']
    return all(hasattr(img, attr) for attr in attrs)


def decode(img):
    '''
    Decode all qr codes quirc can find in img

    Parameters
    ----------
    img: PIL.Image, numpy.ndarray or something else that supports the 2D buffer protocol
        This must be a 2D image in 8-Bit grayscale. Using pillow:

    Returns
    -------
    decoded: list[Data]
        A list of the found and successfully decoded QR codes in `img`

    Example
    -------
    >>> from quirc import decode
    >>> from PIL import Image, ImageOps
    >>> img = ImageOps.grayscale(Image.open("resources/helloworld.png"))
    >>> decode(img)  # doctest: +ELLIPSIS
    [Data(payload=b'Hello, World!', ...)]
    '''
    if _has_pil_image_interface(img):
        decoder = Decoder.from_bytes(img.tobytes(), img.width, img.height)
    else:
        decoder = Decoder(img)

    n_codes = len(decoder)

    codes = []
    for i in range(len(decoder)):
        try:
            codes.append(decoder[i])
        except RuntimeError:
            warnings.warn(f"Failed to decode index {i} ({n_codes} codes total)", DecodingFailedWarning)

    return codes


# easier to do correctly in python
def _data_repr(self):
    return (
        f'Data(payload={self.payload!r}'
        f', version={self.version}'
        f', data_type={self.data_type}'
        f', eci={self.eci}'
        f', ecc_level={self.ecc_level}'
        f', mask={self.mask}'
        ')'
    )


def _code_repr(self):
    return f'Code(size={self.size}, corners={self.corners!r})'


Data.__repr__ = _data_repr
Code.__repr__ = _code_repr
