
#include <SPI.h>  
#include <Pixy.h>


Pixy pixy;
//Defining pins
const int left_wheel = 11;//10
const int left_wheel_b = 10;
const int right_wheel = 6;//5
const int right_wheel_b = 5;
const int switch_pin=3;
const int PING_pin_left=7;
const int  PING_pin_right=4;

//Other constants
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

enum States {S, R};
States state=R;

/*
    Initializes pins and serial port. 
*/
void setup()
{
  Serial.begin(9600);
  pinMode(left_wheel, OUTPUT);   // sets the pin as output
  pinMode(right_wheel, OUTPUT);
  pinMode(switch_pin, INPUT_PULLUP);
  switch_state=digitalRead(switch_pin);
  pixy.init();
  analogWrite(left_wheel, 95);
  digitalWrite(left_wheel_b, LOW);
  
  analogWrite(right_wheel, 100);
  digitalWrite(right_wheel_b, LOW);
  Serial.println("End of setup");
}

/*
    Two States Stopped and running.
    The running state just gets block from pixy and sends them to the get location function.
*/
void loop()
{
  static int i = 0;
  int j;
  //uint16_t blocks;
  int blocks;
  char buf[32]; 
  
  //delay(500);
  if (num_pieces_tape > 5) {
    stop_wheels();
    
  }
  switch(state)
  {
    case S:
      if(switch_state==digitalRead(switch_pin))
        delay(1000);
      else
        switch_state=R;
      break;
    case R:
      blocks = pixy.getBlocks();
      //Serial.print("Blocks: ");
      //Serial.println(blocks);
      if (blocks) {
        i++;
        if (i%1==0) {
          //sprintf(buf, "Detected %d:\n", blocks);
          //print_blocks(blocks, buf);
          check_location(blocks); //Assuming there is only one block now.
          
        } 
      }
  }
}

/*
    Checking location of robot using pixy image data.
    Uses red tape on floor to localize. 
*/
void check_location(uint16_t blocks) {
  int highest_block = 0;
  for (int j=0; j<blocks; j++) {
    if (pixy.blocks[j].y > highest_block) {
      highest_block = pixy.blocks[j].y;      
    }
  }
  
  if (abs(highest_block - previous_y_block_location) > 50) { // checking if gap is large enough
    num_pieces_tape++;
    Serial.println(" ");
    Serial.print("Number of Pieces of Tape: ");
    Serial.println(num_pieces_tape);
    Serial.print("Distance to left in cm: ");
    mapDist(PING_pin_left);
    Serial.print("Distance to right in cm: ");
    mapDist(PING_pin_right);
    Serial.print("Z Distance: ");
    Serial.println(num_pieces_tape*15.24);
    
  }
  previous_y_block_location = highest_block;
}

/*
    Gets a reading from the ping sensor. 
*/
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

/*
    Prints all blocks in the buffer.
*/
void print_blocks(uint16_t blocks, char buf[32]) {
    for (int j=0; j<blocks; j++)
    {
      sprintf(buf, "  block %d: ", j); 
      pixy.blocks[j].print();
    }
  
}


/*
    Once it has reached its desired distance stop the motors.
*/
void stop_wheels() {
  delay(100);
  digitalWrite(left_wheel, HIGH);
  digitalWrite(left_wheel_b, HIGH);
  delay(100);
  digitalWrite(right_wheel, LOW);
  digitalWrite(right_wheel_b, LOW);
  
}

