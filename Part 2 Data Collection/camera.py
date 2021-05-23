import serial
import time
from getkeys import key_check
import numpy as np
import cv2
import os

starting_value = 1
training_data = []
while True:
    file_name = 'training_data-{}.npy'.format(starting_value)

    if os.path.isfile(file_name):
        print('File exists, moving along',starting_value)
        starting_value += 1
    else:
        print('File does not exist, starting fresh!',starting_value)
        break

def main(file_name, starting_value):
    serial_read = serial.Serial('COM5',9600)
    serial_read.timeout = 1
    video = cv2.VideoCapture(1)

    training_data = []

    while True:
        _, frame = video.read()

        frame = cv2.resize(frame, (672,188))

        cv2.imshow('Video', frame)

        #read steering and throttle
        data = serial_read.readline().decode().strip('\r\n')
        output_data = data.split("x")
        arr = np.asarray(output_data, dtype=np.int32)
        steering = arr [0]
        throttle = arr [1]
        is_collect = arr[2]
        #print(arr)

        #save the collected data for training
        if is_collect == 1 and len(arr) == 11:
            print(arr)
            output = [steering, throttle]
            training_data.append([frame, output])
            if len(training_data) % 100 == 0:
                print('currently at: ',len(training_data))

            if len(training_data) == 1000:
                np.save(file_name,training_data)
                print('ALL DONE SAVED')
                training_data = []
                starting_value += 1
                file_name = 'training_data-{}.npy'.format(starting_value)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break



    #Shutdown camera
    video.release()
    cv2.destroyAllWindows()

main(file_name, starting_value)
