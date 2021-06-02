import serial
import time

serial_com = serial.Serial('com9',9600)
serial_com.timeout = 1

for i in range(10):
    i = str('10022001')
    serial_com.write(i.encode())
    time.sleep(0.1)
    #data = serial_com.readline().decode().strip('\r\n')
    #output_data = data.split("x")

    print('steering: ')
