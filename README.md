# pyquirc

Python and C++ bindings for the [quirc](https://github.com/dlbeer/quirc) library, created using [pybind11](https://github.com/pybind/pybind11).

## Example

```python
# we read images using pillow, numpy arrays do also work
from PIL import Image, ImageOps
import quirc

img = Image.open("resources/helloworld.png")

# we need to convert to grayscale for quirc
img = ImageOps.grayscale(img)

# the image must support the 2d buffer protocal with data type uint8
decoded_codes = quirc.decode(img)

print(f'Found {len(decoded_codes)} qr code(s) in the image')

for data in decoded_codes:
    print(data)
    
```

Output:
```
Found 1 qr code(s) in the image
Data(payload=b'Hello, World!', version=1, data_type=DataType.BYTE, eci=ECI.UNDEFINED, ecc_level=ECCLevel.M, mask=5)
```
