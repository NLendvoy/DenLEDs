# DenLEDs
STM32F100 Discovery board controlling 2 arrays of non-addressable RGB LED strips

Author: Nikolas Lendvoy
Date Created: May 2025

## How it's made:
**Tech used:** STM32F100 Discovery Board, Keil uVision5, STM32 ST-Link, Carrier Board

I created this project to work using my available parts, which was the cheapest option. It is not the best way to do it, but I had fun and didn't have to spend anything to set up lights for the den in my house.
I used a microcontroller from a past project and a carrier board. The carrier board's main purpose is to provide alternative power and a few push buttons.

My LED strips have a common input, meaning control for each colour is done with transistors between the LEDs and ground. If I had the option, I would have used an LED strip with a common ground instead of a common input because I could have varied voltage from the microcontroller ports.

## Circuit Diagram
![circuit](https://github.com/user-attachments/assets/886ac5dd-251f-49a0-9ec7-093cde8374ec)
This photo shows how the microcontroller outputs control the LED voltage. It does not show the carrier board buttons attached to the microcontroller.

## How to use:
1. Set up STM32 ST-Link
2. Compile and build the program in Keil uVision5
3. Connect to the STM32F100 Discovery board with ST-Link
4. Program board using ST-Link dashboard

At this point, the board is programmed and ready to use.
