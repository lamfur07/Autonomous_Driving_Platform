import numpy as np
import cv2

train_data = np.load('30June2021-Turns.npy', allow_pickle=True)

for i in range (len(train_data)):
    img,output = train_data[i]
    cv2.imshow('image',img)
    print(output)
    cv2.waitKey(10)
