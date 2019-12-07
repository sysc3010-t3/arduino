# RC Camera Car Arduino
This is the repository for the Arduino of the RC Camera Car system. It contains
the C++ code that runs on the Arduino to handle messages sent from the connected
Raspberry Pi to interface with the connected hardware, such as the motors and
LEDs.

## Setup

### Dependencies:
* Arduino Software IDE

### Steps:
1. Install the Arduino Software IDE.  
    a) Download the installer for your operating system
    [here](https://www.arduino.cc/en/main/software).  
2. Run the Arduino Software IDE and open `hardwareControl/hardwareControl.ino`
within the application.  
3. Compile and load the code on the Arduino.  
    a) Connect the Arduino to the computer running the application via the USB
    cable provided in the kit.  
    b) Press the "upload" button (to the right of the checkmark button) in the
    top left corner to compile the code and upload it to the Arduino.  

## Code Structure

* [hardwareControl/](./hardwareControl)
    * [hardwareControl.ino](./hardwareControl/hardwareControl.ino): program that
    was written with Arduino Software to control the Arduino Board, also know as
    a "sketch"
    * [headlights.h](./hardwareControl/headlights.h): contains C++ code for the
      `Headlights` class declaration and the associated methods
    * [headlights.cpp](./hardwareControl/headlights.cpp): contains C++ code for
      the definitions of the `Headlights` class and methods
    * [ldr.h](./hardwareControl/ldr.h): contains C++ code for the `LDR` class
      declaration and the associated methods
    * [ldr.cpp](./hardwareControl/ldr.cpp): contains C++ code for the
      definitions of the `LDR` class and methods
    * [motor.h](./hardwareControl/motor.h): contains C++ code for the `Motor`
      class declaration and the associated methods
    * [motor.cpp](./hardwareControl/motor.cpp): contains C++ code for the
      definitions of the `Motor` class and methods
    * [motorTest.h](./hardwareControl/motorTest.h): contains C++ code for the
      `MotorTest` class declaration and the associated methods
    * [motorTest.cpp](./hardwareControl/motorTest.cpp): contains C++ code for
      the definitions of the `MotorTest` class and methods
