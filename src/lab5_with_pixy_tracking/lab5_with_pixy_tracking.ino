
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
        if (i%10==0) {
          sprintf(buf, "Detected %d:\n", blocks);
          print_blocks(blocks, buf);
          check_location(pixy.blocks[0].y); //Assuming there is only one block now.
          
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
  Serial.println("-------------");
  Serial.println("Check_Location");
  Serial.print("Blocks: ");
  Serial.println(blocks);
  int lowest_block = 500;
  for (int j=0; j<blocks; j++) {
    if (pixy.blocks[j].y < lowest_block) {
      lowest_block = pixy.blocks[j].y; 
      
    }
    Serial.print("pixy.blocks[j].y: ");
    Serial.println(pixy.blocks[j].y);
  }
  Serial.print("Lowest Block ");
  Serial.println(lowest_block);
  
  if (lowest_block - previous_y_block_location > 50) { // checking if gap is large enough
    num_pieces_tape++;
    previous_y_block_location = lowest_block;
    Serial.print("Number of Pieces of Tape: ");
    Serial.println(num_pieces_tape);
  }
  Serial.println("-------------");
}

void print_blocks(uint16_t blocks, char buf[32]) {
    
    Serial.print(buf);
    for (int j=0; j<blocks; j++)
    {
      sprintf(buf, "  block %d: ", j);
      Serial.print(buf); 
      pixy.blocks[j].print();
    }
  
}


void map()
{}

