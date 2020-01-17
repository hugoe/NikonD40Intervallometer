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
#define interval_started_mode 2
int mode = trigger_mode;


long intervals[12] = {1000, 2000, 4000, 8000, 15000, 30000, 45000, 60000, 120000, 240000, 480000, 840000};
int interval_index = 0;
#define intervals_min 0
#define intervals_max 11

void setup() {
  pinMode(IRledPin, OUTPUT);
  Wire.begin();
  delay(500); //Wait for communcations to establish
  lcd.lcdSetBlacklight(128);
  initDisplay();
}

void loop() {
  buttons = lcd.readButtons();

  if (~buttons & 0x01) // Mode
  {
    if (mode == interval_mode)
      mode = trigger_mode;
    else if (mode == trigger_mode)
      mode = interval_mode;
    updateDisplayMode();
  }

  if (~buttons & 0x02) // +
  {
    interval_index++;
    if(interval_index > intervals_max)
      interval_index = intervals_min;
    updateDisplayInterval();
  }

  if (~buttons & 0x04) // -
  {
    interval_index--;
    if(interval_index < intervals_min)
      interval_index = intervals_max;
    updateDisplayInterval();
  }

  if (~buttons & 0x08) // Start
  {
    if (mode == trigger_mode)
    {
      takePhoto(IRledPin);
    }
    else if (mode == interval_mode)
    {
      mode = interval_started_mode;
      lcd.lcdSetBlacklight(0);
    }
  }
  delay(100);
}

void initDisplay()
{
  lcd.lcdClear();
  lcd.lcdGoToXY(1, 1);
  lcd.lcdWrite(">Interval  1sec.");
  lcd.lcdGoToXY(1, 2);
  lcd.lcdWrite(" Trigger");
}

void updateDisplayMode()
{
  if (mode == interval_mode)
  {
    lcd.lcdGoToXY(1, 1);
    lcd.lcdWrite(">");
    lcd.lcdGoToXY(1, 2);
    lcd.lcdWrite(" ");
  }
  else if (mode == trigger_mode)
  {
    lcd.lcdGoToXY(1, 2);
    lcd.lcdWrite(">");
    lcd.lcdGoToXY(1, 1);
    lcd.lcdWrite(" ");
  }
}

void updateDisplayInterval()
{
  lcd.lcdGoToXY(11, 1);
  char* unit = "sec.";
  int secondsOrMinutes = intervals[interval_index] / 1000;
  if(secondsOrMinutes >=60)
  {
    secondsOrMinutes = secondsOrMinutes / 60;
    unit = "min.";
  }
  char buffer[2];
  sprintf(buffer, "%2d", secondsOrMinutes);
  //itoa(interval_seconds, buffer, 10);
  lcd.lcdWrite(buffer);
  lcd.lcdGoToXY(13, 1);
  lcd.lcdWrite(unit);
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
