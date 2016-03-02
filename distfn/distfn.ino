const int PING_pin_left=7;
const int  PING_pin_right=4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  mapDist(PING_pin_right, PING_pin_left);
  delay(1000);
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
