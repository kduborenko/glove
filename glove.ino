#include <SoftwareSerial.h>

#define KEY_STATE_NONE 0
#define KEY_STATE_JUST_PRESSED 1
#define KEY_STATE_PRESSED 2

#define KEYS_COUNT 9

SoftwareSerial bluetooth(2, 3);

byte keyPins[KEYS_COUNT] = {
  A0, 11, 10, 9, 8, A1, A2, A3, A4
};
byte keyStates[KEYS_COUNT];
byte keyCodes[KEYS_COUNT] = {
  13, 0, 0, 0, 0, 7, 11, 27, 0
};

void setup() {
  for (int i = 0; i < KEYS_COUNT; i++) {
    pinMode(keyPins[i], INPUT);
  }
  Serial.begin(9600);
  bluetooth.begin(115200);
  bluetooth.print("$$$");
  delay(100);
  bluetooth.println("U,9600,N");
  bluetooth.begin(9600);
  
  bluetooth.print("$$$");
  delay(100);
  bluetooth.println("S~,6");     // Enable HID
  delay(100);
  bluetooth.println("SH,0000");  // HID mode keyboard
  delay(100);
  bluetooth.println("SM,6");     // Pairing mode (auto-reconnect)
  delay(100);
  bluetooth.println("S-,Glove"); // Device name
  delay(100);
  bluetooth.println("---");
}

void loop() {
  for (int i = 0; i < KEYS_COUNT; i++) {
    if (digitalRead(keyPins[i]) == 1) {
      if (keyStates[i] == KEY_STATE_NONE) {
        keyStates[i] = KEY_STATE_JUST_PRESSED;
      } else if (keyStates[i] == KEY_STATE_JUST_PRESSED) {
        onKeyPressed(i);
        keyStates[i] = KEY_STATE_PRESSED;
      }
    } else {
      keyStates[i] = KEY_STATE_NONE;
    }
  }
  delay(50);
}

void onKeyPressed(byte keyIndex) {
  if (keyIndex == 0) {
    mediaKey(0x10);
  } else if (keyIndex == 4) {
    mediaKey(0x20);
  } else if (keyCodes[keyIndex] != 0) {
    bluetooth.write(keyCodes[keyIndex]);
    Serial.println(keyPins[keyIndex]);
  }
}

void mediaKey(byte code) {
  byte keySequence[] = {0xFD, 0x03, 0x03, code, 0x00};
  bluetooth.write(keySequence, 5);
  delay(10);
  keySequence[3] = 0;
  bluetooth.write(keySequence, 5);
}
