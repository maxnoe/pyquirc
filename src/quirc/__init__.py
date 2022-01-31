import warnings

from .quirc import Decoder, Data, DataType, ECI, ECCLevel

__all__ = [
    'Decoder',
    'Data',
    'DataType',
    'ECI',
    'ECCLevel',
]


def decode(img):
    decoder = Decoder(img)
    n_codes = len(decoder)

    codes = []
    for i in range(len(decoder)):
        try:
            codes.append(decoder[i])
        except RuntimeError:
            warnings.warn(f"Failed to decode index {i} ({n_codes} codes total)")

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


Data.__repr__ = _data_repr
