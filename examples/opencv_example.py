import cv2
import quirc
import warnings

warnings.simplefilter('ignore', category=quirc.DecodingFailedWarning)

cam = None
n_codes = 0
payloads = set()

try:
    cam = cv2.VideoCapture(0)

    while True:
        check, frame = cam.read()

        if check:
            cv2.imshow('Camera 0 (Press Q to quit)', frame)
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            codes = quirc.decode(gray)

            print(f'Found {len(codes):2d} in frame', end='\r')

            for code in codes:
                if code.payload in payloads:
                    continue

                payloads.add(code.payload)

                n_codes += 1
                print(f'------ Code {n_codes} ------')
                print(code.payload.decode('ascii'))
                print('')

        key = cv2.waitKey(1) & 0xff
        if key == ord('q'):
            break
finally:
    if cam is not None:
        cam.release()
        cv2.destroyAllWindows()
