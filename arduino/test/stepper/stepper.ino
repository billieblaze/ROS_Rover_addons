int endstopPin = A3;
int directionPin = A4;
int stepPin = A5;

void setup() {
  pinMode(A4, OUTPUT);
  pinMode(stepPin, OUTPUT);
  digitalWrite(A4, HIGH);
  digitalWrite(stepPin, LOW);
}

void loop() {
  digitalWrite(stepPin, HIGH);
  delay(1);
  digitalWrite(stepPin, LOW);
  delay(1);
}
