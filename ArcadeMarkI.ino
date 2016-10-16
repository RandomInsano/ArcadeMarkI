#include "HID-Project.h"

#define B_ROW1 0
#define B_ROW2 1
#define B_HAT  2

// Lookup table for jokstick angles.
char angleMap[16] = {-1, -1, -1, 5, -1, -1, 7, 6, -1, 3, -1, 4, 1, 2, 0, -1};

void setup() {  
  Gamepad.begin();

  for (char pin = 4; pin < 8; pin++) {
    pinMode(pin, INPUT);           // set pin to input
    digitalWrite(pin, HIGH);       // turn on pullup resistors
  }

  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
}

void setRow(char row) {
  pinMode(A0, row == B_ROW1 ? OUTPUT : INPUT);
  pinMode(A1, row == B_ROW2 ? OUTPUT : INPUT);
  pinMode(A2, row == B_HAT  ? OUTPUT : INPUT);
}

void setHat() {
  /// Handle the joystick
  char hat = 0;
  setRow(B_HAT);

  for (char a = 0; a < 4; a++)
    hat = hat << 1 | digitalRead(4 + a);

  Gamepad.dPad1(angleMap[hat] + 1);
}

void setButtons() {
  /// Handle the action buttons
  for (char a = 0; a < 2; a++) {
    setRow(a);

    for (char b = 0; b < 4; b++) {
      if (digitalRead(4 + b) == 0)
        Gamepad.press(a * 4 + b + 1);
    }
  }
}

void loop() {
  Gamepad.releaseAll();

  // Set centers for things to ZSNES doesn't have a fit
  Gamepad.xAxis(0xFF / 2);
  Gamepad.yAxis(0xFF / 2);
  
  setButtons();
  setHat();

  Gamepad.write();
}
