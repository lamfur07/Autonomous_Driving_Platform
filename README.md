# Autonomous-Driving-Platform (ADP)
SFU MSE capstone project Jan 2021-Aug 2021 

Supervisor: Dr. Rad 

## Team Members
Abduallah Tariq

Afnan Hassan 

Alzaib Karovalia

Mohammad Uzair

Syed Imad Rizvi

# Demo

## Car's Prespective

![gif](Demo/demo1.gif)

## Third Person Prespective
![gif](Demo/demo2.gif)


# Project Goal
The goal of this project is to build a self-driving vehicle (prototype), using computer vision and deep neural networks, to navigate in various environments. To achieve this goal, the project is divided into several milestones. 

The prototype of the autonomous driving platform is installed in Henes Broon F8.

![image](https://user-images.githubusercontent.com/46413396/129677302-53f791e4-fb5f-4188-af04-75292db8469e.png)



## Part 1: Car Hacking

To hack the car, potentiometers on the steering, throttle and brake are replaced with DACs MCP4725. These DACs are then controlled via Arduino Uno. To wirelessly control the car, the Arduino is connected to a radio receiver. This receiver receives the input signal from the transmitter, which is controlled by the driver. Then these signals are sent to Arduino, which then maps it to generate the voltage between 1.1V to 2.2V (i.e., left to right) via DACs

![image](https://user-images.githubusercontent.com/46413396/129677372-3a1a43b0-68c4-4e37-a7bf-42a5afb181f0.png)

## Part 2: Data Collection

Two different sets of data is collected/download to train the PilotNet (predicting steering) and YOLOv4-Tiny (detecting custom objects).

For the train PilotNet, and test our ADP system on the sidewalk, over 70,000 images were collected by driving the car around the block for a few weeks. Different pathways were chosen to get different data to train the model under different operating conditions. During the data collection, the PWM values from the receiver are read by Arduino. These values are then mapped for the DACs' input. These mapped values are sent to the OBC through serial communication. A Python script on on-board computer (laptop) records the frame input from the camera, and associates steering and throttle values from the serial port.

![image](https://user-images.githubusercontent.com/46413396/129677439-5fc3fdc1-33bb-46e1-a39e-af1ec4e1033a.png)

To train YOLO object detection model, predefined datasets from Kaggle and Google were downloaded. Based on the system requirements, custom classes were built to detect certain signs. These include traffic signs, stop signs, pedestrians, and so forth. Once these classes were created, the downloaded datasets were hand-labeled in accordance with the classes created. In total, over 1100 images were collected for pedestrians, vehicles, stop signs, and 30km/h speed signs in YOLO format. 

![image](https://user-images.githubusercontent.com/46413396/129677465-08db75f8-6009-4fa1-93c3-fca28584513c.png)

## Part 3: AI Training 

The PilotNet, originally proposed by NVIDIA, consists of 9 layers; 5 convolutional layers, 3 fully connected layers, and one normalization layer. We modified the architecture of PilotNet with 5 convolution layers, and 4 fully connected layers. 

As the car is driven straight for most of the part, most of the collected data lie about steering angle 0. Training a neural network on such a dataset would result in biased prediction to drive straight all the time, therefore resulting in poor performance by the network. The excess data, above the threshold, from each bin, is randomly deleted. However, to better balance the overall dataset, each image is flipped and added to the dataset.

![image](https://user-images.githubusercontent.com/46413396/129677498-889687b7-cd6b-450b-81e8-943a23bd4c25.png)

![image](https://user-images.githubusercontent.com/46413396/129677528-951ef526-5422-40bd-a644-3a945be96915.png)

![image](https://user-images.githubusercontent.com/46413396/129677981-b00dc76e-8a76-4bb9-8175-6f313f4b449e.png)

## Part 4: Integration and Testing  

Before testing, we needed to make sure that the remote can override the AI mode in the car at any time, and that the user can take control whenever they please. Testing was carried out on the sidewalk with object detections such as vehicles, pedestrians, stop signs, and speed signs. 

![image](https://user-images.githubusercontent.com/46413396/129678151-5889f71e-eb29-469b-b69c-0e6300812999.png)

# Future Plans
1- Additional cameras surround the car so our model has complete 360-degree awareness of its surroundings.Our team plans on adding at least 3 more cameras to cover each side of the vehicle.

2- Our team plans on training more data as the results of our ADP system are directly proportional to the amount of data trained. 

3- Broaden the object detection dataset to include further traffic signs such as school zones, bicycles, one-way traffic, and merge signs

4- Adding an LSTM (memory) model with a CNN model that would enable time-dependent predictions for our ADP system.

5- Use an embedded computer such as NVIDIA Jetson to eliminate the need of having a laptop to run the neural networks. 

# References

“Broon F810 The Instructions Manual,” Manuals Library. [Online]. Available: https://www.manualslib.com/manual/1259961/Broon-F810.html#product-F830.

Redmon, "YOLO: Real-Time Object Detection", Pjreddie.com, 2021. [Online]. Available: https://pjreddie.com/darknet/yolo/.

M. Bojarski, L. Jackel, B. Firner and U. Muller, "Explaining How End-to-End Deep Learning Steers a Self-Driving Car | NVIDIA Developer Blog", NVIDIA Developer Blog, 2021. [Online]. Available: https://developer.nvidia.com/blog/explaining-deep-learning-self-driving-car/.
