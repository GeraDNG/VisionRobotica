# -*- coding: utf-8 -*-
"""
SET DE INSTRUCCIONES BEBOP Y BEBOP2

CREAR UN OBJETO:

Bebop(drone_type="Bebop") / Bebop(drone_type="Bebop2") 

CONECTAR CON EL DRONE VIA WIFI:

connect(num_retries) / ESPECIFICA EL NUMERO DE INTENTOS DE ESTABLECER COMUNUICACIÓN CON EL DRONE ANTES DE MANDAR UN ERROR

disconnect() / DESCONECTA LA COMUNICACIÓN CON EL DRONE

DESPEGUE Y ATERRIZAJE:
    
safe_takeoff(timeout) / DESPEGA EL DRONE Y VERIFICA QUE SE HAYA REALIZADO. ESTABLECE UN TIMEOUT QUE EN CASO DE SOBREPASAR CANCELA LA OPERACIÓN

safe_land(timeout) / ATERRIZA EL DRONE Y VERIFICA QUE SE HAYA REALIZADO. ESTABLECE UN TIMEOUT QUE EN CASO DE SOBREPASAR CANCELA LA OPERACIÓN

MOVIMIENTO:
    
fly_direct(roll, pitch, yaw, vertical_movement, duration) / MUEVE EL DRONE SEGUN LO INDICADO EN ROLL, PITCH, YAW, VERTICAL_MOVEMENT Y DURATION. 
                                                            LOS PRIMEROS 4 PARAMETROS SE INTRODUCEN EN PORCENTAJE DE SU MAXIMA VELOCIDAD desde -100 hasta 100
                                                            Y LA DURACIÓN DEL MOVIMIENTO EN SEGUNDOS
                                                            
CONFIGURACIÓN:
    
set_max_altitude(altitude)  / ESTABLECE LA ALTITUD MÁXIMA PERMITIDA PARA EL DRONE EN VALORES DESDE 0.5 Y HASTA 150 METROS

set_max_tilt(tilt)   / ESTABLECE LA INCLINACIÓN MÁXIMA PARA LOS MOVIMIENTOS DE ROLL, PITCH, YAW. SE MIDE EN GRADOS Y VA DESDE
                        5(MOVIMIENTO LENTO) HASTA 30 (MUY RÁPIDO)
                        
set_max_vertical_speed(speed)  / ESTABLECE LA VELOCIDAD MÁXIMA VERTICAL EN METROS/SEGUNDO VA DESDE 0.5 HASTA 2.5

REVISAR LOS SENSORES DEL DRONE:
    
ask_for_state_update()  / ENVÍA LA SOLICITUD AL DRONE PARA QUE DEVUELVA LA INFORMACIÓN ACTUALIZADA DE LOS SENSORES

bebop.sensors.SENSOR   / DEVUELVE LA INFORMACIÓN DEL SENSOR INDICADO
POR EJEMPLO:
bebop.sensors.flying_state
bebop.sensors.battery

    
"""
from pyparrot.Bebop import Bebop #Carga la libreria del drone

bebop = Bebop(drone_type="Bebop") #Se crea el objeto bebop

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
    bebop.safe_land(10)             #Aterriza el drone
    bebop.ask_for_state_update()    #Actualiza la información de los sensores
    print("flying state is %s" % bebop.sensors.flying_state) #imprime el estado del drone
    print("Battery is %s" % bebop.sensors.battery)  #imprime el estado de la bateria
    print("DONE - disconnecting")   
    bebop.disconnect()              #Termina la conexión con el drone
