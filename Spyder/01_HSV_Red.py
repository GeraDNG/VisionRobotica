import numpy as np
import cv2

cap = cv2.VideoCapture(0) #Capture images from Laptop Camera

while( 1 ):
    # Take a frame the laptop camera:
    _, frame = cap.read()
    
    # Convert RGB to HSV:
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    # Define the Ranges for the intended Blue to be tracked:
    # (First argument in range is Hue, followed by Saturation and then Value)
    lower_Blue = np.array([0, 50, 50])
    upper_Blue = np.array([30, 255, 255])

    # Treshold the HSV image to get only Blue colors:
    mask = cv2.inRange(hsv,lower_Blue,upper_Blue)

    # Bitwise-AND mask with the original image:
    res = cv2.bitwise_and(frame, frame, mask = mask)

    # Display results:
    cv2.imshow('frame', frame)  # Original image
    cv2.imshow('mask', mask)    # Mask image
    cv2.imshow('res', res)      # Result image

    # Break/end the infinite loop (while), when 'Esc' is pressed:
    keyPressed = cv2.waitKey(5) & 0xFF

    # Check if keyPressed is 'Esc':
    if keyPressed == 27:
        break

cap.release()
cv2.destroyAllWindows()