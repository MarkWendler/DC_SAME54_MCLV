# MPLAB X Project: mc_dc_SAME54_MCLV

This is a motor control demo project for dsPICDEMTM MCLV development board and [SAME54](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/MA320207) microcontroller.

The demo drives a simple DC motor. No speed control, only voltage mode.

* DC motor drive with H bridge
  * Connect the DC motor to M1 and M2 phase connector on MCLV
* Direction change 
  * Button 1 starts one direction
   *Button 2 starts the other direction
* Measurements:
  * Current is stored in the meteringData.current global variable (Not synced with PWM, fix might needed)
  * BEMF is stored in the meteringData.voltage global variable
  * RS232-UART converter can be used with X2Cscope to read these data. [https://x2cscope.github.io/](https://x2cscope.github.io/)
* Only Voltage control (PWM) – 
  * Potentiometer controls the PWM duty cycle
* No current and speed control.

Current and Voltage values can be read out by X2CScope plugin that is integrated in MPLAB X. It is communicating via RS232 port on the board. You will need a USB-RS232 converter.

* **M1/M2/M3:** Motor  phases connected (Sequence is important with sensor mode, but not with sensorless mode)
  * M1 - PHASEA (DC Motor phase 1)
  * M2 - PHASEB (DC Motor phase 2)
  * M3 - NC
* **Supply** 24V Power supply
* 
* **RS232** Used X2Cscope to connect UART to computer. [https://x2cscope.github.io/](https://x2cscope.github.io/)

### Built With

* [XC32 v1.60](https://www.microchip.com/mplab/compilers) - Microchip C compiler for 16bit micros
* [Harmony 3](https://www.microchip.com/harmony) - Configure peripherals and generate low levev drivers
* MPLAB X IDE
