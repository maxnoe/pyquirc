'''
Decode qr codes in input image and print their payloads to stdout one
payload per line
'''
import argparse
import logging

from . import decode


parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('inputfile')
parser.add_argument('-v', '--verbose', action='store_true')

log = logging.getLogger(__name__)


def main():
    logging.basicConfig(
        format='%(levelname)s %(message)s',
        level=logging.WARNING
    )
    logging.captureWarnings(True)

    try:
        from PIL import ImageOps, Image
    except ImportError:
        logging.fatal("This tool needs `pillow` installed")
        raise SystemExit(1)

    args = parser.parse_args()
    if args.verbose:
        logging.getLogger().setLevel(logging.INFO)

    img = Image.open(args.inputfile)
    gray = ImageOps.grayscale(img)

    decoded = decode(gray)
    n_decoded = len(decoded)
    log.info(f'Found {n_decoded} qr code{"s" if n_decoded > 1 else ""} in image')
    for i, qrcode in enumerate(decoded, start=1):
        log.info(f'--- Payload {i} ---')
        print(qrcode.payload.decode('ascii'))


if __name__ == '__main__':
    main()
