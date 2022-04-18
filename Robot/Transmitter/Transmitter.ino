#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

float leftwheelvalue=0;
float rightwheelvalue=0;
float throttlevalue=0;
float steeringvalue=0;
float midvalue_throttle=0;
float midvalue_steering=0;

RF24 radio(7, 8);
struct MyData {
  float left;
  float right;  
};
MyData data;

void resetData() 
{
  data.left = 0;
  data.right = 0;  
}

const byte rxAddr[6] = "00001";

void setup() {
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  radio.stopListening();
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  midvalue_steering = analogRead(A1);
  midvalue_throttle = map(analogRead(A0),0,1023,0,511);
}

void loop() {
  throttlevalue = map(analogRead(A0),0,1023,0,511)-midvalue_throttle;
  steeringvalue=analogRead(A1);
  
  if(steeringvalue==midvalue_steering){
    leftwheelvalue=throttlevalue;
    rightwheelvalue=throttlevalue;
  }
  else if(steeringvalue<midvalue_steering){
    rightwheelvalue=throttlevalue;
    leftwheelvalue=(steeringvalue/midvalue_steering)*throttlevalue;
  }
  else if(steeringvalue>midvalue_steering){
    leftwheelvalue=throttlevalue;
    rightwheelvalue=(abs(1023- steeringvalue)/(1023-midvalue_steering))*throttlevalue;
  }

  data.left = leftwheelvalue;
  data.right = rightwheelvalue;
  radio.write(&data, sizeof(MyData));
  delay(5);
}
