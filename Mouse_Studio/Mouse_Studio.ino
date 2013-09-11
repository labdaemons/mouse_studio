int aviSoft = 4;
int sync = 6;
int sentDat = 0;

#include <Wire.h>
#include "Wire.h"
#include "BlinkM_funcs.h"
#include <"Time.h">
#include "Time.h"
#include <"TimeAlarms.h">
#include "TimeAlarms.h"

#define ON                        0x7
#define OFF                       0x0
#define blinkm_addr               0x09

#define MotorSpeedSet             0x82
#define PWMFrequenceSet           0x84
#define DirectionSet              0xaa
#define MotorSetA                 0xa1
#define MotorSetB                 0xa5
#define Nothing                   0x01
#define Stepernu                  0x1c
#define I2CMotorDriverAdd         0x0f

void setup()
{
  pinMode(aviSoft, OUTPUT);
  pinMode(sync, OUTPUT);
  
  digitalWrite(aviSoft, LOW);
  digitalWrite(sync, LOW);
  
  BlinkM_beginWithPower();
  BlinkM_off(0x09);
  Alarm.delay(10);
  
   Serial.begin(9600);
}

void loop()
{
   switch (sentDat) {
                  case 'A':
                    BlinkM_off(0x09);
                    break;
                  case 'B':
                    BlinkM_fadeToRGB(0x09, 255, 0, 0);
                    break;
                  
                  case 'C':
                    BlinkM_fadeToRGB(0x09, 255, 0, 0);
                    delay(900000);
                    
                    digitalWrite(aviSoft, HIGH);
                    
                    delay(5000);
                    
                    digitalWrite(sync, HIGH);
                    delay(100);
                    digitalWrite(sync, LOW);
                    
                    startPump();
                    delay(2500);
                    stopPump();
                    
                    delay(300000);
                    
                    digitalWrite(aviSoft, LOW);
                    BlinkM_off(0x09);
                    
                    delay(1000);
                    
                    digitalWrite(sync, HIGH);
                    delay(100);
                    digitalWrite(sync, LOW);
                    
                    break;
                    
                  case 'D':
                  triggerAvisoft();
                    
                  
                    
                    delay(310000);
                    
                    digitalWrite(aviSoft, LOW);
                    
                  break;
                  case 'E':
                  stopAvisoft();
                  break;
                  case 'F':
                  AVsync();
                  break;
                  case 'G':
                  startPump();
                  break;
                  case 'H':
                  stopPump();
                  break;
  }
  sentDat = 0;
}

void serialEvent() {
  sentDat = Serial.read();

}

  void MotorSpeedSetAB(unsigned char MotorSpeedA , unsigned char MotorSpeedB)  {
  MotorSpeedA=map(MotorSpeedA,0,100,0,255);
  MotorSpeedB=map(MotorSpeedB,0,100,0,255);
  Wire.beginTransmission(I2CMotorDriverAdd); // transmit to device I2CMotorDriverAdd
  Wire.write(MotorSpeedSet);        // set pwm header 
  Wire.write(MotorSpeedA);              // send pwma 
  Wire.write(MotorSpeedB);              // send pwmb    
  Wire.endTransmission();    // stop transmitting
}

void MotorDirectionSet(unsigned char Direction)  {     //  Adjust the direction of the motors 0b0000 I4 I3 I2 I1
  Wire.beginTransmission(I2CMotorDriverAdd); // transmit to device I2CMotorDriverAdd
  Wire.write(DirectionSet);        // Direction control header
  Wire.write(Direction);              // send direction control information
  Wire.write(Nothing);              // need to send this byte as the third byte(no meaning)  
  Wire.endTransmission();    // stop transmitting 
}

void startPump() {
      MotorSpeedSetAB(255,0);//defines the speed of motor 1 and motor 2;
      delay(10); //this delay needed
      MotorDirectionSet(0b0101);
      delay(10);
}

void stopPump() {
      MotorSpeedSetAB(0,0);
      delay(10); //this delay needed
      MotorDirectionSet(0b0101);
      delay(10);
}
  
void deliverUrine() {
      startPump();
      Alarm.timerOnce(2.5,stopPump);
      Alarm.delay(10);
}

void triggerAvisoft() {
      digitalWrite(aviSoft, HIGH);
      Alarm.delay(10);
}

void stopAvisoft() {
      digitalWrite(aviSoft, LOW);
      Alarm.delay(10);
}

void RecordingSession() {
      BlinkM_fadeToRGB(0x09, 255, 0, 0);
      Alarm.timerOnce(900, triggerAvisoft);
      Alarm.timerOnce(905, deliverUrine);
      Alarm.timerOnce(1200, stopAvisoft);
      Alarm.delay(10);
}

void AVsync() {
  digitalWrite(sync, HIGH);
  delay(100);
  digitalWrite(sync, LOW);
  delay(10);
}
