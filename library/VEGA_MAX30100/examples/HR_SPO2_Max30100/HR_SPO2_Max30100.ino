/**
 @file HR_SPO2_Max30100.ino
 @brief Checks the Heart Rate and SpO2
 @detail Checks the Heart Rate and SpO2 using Max30100 sensor

 * Reference arduino code: https://www.youtube.com/watch?v=8SOTsR1k8-g&t=312s, https://how2electronics.com/interfacing-max30100-pulse-oximeter-sensor-arduino/
 * Refrence aries board: https://vegaprocessors.in/blog/interfacing-max30102-pulse-oximeter-to-thejas-soc/
 * 
 * MAX30100 sensor
 * Device address -0x57
 * Connections:
 * MAX30100     Aries Board
 * 3V3      -   3.3V
 * GND      -   GND
 * SDA      -   SDA0
 * SCL      -   SCL0
 * For connecting to port 1 (SCL 1 and SDA1) of aries board use the default variable TwoWire Wire(1) instead of TwoWire Wire(0);
 * .
 * Note:
 * Use 10K pull-up resistors while connecting SDA and SCL pins to MAX30100 sensor for accurate measurements.
 **/

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

TwoWire Wire(0);


#define REPORTING_PERIOD_MS     1000
 
PulseOximeter pox;
uint32_t tsLastReport = 0;
 
void onBeatDetected()
{
Serial.println("Beat!");
}
 
void setup()
{
Serial.begin(115200);
Serial.print("Initializing pulse oximeter..");
delay(3000);
 
// Initialize the PulseOximeter instance
// Failures are generally due to an improper I2C wiring, missing power supply
// or wrong target chip
if (!pox.begin()) {
Serial.println("FAILED");
for(;;);
} else {
Serial.println("SUCCESS");
}
pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
// Register a callback for the beat detection
pox.setOnBeatDetectedCallback(onBeatDetected);
}
 
void loop()
{
// Make sure to call update as fast as possible
pox.update();
if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
Serial.print("Heart rate:");
Serial.print(pox.getHeartRate());       //Display of Heart rate
Serial.print("bpm / SpO2:");
Serial.print(pox.getSpO2());            //Display of SPO2
Serial.println("%");

tsLastReport = millis();
}
}
