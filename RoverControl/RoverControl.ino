
/*
    No.1 pin HIGH and No.2 pin LOW of the mottors is supposed to go FORWARD
    No.2 pin HIGH and No.1 pin LOW of the mottors is supposed to go BACKWARDS
    No.1 pin LOW  and No.2 pin LOW of the mottors is supposed to go STOP

*/

// Front Motors
int frontLeft1 = 14;
int frontLeft2 = 15;
int frontRight1 = 16;
int frontRight2 = 17;

// RearMototrs
int rearLeft1 = 18;
int rearLeft2 = 19;
int rearRight1 = 20;
int rearRight2 = 21;

#include <XBOXRECV.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXRECV Xbox(&Usb);

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));
}
void loop() {
  Usb.Task();
  if (Xbox.XboxReceiverConnected) {
    for (uint8_t i = 0; i < 4; i++) {
      if (Xbox.Xbox360Connected[i]) {
        if (Xbox.getButtonPress(L2, i) || Xbox.getButtonPress(R2, i)) {
          Serial.print("L2: ");
          Serial.print(Xbox.getButtonPress(L2, i));
          Serial.print("\tR2: ");
          Serial.println(Xbox.getButtonPress(R2, i));
          Xbox.setRumbleOn(Xbox.getButtonPress(L2, i), Xbox.getButtonPress(R2, i), i);
        }

        if (Xbox.getAnalogHat(LeftHatX, i) > 7500 || Xbox.getAnalogHat(LeftHatX, i) < -7500 || Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500 || Xbox.getAnalogHat(RightHatX, i) > 7500 || Xbox.getAnalogHat(RightHatX, i) < -7500 || Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500) {

          if (Xbox.getAnalogHat(LeftHatX) < -7500 && Xbox.getAnalogHat(LeftHatY) < -7500) {
            Serial.print(F("ONLY X NEGATIVE and Y NEGATIVE"));
            goBackwardsLeft();
          }
          else if (Xbox.getAnalogHat(LeftHatX) > 7500 && Xbox.getAnalogHat(LeftHatY) < -7500) {
            Serial.print(F("ONLY X POSITIVE and Y NEGATIVE"));
            goBackwardsRight();
          }
          else if (Xbox.getAnalogHat(LeftHatX) < -7500 && Xbox.getAnalogHat(LeftHatY) > 7500) {
            Serial.print(F("ONLY X NEGATIVE and Y POSITIVE"));
            goForwardLeft();
          }
          else if (Xbox.getAnalogHat(LeftHatX) > 7500 && Xbox.getAnalogHat(LeftHatY) > 7500) {
            Serial.print(F("ONLY X POSITIVE and Y POSITIVE"));
            goForwardRight();
          }
          else if (Xbox.getAnalogHat(LeftHatX) > 7500 && Xbox.getAnalogHat(LeftHatY) < -10000 && Xbox.getAnalogHat(LeftHatY) > 10000) {
            Serial.print(F("ONLY X POSITIVE"));
            goForward();
          }
          else if (Xbox.getAnalogHat(LeftHatX) < -7500 && Xbox.getAnalogHat(LeftHatY) < -10000 && Xbox.getAnalogHat(LeftHatY) > 10000) {
            Serial.print(F("ONLY X NEGATIVE"));
            goLeft();
          }
          else if (Xbox.getAnalogHat(LeftHatY) > 7500 && Xbox.getAnalogHat(LeftHatX) < -10000 && Xbox.getAnalogHat(LeftHatX) > 10000) {
            Serial.print(F("ONLY X POSITIVE"));
            goRight();
          }
          else if (Xbox.getAnalogHat(LeftHatY) < -7500 && Xbox.getAnalogHat(LeftHatX) < -10000 && Xbox.getAnalogHat(LeftHatX) > 10000) {
            Serial.print(F("ONLY Y NEGATIVE"));
            goBackwards();
          }  else {
            stopCommand();
          }
          Serial.println();
        }

        if (Xbox.getButtonClick(UP, i)) {
          Xbox.setLedOn(LED1, i);
          Serial.println(F("Up"));
        }
        if (Xbox.getButtonClick(DOWN, i)) {
          Xbox.setLedOn(LED4, i);
          Serial.println(F("Down"));
        }
        if (Xbox.getButtonClick(LEFT, i)) {
          Xbox.setLedOn(LED3, i);
          Serial.println(F("Left"));
        }
        if (Xbox.getButtonClick(RIGHT, i)) {
          Xbox.setLedOn(LED2, i);
          Serial.println(F("Right"));
        }

        if (Xbox.getButtonClick(START, i)) {
          Xbox.setLedMode(ALTERNATING, i);
          Serial.println(F("Start"));
        }
        if (Xbox.getButtonClick(BACK, i)) {
          Xbox.setLedBlink(ALL, i);
          Serial.println(F("Back"));
        }
        if (Xbox.getButtonClick(L3, i))
          Serial.println(F("L3"));
        if (Xbox.getButtonClick(R3, i))
          Serial.println(F("R3"));

        if (Xbox.getButtonClick(L1, i))
          Serial.println(F("L1"));
        if (Xbox.getButtonClick(R1, i))
          Serial.println(F("R1"));
        if (Xbox.getButtonClick(XBOX, i)) {
          Xbox.setLedMode(ROTATING, i);
          Serial.print(F("Xbox (Battery: "));
          Serial.print(Xbox.getBatteryLevel(i)); // The battery level in the range 0-3
          Serial.println(F(")"));
        }
        if (Xbox.getButtonClick(SYNC, i)) {
          Serial.println(F("Sync"));
          Xbox.disconnect(i);
        }

        if (Xbox.getButtonClick(A, i))
          Serial.println(F("A"));
        if (Xbox.getButtonClick(B, i))
          Serial.println(F("B"));
        if (Xbox.getButtonClick(X, i))
          Serial.println(F("X"));
        if (Xbox.getButtonClick(Y, i))
          Serial.println(F("Y"));
      }
    }
  }
}

// ---------------------- Start Motor Control -------------------------------------

void goForward() {
  digitalWrite(frontLeft1, HIGH);
  digitalWrite(frontLeft2, LOW);
  digitalWrite(frontRight1, HIGH);
  digitalWrite(frontRight2, LOW);
  digitalWrite(rearLeft1, HIGH);
  digitalWrite(rearLeft2, LOW);
  digitalWrite(rearRight1, HIGH);
  digitalWrite(rearRight2, LOW);
}

void goBackwards() {
  digitalWrite(frontLeft1, LOW);
  digitalWrite(frontLeft2, HIGH);
  digitalWrite(frontRight1, LOW);
  digitalWrite(frontRight2, HIGH);
  digitalWrite(rearLeft1, LOW);
  digitalWrite(rearLeft2, HIGH);
  digitalWrite(rearRight1, LOW);
  digitalWrite(rearRight2, HIGH);
}

void goLeft() {
  digitalWrite(frontLeft1, LOW);
  digitalWrite(frontLeft2, HIGH);
  digitalWrite(frontRight1, HIGH);
  digitalWrite(frontRight2, LOW);
  digitalWrite(rearLeft1, LOW);
  digitalWrite(rearLeft2, HIGH);
  digitalWrite(rearRight1, HIGH);
  digitalWrite(rearRight2, LOW);
}

void goForwardLeft() {
  digitalWrite(frontLeft1, LOW);
  digitalWrite(frontLeft2, LOW);
  digitalWrite(frontRight1, HIGH);
  digitalWrite(frontRight2, LOW);
  digitalWrite(rearLeft1, LOW);
  digitalWrite(rearLeft2, LOW);
  digitalWrite(rearRight1, HIGH);
  digitalWrite(rearRight2, LOW);
}

void goBackwardsLeft() {
  digitalWrite(frontLeft1, LOW);
  digitalWrite(frontLeft2, LOW);
  digitalWrite(frontRight1, LOW);
  digitalWrite(frontRight2, HIGH);
  digitalWrite(rearLeft1, LOW);
  digitalWrite(rearLeft2, LOW);
  digitalWrite(rearRight1, LOW);
  digitalWrite(rearRight2, HIGH);
}

void goRight() {
  digitalWrite(frontLeft1, HIGH);
  digitalWrite(frontLeft2, LOW);
  digitalWrite(frontRight1, LOW);
  digitalWrite(frontRight2, HIGH);
  digitalWrite(rearLeft1, HIGH);
  digitalWrite(rearLeft2, LOW);
  digitalWrite(rearRight1, LOW);
  digitalWrite(rearRight2, HIGH);
}

void goForwardRight() {
  digitalWrite(frontLeft1, HIGH);
  digitalWrite(frontLeft2, LOW);
  digitalWrite(frontRight1, LOW);
  digitalWrite(frontRight2, LOW);
  digitalWrite(rearLeft1, HIGH);
  digitalWrite(rearLeft2, LOW);
  digitalWrite(rearRight1, LOW);
  digitalWrite(rearRight2, LOW);
}

void goBackwardsRight() {
  digitalWrite(frontLeft1, LOW);
  digitalWrite(frontLeft2, HIGH);
  digitalWrite(frontRight1, LOW);
  digitalWrite(frontRight2, LOW);
  digitalWrite(rearLeft1, LOW);
  digitalWrite(rearLeft2, HIGH);
  digitalWrite(rearRight1, LOW);
  digitalWrite(rearRight2, LOW);
}

void stopCommand() {
  digitalWrite(frontLeft1, LOW);
  digitalWrite(frontLeft2, LOW);
  digitalWrite(frontRight1, LOW);
  digitalWrite(frontRight2, LOW);
  digitalWrite(rearLeft1, LOW);
  digitalWrite(rearLeft2, LOW);
  digitalWrite(rearRight1, LOW);
  digitalWrite(rearRight2, LOW);
}

// -------------------- End Motor Control ---------------------------------
