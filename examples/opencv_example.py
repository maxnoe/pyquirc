import cv2
import quirc
import warnings
import numpy as np

warnings.simplefilter('ignore', category=quirc.DecodingFailedWarning)

cam = None
n_codes = 0
payloads = set()
color = (0, 0, 255) # BGR

try:
    cam = cv2.VideoCapture(0)

    while True:
        check, frame = cam.read()

        if check:

            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            codes = quirc.decode(gray)

            print(f'Found {len(codes):2d} in frame', end='\r')

            for code, data in codes:
                if data.payload not in payloads:
                    n_codes += 1
                    payloads.add(data.payload)
                    print(f'------ Code {n_codes} ------')
                    print(data.payload.decode('ascii'))
                    print('')

                corners = np.array([(p.x, p.y) for p in code.corners], np.int32)
                frame = cv2.polylines(frame, [corners], True, color, 3)

            cv2.imshow('Camera 0 (Press Q to quit)', frame)

        key = cv2.waitKey(1) & 0xff
        if key == ord('q'):
            break
finally:
    if cam is not None:
        cam.release()
        cv2.destroyAllWindows()
