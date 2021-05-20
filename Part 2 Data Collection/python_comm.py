import serial
import time
from getkeys import key_check
import numpy as np
import cv2
import os



def main():


    video = cv2.VideoCapture(0)


    while True:
        #read camera frame
        _, frame = video.read()
        cv2.imshow('video', frame)



        #print(output)
        keys = key_check()
        if 'Q' in keys:
            break
main()
