# we read images using pillow, numpy arrays do also work
from PIL import Image, ImageOps
import quirc

img = Image.open("resources/helloworld.png")

# we need to convert to grayscale for quirc
img = ImageOps.grayscale(img)

# the image must support the 2d buffer protocal with data type uint8
decoded_codes = quirc.decode(img)

print(f'Found {len(decoded_codes)} qr code(s) in the image')

for code, data in decoded_codes:
    print(data)
