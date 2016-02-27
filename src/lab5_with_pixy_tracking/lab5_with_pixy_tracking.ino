/*
const int outPin12 = 12;
const int outPin11 = 11;
const int outPin10 = 10;
const int outPin9 = 9;

void setup() {
  // Configures outPin1 to be an output pin.
  pinMode(outPin12, OUTPUT);
  pinMode(outPin11, OUTPUT);
  pinMode(outPin10, OUTPUT);
  pinMode(outPin10, OUTPUT);

  digitalWrite(outPin12, HIGH);
  digitalWrite(outPin11, LOW);

  digitalWrite(outPin10, HIGH);
  digitalWrite(outPin9, LOW);  
}

void loop() {
  // Signal is turned high and low alternately
  // and immediately.
//  digitalWrite(outPin12, HIGH);
//  digitalWrite(outPin11, LOW);
//  delay(1000);
//  digitalWrite(outPin5, LOW);
//  digitalWrite(outPin3, LOW);
//  delay(1000);
//  digitalWrite(outPin10, HIGH);
//  digitalWrite(outPin9, LOW);
//  delay(1000);
//  digitalWrite(outPin5, LOW);
//  digitalWrite(outPin3, LOW);
//  delay(1000);
}
*/
#include <SPI.h>  
#include <Pixy.h>

Pixy pixy;
const int left_wheel = 11;//10
const int left_wheel_b = 10;
const int right_wheel = 6;//5
const int right_wheel_b = 5;
const int switch_pin=3;
const int max_v=100;
int left_v=0;
int right_v=0;
unsigned long time;
const unsigned long time_thresh=100;
bool switch_state;
bool count_time=false;

enum States {S, A, R, D};
States state=S;

void setup()
{
  Serial.begin(38400);
  pinMode(left_wheel, OUTPUT);   // sets the pin as output
  pinMode(right_wheel, OUTPUT);
  pinMode(switch_pin, INPUT_PULLUP);
  switch_state=digitalRead(switch_pin);
  pixy.init();
  analogWrite(left_wheel, 0);
  analogWrite(left_wheel_b, 0);
  analogWrite(right_wheel, 0);
  analogWrite(right_wheel_b, 0);
}

void loop()
{
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32]; 
  
  switch(state)
  {
    S:
      if(switch_state==digitalRead(switch_pin))
        delay(1000);
      else
        switch_state=A;
      break;
    A:
      map();
      switch_state=!switch_state;
      while(left_v<max_v)
      {
        analogWrite(left_wheel, left_v);  
        analogWrite(right_wheel, right_v);
        left_v+=25;
        right_v+=25;
        delay(200);
      }
      switch_state=R;
      break;
    R:
      blocks = pixy.getBlocks();
      i++;

      if (i%10==0)
      {
        if(blocks==0)
        {
          if(count_time==false)
          {
            time=millis();  
            count_time=true;
          }
          else
          {
            if((millis()-time)>time_thresh)
            {
              switch_state=D;
              count_time=false;
            }
          }
        }
        else
        {
          if(count_time)
            count_time=false;
          for (j=0; j<blocks; j++)
          {
            if(pixy.blocks[j].y<70)
            {//adjust direction
                if(pixy.blocks[j].x<150)
                {//turn right
                  left_v=50+max_v;
                  right_v=max_v;
                  analogWrite(left_wheel, left_v);  
                  analogWrite(right_wheel, right_v);
                  time=millis();
                  while((millis()-time)<10);
                  left_v=max_v;
                }
                else if(pixy.blocks[j].x>170) 
                {//turn left
                  left_v=max_v;
                  right_v=50+max_v;
                  analogWrite(left_wheel, left_v);  
                  analogWrite(right_wheel, right_v);
                  while((millis()-time)<10);
                  right_v=max_v;
                }
            }
                
          }
        }
          
      }
  
      break;
    D:
      if(left_v>right_v)
        left_v=right_v;
      else
        right_v=left_v;
      while(left_v>0)
      {
        analogWrite(left_wheel, left_v);  
        analogWrite(right_wheel, right_v);
        left_v-=25;
        right_v-=25;
        delay(200);
      }
      map();
      switch_state=S;
      break;
  }
  
  
  
/*  
  while(brightness)
  {
  analogWrite(outPin3, brightness);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  delay(50);
  brightness-=5;
  }
  delay(1000);
  brightness=255;
*/
}

void map()
{}

