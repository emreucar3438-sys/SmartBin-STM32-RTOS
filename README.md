# SmartBin-STM32-RTOS
n automated, real-time waste management system based on STM32, utilizing FreeRTOS, interrupt-driven task notification, and watchdog timer for robust performance.
Project Overview
The system uses an ultrasonic sensor to detect proximity and a push button for manual override. To maximize efficiency and responsiveness, the project avoids polling and instead utilizes Interrupts and Task Notifications. A Watchdog Timer is also integrated to ensure system stability and fault detection.
#How It Works
Autonomous Mode: When the ultrasonic sensor detects an object within 10 cm, the servo motor opens the lid automatically.

Manual Mode: Pressing the button triggers a hardware Interrupt, allowing immediate manual control regardless of sensor data.

Fault Monitoring: If the system hangs or encounters a failure, the Watchdog Timer triggers the error LED.

This project serves as a practical implementation of RTOS architectures in embedded systems.
