#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
float left_wheel = 0;
float right_wheel =0;

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

void setup()
{
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  radio.startListening();
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  analogWrite(5,0);
  analogWrite(6,0);
  analogWrite(9,0);
  analogWrite(10,0);
}

void loop()
{
  if (radio.available())
  {
    radio.read(&data, sizeof(MyData));
    left_wheel = data.left;
    right_wheel = data.right;
    
    if (right_wheel<-255){
      right_wheel = -255;
    }
    
    if (left_wheel<-255){
      left_wheel = -255;
    }

    if(left_wheel <0){
      analogWrite(5,abs(left_wheel));
      analogWrite(9,0);
    }
    else{
      analogWrite(9,left_wheel);
      analogWrite(5,0);
    }

    if(right_wheel<0){
      analogWrite(6,abs(right_wheel));
      analogWrite(10,0);
    }
    else{
      analogWrite(10,right_wheel);
      analogWrite(6,0);
    }
  }
}
