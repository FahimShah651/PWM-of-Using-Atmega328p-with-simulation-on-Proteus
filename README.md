# PWM-of-Using-Atmega328p-with-simulation-on-Proteus
This repository contains code for generating Pulse Width Modulation (PWM) signals using an Atmega328p microcontroller, with a simulation set up on Proteus also tested on the Hardware ic atmega328p.
The code initializes PWM on the Atmega328p microcontroller to control the duty cycle of a waveform. The PWM signals are used to modulate outputs connected to specific pins (OC1A and OC1B). The duty cycle is dynamically adjusted based on external interrupts (INT0 and INT1), which simulate speed control for a motor or other device. The setup includes:

# PWM Initialization:
Setting PWM pins as outputs and configuring Fast PWM mode with ICR1 as the TOP value.
# External Interrupts:
Configuring INT0 and INT1 to trigger on falling edges, allowing real-time adjustments to the PWM duty cycle.
# Duty Cycle Adjustment:
Using the interrupt service routines (ISR) to increase or decrease the duty cycle, affecting the speed of the connected device.
The main loop continuously updates the PWM duty cycle, with a delay to observe changes.
