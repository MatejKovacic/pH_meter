# pH Meter
Arduino based BlueSensor compatible pH meter

pH is a measure of acidity or alkalinity of a solution. pH scale ranges from 0 (acid) to 14 (alkaline). The pH indicates the concentration of hydrogen [H]+ ions present in certain solutions.
 
## Materials needed
- Arduino Nano
- cheap pH meter probe with circuit (we used PH-4502C, which is unfortunately of a very low quality, but there are similar probes on the market)

Compatible probes have the following pinout:
- To (temperature)
- Do (limit ("digital") pH signal)
- Po (analog pH value)
- G (nalog GND)
- G (supply GND)
- V+ (5V power supply)

(Circuit uses one ground for a circuit and one separated ground for a probe.)

## Wiring

pH meter - Arduino Nano
- V+ --- 5V
- G  --- GND
- G  --- GND
- Po --- A0
- Do --- (not connected)
- To --- (not connected)

## Calibration
Please note that you need to calibrate pH sensor. In order to do this, you need two pH reference values and measure the voltage returned by the sensor on the pin Po. The best thing to do is to use a calibration solution in powders. Use two powders with known pH and obtain the respective voltages. Then compute the formula (k and n): pHValue = k * phVoltage + n

HINT: you can use 'pH_meter.ods' for easier calculation.
