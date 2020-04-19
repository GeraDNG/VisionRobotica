import keyboard #Using module keyboard
from pyparrot.Bebop import Bebop

bebop = Bebop()
print("connecting")
success = bebop.connect(10)
print(success)

if (success):
    while True:
        
        if keyboard.is_pressed('p'): 
            print('taking off')
            bebop.safe_takeoff(10)
        if keyboard.is_pressed('o'): 
            print('landing')
            bebop.safe_land(10)
        if keyboard.is_pressed('Esc'): 
            break
    print("DONE - disconnecting")
    bebop.disconnect()
            