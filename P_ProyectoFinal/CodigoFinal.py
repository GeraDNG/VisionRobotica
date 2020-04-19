import cv2
import time
import numpy as np
import threading
from pyparrot.Bebop import Bebop
from pyparrot.DroneVisionGUI import DroneVisionGUI

isAlive = False

class UserVision:
    def __init__(self, vision):
        self.index = 0
        self.vision = vision

    def save_pictures(self, args):
        # Saving picture:
        img = self.vision.get_latest_valid_picture()

        if (img is not None):
            filename = "test_image_%06d.png" % self.index           # The name of the picture taked by the drone, to be saved
            #cv2.imwrite(filename, img)                             # Uncommenting this line, will save all the image in the file's folder. CAUTION!!!
            self.index +=1


def demo_user_code_after_vision_opened(bebopVision, args):
    bebop = args[0]

    print("Vision successfully started!")
    print("Battery is %s" % bebop.sensors.battery)                  # Write the porcentage of battery when starting the flight.
    while(True):
        try:
            # Variable name = Command to read the image saved before (on the default folder):
            frame = cv2.imread(r'C:\ProgramData\Anaconda3\Lib\site-packages\pyparrot\images\visionStream.jpg')
            
            # Convert BGR to HSV:
            HSV = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

            # Define range of desired color in HSV:
            lowerLimit = np.array([0, 65 , 192], dtype = np.uint8)     # Defined as unsigned int of 8 bits. Order: H, S, V.
            upperLimit = np.array([34, 255, 255], dtype = np.uint8)     # Yellow color selected.


            # Threshold the HSV image to get only the selected color:
            Threshold = cv2.inRange(HSV, lowerLimit, upperLimit)

            # Bitwise-AND:
            filtered = cv2.bitwise_and(frame, frame, mask = Threshold)     # This will combine the original image and the filter color selected in one image.
            
            # Blur Filter:
            Blur = cv2.medianBlur(filtered,5)                       # Making a Blur in order to reducing noise.

            # Convert to Gray scale:
            grayImage = cv2.cvtColor(Blur, cv2.COLOR_BGR2GRAY)

            # Gonig forward:
            bebop.smart_sleep(3)                                        # Delay.
            bebop.fly_direct(0,30,0,0,2)                                # Going fordward.

            # Circle detection, using Hough function:
            print("Looking for the correct Circle...")
            Circle = cv2.HoughCircles(grayImage,cv2.HOUGH_GRADIENT, 1, 150, param1 = 50, param2=30, minRadius=0, maxRadius=0);

            # Verify if a Circle is detected and Draw it:
            if Circle is not None:
                print("Circle Detected!")
                Circle = np.uint16(np.around(Circle))
                for i in Circle[0,:]:
                    # Draw the circle:
                    grayImage = cv2.circle(filtered,(i[0],i[1]),i[2],(0,0,255),3)   # Draw the outer of the Circle. 
                    grayImage = cv2.circle(filtered,(410,240),2,(0,0,255),3)        # Draw the center of the Image.

                    # Get the circle position:
                    Circle_Width  = i[0]                                            # Get the position of the Drone.
                    Circle_Height = i[1]                                            # Get the position of the Drone.
                    
                    # Define the drone position, always is the center of the image:
                    Drone_height = 240 
                    Drone_width = 420

                # Drone movement to correct the position and go to the center of the circle:
                print("Going to the center...")
                # Check if is displaced: 
                if Circle_Width > (Drone_width+15):                 # Check if the drone is on the left of the circle.
                    bebop.fly_direct(6,0,0,0,2)                     # Move to the right.
                    bebop.smart_sleep(2)                            # Delay.
                if Circle_Width < (Drone_width-15):
                    bebop.fly_direct(-6,0,0,0,2)
                    bebop.smart_sleep(2)
                if Circle_Height > (Drone_height+15): 
                    bebop.fly_direct(0,6,0,0,2)
                    bebop.smart_sleep(2)
                if Circle_Height < (Drone_height-15):
                    bebop.fly_direct(0,-6,0,0,2)
                    bebop.smart_sleep(2)

                elif (Circle_Height < (Drone_height+50)) or (Circle_Height > (Drone_height-50)) or (Circle_Width < (Drone_width+50)) or (Circle_Width > (Drone_width-50)):
                    #bebop.fly_direct(-5,13,0,0,2)                  # Correction for the air.
                    bebop.safe_land(8)                              # Land the Drone.
                    print("Center finded. Landing...")

            # Display the images:
            cv2.imshow('Original', frame)                           # Original image.
            cv2.imshow('Gray Scale', grayImage)                     # GrayScale image.
            #cv2.imshow('Filtered', filtered)                        # Only the selected color (filtered) image.

            # Safety key!
            k = cv2.waitKey(5) & 0xFF                               # Press 'Esc' to break and land the drone.
            if k == 27 or k == 20 or keyboard.is_pressed('Esc'):
                cv2.destroyAllWindows()
                break
  
        except:
            pass
    
    print("Battery is %s" % bebop.sensors.battery)                  # Write the porcentage of battery at the end of the flight.
    
    # Land the drone:
    bebop.safe_land(10)                  
    print("Drone landed")

    #bebop.ask_for_state_update()                                    # Update sensors information.
    #print("flying state is %s" % bebop.sensors.flying_state)        # Print the drone status.
    
    # Finish the connection to the drone:
    bebop.disconnect()                                              
    print("DONE - disconnected")

if __name__ == "__main__":
    # Make the bebop object:
    bebop = Bebop(drone_type="Bebop2")                              # Write the Bebop number, in this case is bebop2.

    # Connect to the bebop:
    print("Connecting to bebop...") 
    success = bebop.connect(5)                                      # Verify the drone connection.

    if (success):
        Done = 0
        print("Connection succesfull.")
        bebop.smart_sleep(4)                                        # Delay to ensure that the drone receives the commands well.

        # Set the limits for the drone:
        bebop.set_max_altitude(2)                                   # Set the maximum altitude in 2 meters.
        bebop.set_max_tilt(5)                                       # Sets the maximum speed of angular movements in 5°.
        bebop.set_max_vertical_speed(0.5)                           # Sets the maximum vertical speed at 0.5 m/s.

        bebop.safe_takeoff(10)                                      # Drone take off.
        bebop.smart_sleep(3)                                        # Delay to stabilize the drone in the air.

        bebop.fly_direct(0,0,0,25,3)
        bebop.smart_sleep(3)
        
        
        # Check:
        #print("Checking sensors...")
        #bebop.ask_for_state_update()                                # Update sensors information.
        #print("Flying state is %s" % bebop.sensors.flying_state)    # Print the drone status.
        #bebop.smart_sleep(4)                                        # Delay to stabilize the drone in the air.

        # Move the camera:
        bebop.pan_tilt_camera(-90,0)                                # Rotate the camera (-90° means downside and 0° means in front)
        bebop.smart_sleep(3)                                        # Delay

        # Start up the video:
        bebopVision = DroneVisionGUI(bebop, is_bebop=True, user_code_to_run=demo_user_code_after_vision_opened,
                                     user_args=(bebop, ))

        userVision = UserVision(bebopVision)
        bebopVision.set_user_callback_function(userVision.save_pictures, user_callback_args=None)
        bebopVision.open_video()

        bebop.smart_sleep(3)                                        # Delay

    else:
        print("Error connecting to bebop.  Retry")                  # Print only if an error happends.
