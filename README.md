Follow this tutorial https://exploreembedded.com/wiki/Secure_IOT_with_AWS_and_Hornbill_ESP32

This tutorial does the following in AWS: 

1 Setup AWS account / IoT Core

2 Create a thing in IoT Core

3 Generate cerficates and keys for the thing and download, activate certificate

4 Define a policy for the IoT device, and attach the policy to the certifcates

5 Test setup using MQTT



Arduino IDE

Install the Arduino core for ESP32 WiFi chip. 
To do this in Arduino go to Preferences window. 
Enter https://dl.espressif.com/dl/package_esp32_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
Open Boards Manager from Tools > Board menu and install esp32 platform (and don't forget to select your ESP32 board from Tools > Board menu after installation).

(More details here https://github.com/espressif/arduino-esp32) 


You also need to install this library: https://github.com/256dpi/arduino-mqtt

Set the following parameters in aws.ino

const char *ssid = "your wifi ssid";
const char *pass = "your wifi password";

const char *awsEndPoint = xxxxxxxxxxxxx-ats.iot.us-west-2.amazonaws.com";		// get from AWS IoT Core, in Things\thing name\Interact and copy HTTPS link

const char *subscribeTopic = "intopic"; 	// set to any topic name. This is the topic the device will listen to
const char *publishTopic = "outtopic";		// set to any topic name. This is the topic the device will upload data to

Also in aws.ino, update the rootCABuff certificateBuff privateKeyBuff array variables with the Root CA (cert), certificate.pem.crt (cert) and private.pem.key downloaded in part 3. (Note, make sure to have "/n"'s in correct format). 

Compile and upload to ESP32

In AWS IoTCore, go to Test, then to Subscribe. Subscribe to topic outtopic and you should see the messages from the device appear, showing they are received by AWS IoT Core MQTT broker. 


