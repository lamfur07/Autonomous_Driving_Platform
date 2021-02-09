import serial
import time
from getkeys import key_check
import numpy as np


serial_read = serial.Serial('COM5',9600)
serial_read.timeout = 1



while True:
    for i in range (2):
        data = serial_read.readline().decode().strip('\r\n')
        output = data.split("x")
        arr = np.asarray(output, dtype=np.int32)
        print(arr)
    
    '''
        if data [0] == 'A':
            sensor_A = data [1:]
            print("data from sensor A: ",sensor_A)
        if data [0] == 'B':
            sensor_B = data [1:]
            print("data from sensor B: ",sensor_B)
        
        #time.sleep(0.05)
    
    #output = np.array[sensor_A, sensor_B]
    '''
    #print(output)
    keys = key_check()
    if 'Q' in keys:
        break

serial_read.close()

    


'''
serialcomm = serial.Serial('COM5', 9600)
serialcomm.timeout = 1

while True:
    i = input("input(on/off): ").strip()
    if  i == 'done':
        print ("finished program")
        break

    serialcomm.write(i.encode())
    time.sleep(0.2)
    print(serialcomm.readline().decode('ascii'))

serialcomm.close()
'''