#include <LCD16x2.h>
#include <Wire.h>
//#include <avr/sleep.h>

// Using Olimex Shield LCD16x2 with 4 buttons
// https://www.electrokit.com/produkt/lcd-knapp-shield-i2c-uart/
LCD16x2 lcd;
int buttons;
int IRledPin =  13;

#define interval_mode 0
#define trigger_mode 1
int mode = trigger_mode;
int interval_seconds = 1;
bool updateDisplay = true;

void setup() {
  pinMode(IRledPin, OUTPUT);
  Wire.begin();
  delay(500); //Wait for communcations to establish
  lcd.lcdSetBlacklight(128);
  displayState();
}

void loop() {
  buttons = lcd.readButtons();

  if (~buttons & 0x01) // Mode
  {
    if (mode == interval_mode)
      mode = trigger_mode;
    else if (mode == trigger_mode)
      mode = interval_mode;
    updateDisplay = true;
  }

  if (~buttons & 0x02) //+
  {
    interval_seconds++;
    updateDisplay = true;
  }

  if (~buttons & 0x04) //-
  {
    if(interval_seconds >= 2)
    {
      interval_seconds--;
      updateDisplay = true;
    }
  }

  if (~buttons & 0x08) // Start
  {
    if (mode == trigger_mode)
      takePhoto(IRledPin);
  }

  displayState();
  delay(100);
}

void displayState()
{
  if (!updateDisplay) {
    return;
  }

  lcd.lcdClear();
  lcd.lcdGoToXY(1, 1);
  lcd.lcdWrite("Nikon D40 Hugo E");
  lcd.lcdGoToXY(1, 2);
  if (mode == interval_mode) {
    lcd.lcdWrite("Interval ");
    lcd.lcdGoToXY(10, 2);
    char buffer[2];
    itoa(interval_seconds, buffer, 10);
    lcd.lcdWrite(buffer);
    lcd.lcdGoToXY(13, 2);
    lcd.lcdWrite("sec.");
  }
  else if (mode == trigger_mode)
    lcd.lcdWrite("Trigger");
  updateDisplay = false;
}

// Nikon D40 sequence
// source: https://www.christidis.info/index.php/personal-projects/arduino-nikon-infrared-command-code)
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
