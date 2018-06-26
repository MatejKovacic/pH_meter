/*
  Application to read data from pH meter electrode.
 
  pH is a measure of acidity or alkalinity of a solution.
  pH scale ranges from 0 (acid) to 14 (alkaline).
  The pH indicates the concentration of hydrogen [H]+ ions present in certain solutions.
 
  Wiring:

  [pH meter --- Arduino Nano]
  =========================
   V+ --- 5V
   G  --- GND
   G  --- GND
   Po --- A0
   Do --- (not connected)
   To --- (not connected)

  Please note that you need to calibrate pH sensor.
  In order to do this, you need two pH reference values and measure the voltage 
  returned by the sensor on the pin Po.
  The best thing to do is to use a calibration solution in powders. Use two powders
  with known pH and obtain the respective voltages. Then compute the formula (k and n):

    pHValue = k * phVoltage + n

  HINT: you can use 'pH_meter.ods' for easier calculation.
  
  Based on: http://scidle.com/how-to-use-a-ph-sensor-with-arduino/
*/

#include <ArduinoJson.h>  // We are using JSON output to BlueSensor device.

const int analogInPin = A0;  // Po is connected to A0 on Arduino.

int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;

void setup() {
  Serial.begin(9600);
}
 
void loop() {
  /* We are taking 10 samples of the analogue Arduno's input,
     ordering them, discarding the highest and the lowest, 
     and calculating the mean with the six remaining samples.
  */
  for(int i=0;i<10;i++) 
  { 
    buf[i]=analogRead(analogInPin);
    delay(10);
  }

  for(int i=0;i<9;i++)
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }

  avgValue=0;

  for(int i=2;i<8;i++)
  avgValue+=buf[i];
  float pHVoltage=(float)avgValue*5.0/1024/6;  // This is the average measured voltage.

  // Now we compute pHValue from the formula:
  float pHValue = -5.38 * pHVoltage + 28.32;


  // Print results in JSON (https://arduinojson.org/assistant/):
  const size_t bufferSize = 2*JSON_ARRAY_SIZE(5) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4);
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.createObject();

  JsonObject& metadata = root.createNestedObject("metadata");
  metadata["device_name"] = "pH meter";
  metadata["device_id"] = "pH";
  metadata["device_location"] = "IJS";

  JsonObject& metadata_sensors = metadata.createNestedObject("sensors");

  JsonArray& metadata_sensors_pH = metadata_sensors.createNestedArray("pH");
  metadata_sensors_pH.add("pH");
  metadata_sensors_pH.add("PH-4502C");
  metadata_sensors_pH.add("pH");
  metadata_sensors_pH.add("green");
  metadata_sensors_pH.add("ready");

  JsonArray& metadata_sensors_voltage = metadata_sensors.createNestedArray("voltage");
  metadata_sensors_voltage.add("Voltage");
  metadata_sensors_voltage.add("PH-4502C");
  metadata_sensors_voltage.add("V");
  metadata_sensors_voltage.add("red");
  metadata_sensors_voltage.add("ready");
  root["time"] = 0;

  JsonObject& data = root.createNestedObject("data");
  data["pH"] = pHValue;
  data["voltage"] = pHVoltage;

  root.printTo(Serial);
  Serial.println();

  /*
  // Print results on serial console (useful for calibration):
  Serial.print("Sensor voltage = ");
  Serial.print(pHVoltage);
  Serial.print("V, pH value = ");
  Serial.println(pHValue);
  */
  
  // Wait for 1 second...
  delay(1000);
}
