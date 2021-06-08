import serial
import time
import numpy as np
import cv2
import os
from getkeys import key_check
from tensorflow.keras.models import load_model
import struct

#train_data = np.load('26May2021.npy', allow_pickle=True)

def map (x, in_min, in_max, out_min, out_max):
  mapped_out = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
  return mapped_out

predicted_avg_steering = [2047, 2047, 0]

def main():
    serial_com = serial.Serial('com5',9600)
    serial_com.timeout = 1


    model = load_model('ADP0.h5')

    prev_frame_time = 0
    new_frame_time = 0

    video = cv2.VideoCapture(1)
    while True:
    #for i in range (len(train_data)):
        #frame,output = train_data[i]
        #cv2.imshow('image',frame)
        #print(output)

        _, frame = video.read()

        frame = cv2.resize(frame, (672,188))
        frame = frame [1:188, 1:336]
        cv2.imshow('Video', frame)

        prediction = model.predict([frame.reshape(-1,187,335,3)])[0]
        predicted_steering = prediction[0]
        predicted_throttle = prediction[1]

        if predicted_steering < 0:
            predicted_steering = int(map(predicted_steering, -1, 0, 1365, 2047.5))
        elif predicted_steering > 0:
            predicted_steering = int(map(predicted_steering, 0, 1, 2047.5, 2730))
        else:
            predicted_steering = 0

        predicted_avg_steering[0] = predicted_avg_steering[1]
        predicted_avg_steering[1] = predicted_avg_steering[2]
        predicted_avg_steering[2] = predicted_steering

        predicted_steering = int(sum(predicted_avg_steering)/3)
        predicted_throttle = int(map(predicted_throttle, 0, 1, 0, 1050))

        predicted_steering = str(predicted_steering).zfill(4)
        predicted_throttle = str(predicted_throttle).zfill(4)

        send_data = predicted_steering + predicted_throttle +'\n'

        serial_com.write(send_data.encode())
        time.sleep(0.1)

        new_frame_time = time.time()
        fps = 1/(new_frame_time-prev_frame_time)
        prev_frame_time = new_frame_time
        print('FPS: ',format(fps,'.2f'), send_data)

        cv2.waitKey(5)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
main()
