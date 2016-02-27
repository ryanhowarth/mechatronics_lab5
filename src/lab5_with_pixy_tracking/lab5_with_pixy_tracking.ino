
#include <SPI.h>  
#include <Pixy.h>

Pixy pixy;
const int left_wheel = 11;//10
const int left_wheel_b = 10;
const int right_wheel = 6;//5
const int right_wheel_b = 5;
const int switch_pin=3;
const int max_v=100;
const int left_wheel_v = 100;
const int right_wheel_v = 125;
const int PING_pin_left=7;
const int  PING_pin_right=4;

int previous_y_block_location = 500; //large number so it will count first frame;
int num_pieces_tape = 0;

int left_v=0;
int right_v=0;
unsigned long time;
const unsigned long time_thresh=100;
bool switch_state;
bool count_time=false;

enum States {S, A, R, D};
States state=R;

void setup()
{
  Serial.begin(9600);
  pinMode(left_wheel, OUTPUT);   // sets the pin as output
  pinMode(right_wheel, OUTPUT);
  pinMode(switch_pin, INPUT_PULLUP);
  switch_state=digitalRead(switch_pin);
  pixy.init();
  analogWrite(left_wheel, 100);
  digitalWrite(left_wheel_b, LOW);
  
  analogWrite(right_wheel, 125);
  digitalWrite(right_wheel_b, LOW);
  Serial.println("End of setup");
}

void loop()
{
  static int i = 0;
  int j;
  //uint16_t blocks;
  int blocks;
  char buf[32]; 
  
  //delay(500);
  switch(state)
  {
    case S:
      if(switch_state==digitalRead(switch_pin))
        delay(1000);
      else
        switch_state=R;
      break;
    case A:
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
    case R:
      blocks = pixy.getBlocks();
      //Serial.print("Blocks: ");
      //Serial.println(blocks);
      if (blocks) {
        i++;
        if (i%5==0) {
          //sprintf(buf, "Detected %d:\n", blocks);
          //print_blocks(blocks, buf);
          check_location(blocks); //Assuming there is only one block now.
          
        }
        
      
      }
      /*
      if (i%50==0)
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
      */
      
    case D:
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

void check_location(uint16_t blocks) {
  int highest_block = 0;
  for (int j=0; j<blocks; j++) {
    if (pixy.blocks[j].y > highest_block) {
      highest_block = pixy.blocks[j].y;      
    }
  }
  
  if (abs(highest_block - previous_y_block_location) > 50) { // checking if gap is large enough
    num_pieces_tape++;
    //previous_y_block_location = highest_block;
    Serial.print("Number of Pieces of Tape: ");
    Serial.println(num_pieces_tape);
    Serial.print("Distance to left in cm: ");
    mapDist(PING_pin_left);
    Serial.print("Distance to right in cm: ");
    mapDist(PING_pin_right);
    
  }
  previous_y_block_location = highest_block;
}

void mapDist(int PING_pin_left){
  // left
  pinMode(PING_pin_left, OUTPUT);
  digitalWrite(PING_pin_left, LOW);
  delayMicroseconds(2);
  digitalWrite(PING_pin_left, HIGH);
  delayMicroseconds(5);
  digitalWrite(PING_pin_left,LOW);
  pinMode(PING_pin_left, INPUT);
  while (digitalRead(PING_pin_left) == LOW) {
  }
  unsigned long tstart=micros();
  unsigned long tnow=micros();
  while (digitalRead(PING_pin_left) == HIGH) {
    tnow=micros();
    if (tnow-tstart > 19000) {
      break;
    }
  }
  unsigned long tend = micros();
  unsigned long tdiff = tend-tstart;
  float dist = .034*tdiff/2;
  //Serial.print("Distance to left in cm: ");
  Serial.println(dist);
}


void mapDist(int PING_pin_left,int PING_pin_right){
  // left
  pinMode(PING_pin_left, OUTPUT);
  digitalWrite(PING_pin_left, LOW);
  delayMicroseconds(2);
  digitalWrite(PING_pin_left, HIGH);
  delayMicroseconds(5);
  digitalWrite(PING_pin_left,LOW);
  pinMode(PING_pin_left, INPUT);
  while (digitalRead(PING_pin_left) == LOW) {
  }
  unsigned long tstart=micros();
  unsigned long tnow=micros();
  while (digitalRead(PING_pin_left) == HIGH) {
    tnow=micros();
    if (tnow-tstart > 19000) {
      break;
    }
  }
  unsigned long tend = micros();
  unsigned long tdiff = tend-tstart;
  float dist = .034*tdiff/2;
  Serial.print("Distance to left in cm: ");
  Serial.println(dist);
  
  
  // right
  pinMode(PING_pin_right, OUTPUT);
  digitalWrite(PING_pin_right, LOW);
  delayMicroseconds(2);
  digitalWrite(PING_pin_right, HIGH);
  delayMicroseconds(5);
  digitalWrite(PING_pin_right,LOW);
  pinMode(PING_pin_right, INPUT);
  while (digitalRead(PING_pin_right) == LOW) {
  }
  tstart=micros();
  while (digitalRead(PING_pin_right) == HIGH) {
    tnow=micros();
    if (tnow-tstart > 19000) {
      break;
    }
  }
  tend = micros();
  tdiff = tend-tstart;
  dist = .034*tdiff/2;
  Serial.print("Distance to right in cm: ");
  Serial.println(dist);
  Serial.println("----------");
}



void print_blocks(uint16_t blocks, char buf[32]) {
    
    //Serial.print(buf);

    for (int j=0; j<blocks; j++)
    {
      sprintf(buf, "  block %d: ", j);
      //Serial.print(buf); 
      pixy.blocks[j].print();
    }
  
}


void map()
{}

