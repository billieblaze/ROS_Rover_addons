void setup(){
  pinMode(13, OUTPUT);
  pinMode(A3, INPUT_PULLUP);
}

void loop(){
  while (!digitalRead(A3)){
    digitalWrite(13,HIGH);
  }
}
