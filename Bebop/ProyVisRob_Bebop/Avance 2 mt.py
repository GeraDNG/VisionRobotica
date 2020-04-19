from pyparrot.Bebop import Bebop #Carga la libreria del drone

bebop = Bebop(drone_type="Bebop2") #Se crea el objeto bebop

print("connecting") 
success = bebop.connect(10) #Verifica la conexión
print(success)

# No cambiar los valores de las siguientes 3 configuraciones !!!!!!
bebop.set_max_altitude(2)           #Establece la altitud máxima en 3 metros
bebop.set_max_tilt(5)               #Establece la velocidad máxima de movimientos angulares en 5°
bebop.set_max_vertical_speed(0.5)   #Establece la velocidad máxima vertical en 0.5 m/s

if (success):
    print("sleeping")
    bebop.smart_sleep(4)            #Realiza una pausa en el programa
    bebop.safe_takeoff(10)          #Despega el drone
    bebop.ask_for_state_update()    #Actualiza la información de los sensores
    print("flying state is %s" % bebop.sensors.flying_state) #imprime el estado del drone
    bebop.smart_sleep(4)            #El drone se mantiene 4 segundos en el aire
    # bebop.fly_direct(30, 30, 0, 0, 3) #45 degrees forward
    bebop.fly_direct(0, 0, 100, 0, 5) #90 degrees yaw
    bebop.smart_sleep(4) 
    bebop.safe_land(10)             #Aterriza el drone
    bebop.ask_for_state_update()    #Actualiza la información de los sensores
    print("flying state is %s" % bebop.sensors.flying_state) #imprime el estado del drone
    print("Battery is %s" % bebop.sensors.battery)  #imprime el estado de la bateria
    print("DONE - disconnecting")   
    bebop.disconnect()              #Termina la conexión con el drone
