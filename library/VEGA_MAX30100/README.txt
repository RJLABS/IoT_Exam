* Library Name : VEGA_MAX30100 by VEGA-Processor
 
This is a library for the MAX30100 pulse oximeter sensor SPECIFICALLY FOR USE WITH VEGA ARIES Boards

In this library we are using I2C protocol.

 * MAX30100 Datasheet: https://www.analog.com/media/en/technical-documentation/data-sheets/max30100.pdf 
 * ARIES Pinmap: https://vegaprocessors.in/files/PINOUT_ARIES%20V2.0_.pdf

Check out the above links for mlx datasheet and ARIES Pinmap. 
 
 * Connections:
 * MAX30100     Aries Board
 * VIN      -   3.3V
 * GND      -   GND
 * SDA      -   SDA-0
 * SCL      -   SCL-0
 * You can use any SDA,SCL pins 
 * For connecting to port 1 (SCL 1 and SDA1) of aries board use the default variable TwoWire Wire(1) instead of TwoWire Wire(0);
 * Use 10K pull-up resistors while connecting SDA and SCL pins to MAX30100 sensor for accurate measurements.

Other Useful links:

 * Official site: https://vegaprocessors.in/
 * YouTube channel: https://www.youtube.com/@VEGAProcessors

