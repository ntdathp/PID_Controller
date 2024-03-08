---
FW: STM32 based on HAL Library
SW: QT6 with the plot based on QTCustomPLot
---
# Digital PID Controller 
Digital PID Controller for DC Motor (velocity and position) with incremental encoder.

I used the DC Motor GA37-520 for this project.

# About the algorithm
Using Trappizoidal Rules in order to convert the PID's transfer function from S domain to Z domain.

Adding anti-windup for I part and low-pass filter for D part.

# About the GUI
Plotting with QTCustomPlot

Auto detect COM Port and baudrate





