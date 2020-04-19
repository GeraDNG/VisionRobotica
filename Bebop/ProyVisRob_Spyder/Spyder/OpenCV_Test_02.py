import cv2
import numpy as np
# img = cv2.imread('d:\Temp16\messi5.jpg')
img = cv2.imread('messi5.jpg')

while(1):
    cv2.imshow('image',img)
    k = cv2.waitKey(1) & 0xFF
    if k == 27:
        break
cv2.destroyAllWindows()

