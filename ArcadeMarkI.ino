#include "Joystick.h"

#define B_ROW1 0
#define B_ROW2 1
#define B_HAT  2

// Lookup table for jokstick angles. This Joystick API is really annoying...
char angleMap[16] = {-1, -1, -1, 5, -1, -1, 7, 6, -1, 3, -1, 4, 1, 2, 8, -1};

void setup() {  
  //Serial.begin(115200);
  Joystick.begin(false);

  for (char pin = 4; pin < 8; pin++) {
    pinMode(pin, INPUT);           // set pin to input
    digitalWrite(pin, HIGH);       // turn on pullup resistors
  }

  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);

  Serial.println("Started");
  delay(1000);
}

void setRow(char row) {
  pinMode(A0, row == B_ROW1 ? OUTPUT : INPUT);
  pinMode(A1, row == B_ROW2 ? OUTPUT : INPUT);
  pinMode(A2, row == B_HAT  ? OUTPUT : INPUT);
}

void loop() {
  // Handle the action buttons
  for (char a = 0; a < 2; a++) {
    setRow(a);

    for (char b = 0; b < 4; b++) {
      Joystick.setButton(a * 4 + b, digitalRead(4 + b) ? 0 : 1);
      Serial.print(digitalRead(4 + b), HEX);
    }
    Serial.print(a, HEX);
    Serial.print(" ");
  }

  // Handle the joy stick
  char hat = 0;
  setRow(B_HAT);

  for (char a = 0; a < 4; a++)
    hat = hat << 1 | digitalRead(4 + a);

  hat = angleMap[hat];
  if (hat > 0)
    Joystick.setHatSwitch(0, hat * 45);
  else
    Joystick.setHatSwitch(0, -1);
  
  Serial.print(hat * 45, DEC);

  // Send the data to the computer
  Serial.println();
  Joystick.sendState();
}
