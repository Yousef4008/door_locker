# door_locker
Door Locker System with ATmega32 Microcontrollers
Welcome to the Door Locker System project, where we harness the power of two ATmega32 microcontrollers to create a secure and interactive door locking mechanism. This system consists of two main components:

HMI_ECU (Human Machine Interface)
The HMI_ECU serves as the interface between the user and the system. It features a 2x16 LCD for displaying messages and a 4x4 keypad for user input. This unit is responsible for capturing user inputs, such as passwords, and providing feedback through the LCD.

Control_ECU
The Control_ECU takes charge of all the processing and decision-making within the system. It incorporates EEPROM for data storage, a buzzer for audible alerts, and a DC motor for controlling the door. This ECU handles tasks like password verification, door opening, and activation of the system alarm.

Getting Started
Clone the Repository:

git clone https://github.com/your-username/door-locker-system.git
cd door-locker-system
Hardware Setup:

Connect the components based on the provided circuit diagram.
Ensure proper connections between the MC1 (HMI_ECU) and MC2 (Control_ECU).
Compile and Upload:

Use your preferred AVR programming tool to compile and upload the code to each ATmega32 microcontroller.
Run the System:

Power up the system and interact with the HMI_ECU using the keypad.
Observe the Control_ECU making intelligent decisions based on user inputs.
