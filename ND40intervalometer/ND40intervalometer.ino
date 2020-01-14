#include <avr/sleep.h>
int IRledPin =  13;
int buttonPin = 2;
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  pinMode(IRledPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    LedSequence(LED_BUILTIN);
    takePhoto(IRledPin);
  }
  //delay(30000);
}

void LedSequence(int led) {
  int blinks = 3;

  while (blinks > 0) {
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(200);
    blinks--;
  }
}

//Nikon D40 sequence
void takePhoto(int led) {
  int i;
  for (i = 0; i < 76; i++) {
    digitalWrite(led, HIGH);
    delayMicroseconds(7);
    digitalWrite(led, LOW);
    delayMicroseconds(7);
  }
  delay(27);
  delayMicroseconds(810);
  for (i = 0; i < 16; i++) {
    digitalWrite(led, HIGH);
    delayMicroseconds(7);
    digitalWrite(led, LOW);
    delayMicroseconds(7);
  }
  delayMicroseconds(1540);
  for (i = 0; i < 16; i++) {
    digitalWrite(led, HIGH);
    delayMicroseconds(7);
    digitalWrite(led, LOW);
    delayMicroseconds(7);
  }
  delayMicroseconds(3545);
  for (i = 0; i < 16; i++) {
    digitalWrite(led, HIGH);
    delayMicroseconds(7);
    digitalWrite(led, LOW);
    delayMicroseconds(7);
  }
}
