#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x48);  // ADC object at I2C address 0x48 for addr pin = GND

int16_t adc0_1, adc0, adc1, adc2, unoA0;  // variables to hold ADC readings
float multiplier = 0.1875F;               // ADS1115  @ +/- 6.144V gain = 0.1875mV/step
float adcScale   = 4.8828F;               // Arduino 10 bit @ 5 volts   = 4.88mV/step

const float max_voltage = 4.800;  //Enter the shunt maximum voltage (volt)
const float v_ref = 1.220;  // voltage reference (volt)
const float gain = 50;  // gain of the current sense amplifier  INA240A1PW
const float resistance = 0.0025;  // resistance of the shunt resistor (ohm)

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int inputPin = A0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  ads.begin();  // init ADS1115 ADC
}

// the loop routine runs over and over again forever:
void loop() {
  
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  adc2 = ads.readADC_SingleEnded(2);        // read single AIN2
  readings[readIndex] = adc2;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  //Serial.println(average);
  //delay(1000);        // delay in between reads for stability
  
   
  float voltage = adc2 * multiplier/1000 ;
  
  //float voltage_vref = voltage - 1.2 + 0.0175;

 float voltage_vref = voltage - 1.2 ;
  float in = voltage_vref/50; 
  
  float current = in/0.0025*1.6; 
  
  // print out the value you read:
  
  Serial.println(current,3);
  
  delay(1000);
}
