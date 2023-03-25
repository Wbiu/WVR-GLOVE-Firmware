# WVR GLOVE

This the main repository of the **WVR GLOVE**.
Within the repository is the main firmware for the glove.

## Overview
The whole project does not use any of the existing VR Hardware such as the HTC´s or Meta´s VR Headsets.   


## Building and configuring the glove 
Instruction to build and configuring the glove see : [wiki]("")

## Testing ground in Unreal Engine 5
After building the Glove you can try the glove in the [UE5 Demo]() that I made.

## UWB tracking
Since no VR Hardware are used, a UWB based tracking was used instead. UWB tracking firmware see [here]().

## About the firmware
The firmware was coded in [Platformio](https://platformio.org/).<br/>
An esp32 wroom was used to build this glove.



> **Important note** : 
WVR Glove was my a graduation project that was made to work with
the Unreal Engine 5 Demo without any VR-Hardware. Which means that this glove **won´t** work in any VR Game.
**But** this could change in the future.
