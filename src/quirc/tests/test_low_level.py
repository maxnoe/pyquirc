'''Test the Decoder interface directly'''
import pytest


def test_decoder(ecc_level_Q):
    from quirc import Decoder

    qr, img, content = ecc_level_Q

    decoder = Decoder.from_bytes(img.tobytes(), img.width, img.height)
    assert len(decoder) == 1

    code = decoder.extract(0)
    corners = {(c.x, c.y) for c in code.corners}
    assert code.size == qr.modules_count
    assert (qr.box_size * qr.border, qr.box_size * qr.border) in corners
    assert (qr.box_size * (qr.border + code.size), qr.box_size * qr.border) in corners
    assert (qr.box_size * (qr.border + code.size), qr.box_size * (qr.border + code.size)) in corners
    assert (qr.box_size * qr.border, qr.box_size * (qr.border + code.size)) in corners

    data = decoder.decode(code)
    assert data.payload == content

    with pytest.raises(TypeError):
        decoder.extract(-1)

    with pytest.raises(IndexError):
        decoder.extract(1)
