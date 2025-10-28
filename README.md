
[README.md](https://github.com/user-attachments/files/23193039/README.md)
# AI-based-Dynamic-Traffic-Light-Management-System

This project implements a smart traffic light control system using an Arduino Uno in Proteus software. This system dynamically adjusts green light durations based on real-time traffic density data received via serial communication.


## Features

✅ Adaptive Traffic Control – Dynamically allocates green light duration based on vehicle density.

✅ Serial Communication Input – Accepts traffic density data through the Serial Monitor.

✅ Optimized Timing Algorithm – Ensures efficient traffic flow using a weighted timing system.

✅ Automatic Traffic Light Management – Controls red, yellow, and green lights for four lanes.

✅ Arduino-Based Implementation – Runs on an Arduino Uno, using basic digital I/O pins.

## How It Works

    The Arduino receives traffic density data (0-100) for four lanes via serial input.
    It calculates optimal green light durations based on the relative densities.
    The traffic lights are managed accordingly, ensuring smooth traffic flow.
    The system cycles through lanes based on real-time data updates.


## Setup

### Vehicle density detection using Ultralytics

Ultralytics is a Python library that uses Artificial Intelligence (AI) to perform object detection and image segmentation.

#### Installation

Download the 'yolo11n.pt' file and traffic lane images and save them together in a folder.
Now, in the same folder, download the 'main.py' file for running the AI detection algorithm on the traffic lane images. Open this folder using Visual Studio Code .


To run the ultralytics module, we need python 3.10, as the module only works for python versions ranging from 3.8 to 3.10

Install python3.10 from [python.org](https://www.python.org/downloads/), then open the exe file. While installing, make sure to check the box: ADD TO PATH.

Run the following commands in Visual Studio Code terminal.

To verify the python version installed
```bash
python --version
```

The output should be between the python versions 3.8 to 3.10, like the version shown below:-
```bash
Python 3.10.0
```
Update the pip command

```bash
pip install --upgrade pip setuptools wheel
```
Now install ultralytics

```bash
pip install ultralytics
```


### VSPE

VSPE stands for "Virtual Serial Ports Emulator" and is a software application that allows users to create virtual serial ports on their computer, essentially simulating a physical serial connection, which is useful for testing applications that need to interact with serial devices without requiring a real serial port available on the system

Download the VSPE software from [eterlogic.com](https://eterlogic.com/Products.VSPE_Download.html)

After installation, go to devices, and create a device. Select the device type 'Virtual Pair', after that select COM1 and COM2 pins. Make sure to check the 'Emulate baud rate (optional)' box.

After finishing, click on 'Start Emulation' run button.

### Arduino IDE

Download the Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software). Run the software and select 'Arduino UNO' board and then 'COM2 Serial Port'. After that, download and open the file 'arduino_code.ino', then compile it. 

After the compilation, an address for a hex file will be generated, copy that address. The address will look like this:-
```
"C:\\Users\\[username]\\AppData\\Local\\arduino\\sketches\\2C49F5891C1ADF93E77D33A4EFB51FFB/arduino_code.ino.hex"
```

### Proteus

Download [Proteus 8.13](https://www.labcenter.com/whatsnew/8.13/). Then download the file 'Traffic_Contol_system.pdsprj', and then open it via Proteus. 

Then double click on the Arduino Uno component. Go to the program file bar, and paste the address of the hex file copied earlier. Then save the project.

## Simulation

First run the VSPE.

Then compiling the arduino code in Arduino IDE, and copy its generated hex file path and paste it in Arduino Uno component in Proteus. 

Then run the Proteus simulation.

After that, run the python code in the Visual Studio Code by using the command:-

```
C:/Users/[username]/AppData/Local/Programs/Python/Python310/python.exe c:/Users/[username]/Documents/[foldername]/main.py
```

Now observe the traffic lights in Proteus

## Future Enhancements

🚀 Integration with IR Sensors for automatic real-time traffic detection.

📡 Wireless Communication using ESP8266/LoRa for IoT-based control.

📊 Web Dashboard for remote monitoring and control.

🔗 Contributions are welcome! Feel free to fork, improve, or suggest enhancements. 🚦
