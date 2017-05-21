void setup(){
  pinMode(13, OUTPUT);
  pinMode(A3, INPUT);
}

void loop(){
  while (digitalRead(A3)){
    digitalWrite(13,HIGH);
  }
}
