import serial
#import multiprocessing as mp
import math
import numpy as np
import cv2
import time
from grabscreen import grab_screen
from tensorflow.keras.models import load_model

THROTTLE_MAX = int(0.45*1050)
DIRECTION_THRESHOLD = 0.3
'''
The following function maps the data from one range to another range
'''
def map (x, in_min, in_max, out_min, out_max):
  mapped_out = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
  return mapped_out

'''
The following function predicted the steering angle using PilotNet
'''
# initialize the array (used to calculate the average of last three predictions)
predicted_avg_steering = [2047, 2047, 0]
prediction_arr = [0 ,0 ,0]

def pilotnet_prediction(model, frame):
    prediction = model.predict([frame.reshape(-1,187,335,3)])[0]
    predicted_steering = prediction[0]

    #map the predicted steering for DAC input
    if predicted_steering < 0:
        predicted_steering = int(map(predicted_steering, -1, 0, 1365, 2047.5))
    elif predicted_steering > 0:
        predicted_steering = int(map(predicted_steering, 0, 1, 2047.5, 2730))
    else:
        predicted_steering = 0

    #store last three predicted steering in an array for averaging
    predicted_avg_steering[0] = predicted_avg_steering[1]
    predicted_avg_steering[1] = predicted_avg_steering[2]
    predicted_avg_steering[2] = predicted_steering


    prediction_arr[0] = prediction_arr[1]
    prediction_arr[1] = prediction_arr[2]
    prediction_arr[2] = prediction

    prediction = sum(prediction_arr)/3

    predicted_steering = int(sum(predicted_avg_steering)/3)

    return predicted_steering, prediction
 # or class_label == 'pedestrians'
def throttle_prediction(class_label, throttle_in):
    throttle = throttle_in
    if class_label == 'stop' or class_label == 'traffic light - red':
        throttle = 0
    elif class_label == 'speed-30 km':
        throttle = throttle*0.3
    elif class_label == 'speed-50 km':
        throttle = throttle*0.5

    return throttle

def getColor(class_label):

    if class_label == 'stop' or class_label == 'traffic light - red':
        color = (0,0,255) # red color

    elif class_label == 'traffic light - green':
        color = (0,255,0) # green color

    elif class_label == 'pedestrain' or class_label == 'pedestrian sign':
        color = (255,0,255) # purple color

    elif class_label == 'speed-50 km' or class_label == 'speed-30 km':
        color = (255,255,255) # white color
    else:
        color = (255,0,0) # blue color

    return color


def main():
    #send steering and throttle data via serial port
    serial_com = serial.Serial('com5',9600)
    serial_com.timeout = 1

    '''Load Modified PilotNet to predict steering'''
    model = load_model('ADP5.h5')

    '''Load YOLO (YOLOv4-Tiny)'''#update this
    net = cv2.dnn.readNet("yolov4-tiny_training_last.weights", "yolov4-tiny_training.cfg")

    classes = []
    with open("classes.txt", "r") as f:
        classes = [line.strip() for line in f.readlines()]

    #get last layers names
    layer_names = net.getLayerNames()
    output_layers = [layer_names[i[0] - 1] for i in net.getUnconnectedOutLayers()]
    confidence_threshold = 0.5
    font = cv2.FONT_HERSHEY_SIMPLEX

    prev_frame_time = 0
    new_frame_time = 0


    video = cv2.VideoCapture(0)
    while True:

        ''' Pilot Net'''
        #read frame from the camera
        _, frame = video.read()

        #resize the frame as per model's input
        frame = cv2.resize(frame, (672,188))
        frame = frame [1:188, 1:336]
        frame_cpy = frame
        #cv2.imshow('camera', frame)
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        #get predicted steering from PilotNet
        predicted_steering, raw_prediction = pilotnet_prediction(model, frame)

        if raw_prediction > -DIRECTION_THRESHOLD and raw_prediction < DIRECTION_THRESHOLD:
            direction = 'Straight: ' + str(raw_prediction)
        elif raw_prediction < -DIRECTION_THRESHOLD:
            direction = 'Left: ' + str(raw_prediction)
        elif raw_prediction > DIRECTION_THRESHOLD:
            direction = 'Right: ' + str(raw_prediction)

        image = cv2.putText(frame_cpy, direction, (10, 50), cv2.FONT_HERSHEY_SIMPLEX,
                   1, (0, 0, 0), 2, cv2.LINE_AA)

        cv2.imshow('camera', frame_cpy)


        predicted_throttle_int = THROTTLE_MAX
        '''YOLO'''
        #_, img = video.read()
        img = frame
        # img = cv2.resize(frame, (672,188))
        # img = img [1:188, 1:336]
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        height, width, channels = img.shape

        #Detecting objects (YOLO)
        blob = cv2.dnn.blobFromImage(img, 0.00392, (416, 416), (0, 0, 0), True, crop=False)
        net.setInput(blob)
        outs = net.forward(output_layers)

        #Showing informations on the screen (YOLO)
        class_ids = []
        confidences = []
        boxes = []
        for out in outs:
            for detection in out:
                scores = detection[5:]
                class_id = np.argmax(scores)
                confidence = scores[class_id]
                if confidence > confidence_threshold:
                    # Object detected
                    center_x = int(detection[0] * width)
                    center_y = int(detection[1] * height)
                    w = int(detection[2] * width)
                    h = int(detection[3] * height)
                    # Rectangle coordinates
                    x = int(center_x - w / 2)
                    y = int(center_y - h / 2)
                    boxes.append([x, y, w, h])
                    confidences.append(float(confidence))
                    class_ids.append(class_id)
        indexes = cv2.dnn.NMSBoxes(boxes, confidences, 0.5, 0.4)
        for i in range(len(boxes)):
            if i in indexes:
                x, y, w, h = boxes[i]
                label = str(classes[class_ids[i]]) + "=" + str(round(confidences[i]*100, 2)) + "%"
                #check predicted class, and react accordingly here (to do)
                class_label = str(classes[class_ids[i]])
                predicted_throttle_int = int(throttle_prediction(class_label,predicted_throttle_int))
                color = getColor(class_label)
                cv2.rectangle(img, (x, y), (x + w, y + h), color, 2)
                cv2.putText(img, label, (x, y), font, 0.5, color, 2)



        # if(predicted_steering<1706 or predicted_steering>2388):
        #     if (predicted_throttle_int > 0.45*THROTTLE_MAX):
        #         predicted_throttle_int = 0.45*THROTTLE_MAX

        cv2.imshow("Image", img)

        #Throttle value from YOLO and LIDAR (To DO)
        #predicted_throttle = int(1050*0.35)

        #convert steering and throttle to 4 digit numbers
        predicted_steering = str(predicted_steering).zfill(4)
        predicted_throttle = str(predicted_throttle_int).zfill(4)

        send_data = predicted_steering + predicted_throttle +'\n'

        # send data via serial communication
        serial_com.write(send_data.encode())
        time.sleep(0.05)

        #calculate the FPS
        new_frame_time = time.time()
        fps = 1/(new_frame_time-prev_frame_time)
        prev_frame_time = new_frame_time
        print('FPS: ',format(fps,'.2f'), predicted_steering,"  ",predicted_throttle)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    video.release()
    cv2.destroyAllWindows()

main()
