# pyquirc

Python and C++ bindings for the [quirc](https://github.com/dlbeer/quirc) library, created using [pybind11](https://github.com/pybind/pybind11).

## Installation

The package is on pypi, just

```
$ pip install pyquirc
```

There is an older, no longer maintained package `quirc`, not to be confused.

## Example

```python
# we read images using pillow, numpy arrays do also work
from PIL import Image, ImageOps
import quirc

img = Image.open("resources/helloworld.png")

# we need to convert to 8-bit grayscale for quirc
img = ImageOps.grayscale(img)

# Images must either have `tobytes()`, `width` and `height` like PIL images
# or support the 2d buffer protocal, both with data type uint8 (8-Bit Grayscale)
decoded_codes = quirc.decode(img)

print(f'Found {len(decoded_codes)} qr code(s) in the image')

for code, data in decoded_codes:
    print(code)
    print(data)
```

Output:
```
Found 1 qr code(s) in the image
Code(size=21, corners=[Point(40, 40), Point(250, 40), Point(250, 250), Point(40, 250)])
Data(payload=b'Hello, World!', version=1, data_type=DataType.BYTE, eci=ECI.UNDEFINED, ecc_level=ECCLevel.M, mask=5)
```


## On the Command Line

This package also installs the tool `pyquirc_decode`, which will
use `pillow` as above and decode the QR code in the given image.

The payloads of each decoded qr code will be printed to stdout, one per line:

```
$ pyquirc_decode resources/helloworld.png -v
INFO Found 1 qr code in image
INFO --- Payload 1 ---
Hello, World!
```

The log messages come on stderr, so piping the the output will only pipe
the payloads, not the log output.
