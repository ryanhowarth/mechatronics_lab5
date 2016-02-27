 int analogPin = A0;     // potentiometer wiper (middle terminal) connected to analog pin 3

int aref = 22;                   // outside leads to ground and +5V

int val = 0;           // variable to store the value read

//float volts_per_tick = .0049;

void setup()

{

  Serial.begin(9600);          //  setup serial
  //analogReference(EXTERNAL);
  //pinMode(aref, OUTPUT);
  //digitalWrite(aref, HIGH);
  analogReference(DEFAULT);
}



void loop()

{
  //byte *p = (byte*) &val;
  //Serial.println(val);
  //val = analogRead(analogPin);    // read the input pin
  val = 250;
  Serial.write(val/256);
  Serial.write(val%256);
  Serial.write(255);
  //Serial.println(*p);
  //Serial.println(' ');16  
  
  
  
  //String val2 = (String)val;
  //delay(1000);
  //val2 = val2 + '\n';
  //Serial.print(val2);  // debug value
  //Serial.println("V");

}
