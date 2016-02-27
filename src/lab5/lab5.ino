const int outPin11 = 11;
const int outPin10 = 10;
const int outPin6 = 6;
const int outPin5 = 5;

void setup() {
  // Configures outPin1 to be an output pin.
  pinMode(outPin11, OUTPUT);
  pinMode(outPin10, OUTPUT);
  pinMode(outPin6, OUTPUT);
  pinMode(outPin5, OUTPUT);

  digitalWrite(outPin11, LOW);
  analogWrite(outPin10, 100);

  analogWrite(outPin5, 100);
  digitalWrite(outPin5, LOW);  
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
