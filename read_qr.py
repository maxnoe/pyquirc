import argparse
from PIL import ImageOps, Image
import quirc


parser = argparse.ArgumentParser()
parser.add_argument('inputfile')


def main():
    args = parser.parse_args()
    img = Image.open(args.inputfile)
    gray = ImageOps.grayscale(img)

    decoded = quirc.decode(gray)
    n_decoded = len(decoded)
    print(f'Found {n_decoded} qr code{"s" if n_decoded > 1 else ""} in image')
    for qrcode in decoded:
        print('--- Payload ---')
        print(qrcode.payload.decode('ascii'))
        print()


if __name__ == '__main__':
    main()
