from .quirc import decode, Data, DataType, ECI, ECCLevel

__all__ = [
    'decode',
    'Data',
    'DataType',
    'ECI',
    'ECCLevel',
]


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
