#include <XBOXONE.h>
#include <SPI.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
USB Usb;
XBOXONE Xbox(&Usb);
 
/*
 *  No.1 pin HIGH and No.2 pin LOW of the mottors is supposed to go FORWARD
 *  No.2 pin HIGH and No.1 pin LOW of the mottors is supposed to go BACKWARDS
 *  No.1 pin LOW  and No.2 pin LOW of the mottors is supposed to go STOP
 *  
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

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXBOX USB Library Started"));
}
void loop() {
  Usb.Task();
  if (Xbox.XboxOneConnected) {
    if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
      
      if (Xbox.getAnalogHat(LeftHatX) < -7500 && Xbox.getAnalogHat(LeftHatY) < -7500) {
        Serial.print(F("ONLY X NEGATIVE and Y NEGATIVE"));
        goBackwardsLeft();
        //Serial.print(F("LeftHatX: "));
        //Serial.print(Xbox.getAnalogHat(LeftHatX));
        //Serial.print("\t");
      }
      else if (Xbox.getAnalogHat(LeftHatX) > 7500 && Xbox.getAnalogHat(LeftHatY) < -7500) {
        Serial.print(F("ONLY X POSITIVE and Y NEGATIVE"));
        goBackwardsRight();
        //Serial.print(F("LeftHatX: "));
        //Serial.print(Xbox.getAnalogHat(LeftHatX));
        //Serial.print("\t");
      }
      else if (Xbox.getAnalogHat(LeftHatX) < -7500 && Xbox.getAnalogHat(LeftHatY) > 7500) {
        Serial.print(F("ONLY X NEGATIVE and Y POSITIVE"));
        goForwardLeft();
        //Serial.print(F("LeftHatX: "));
        //Serial.print(Xbox.getAnalogHat(LeftHatX));
        //Serial.print("\t");
      }
      else if (Xbox.getAnalogHat(LeftHatX) > 7500 && Xbox.getAnalogHat(LeftHatY) > 7500) {
        Serial.print(F("ONLY X POSITIVE and Y POSITIVE"));
        goForwardRight();
        //Serial.print(F("LeftHatX: "));
        //Serial.print(Xbox.getAnalogHat(LeftHatX));
        //Serial.print("\t");
      }
      else if (Xbox.getAnalogHat(LeftHatX) > 7500 && Xbox.getAnalogHat(LeftHatY) < -7500 && Xbox.getAnalogHat(LeftHatY) > 7500) {
        Serial.print(F("ONLY X POSITIVE"));
        goForward();
        //Serial.print(F("LeftHatX: "));
        //Serial.print(Xbox.getAnalogHat(LeftHatX));
        //Serial.print("\t");
      }
      else if (Xbox.getAnalogHat(LeftHatX) < -7500 && Xbox.getAnalogHat(LeftHatY) < -7500 && Xbox.getAnalogHat(LeftHatY) > 7500) {
        Serial.print(F("ONLY X NEGATIVE"));
        goLeft();
        //Serial.print(F("LeftHatX: "));
        //Serial.print(Xbox.getAnalogHat(LeftHatX));
        //Serial.print("\t");
      }
      else if (Xbox.getAnalogHat(LeftHatY) > 7500 && Xbox.getAnalogHat(LeftHatX) < -7500 && Xbox.getAnalogHat(LeftHatX) > 7500) {
        Serial.print(F("ONLY X POSITIVE"));
        goRight();
        //Serial.print(F("LeftHatX: "));
        //Serial.print(Xbox.getAnalogHat(LeftHatX));
        //Serial.print("\t");
      }
      else if (Xbox.getAnalogHat(LeftHatY) < -7500 && Xbox.getAnalogHat(LeftHatX) < -7500 && Xbox.getAnalogHat(LeftHatX) > 7500) {
        Serial.print(F("ONLY Y NEGATIVE"));
        goBackwards();
        //Serial.print(F("LeftHatX: "));
        //Serial.print(Xbox.getAnalogHat(LeftHatX));
        //Serial.print("\t");
      }  else {
        stopCommand();
      }

 
      if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
        Serial.print(F("LeftHatY: "));
        Serial.print(Xbox.getAnalogHat(LeftHatY));
        Serial.print("\t");
      }
      if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
        Serial.print(F("RightHatX: "));
        Serial.print(Xbox.getAnalogHat(RightHatX));
        Serial.print("\t");
      }
      if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
        Serial.print(F("RightHatY: "));
        Serial.print(Xbox.getAnalogHat(RightHatY));
      }
      Serial.println();
    }

    if (Xbox.getButtonPress(L2) > 0 || Xbox.getButtonPress(R2) > 0) {
      if (Xbox.getButtonPress(L2) > 0) {
        Serial.print(F("L2: "));
        Serial.print(Xbox.getButtonPress(L2));
        Serial.print("\t");
      }
      if (Xbox.getButtonPress(R2) > 0) {
        Serial.print(F("R2: "));
        Serial.print(Xbox.getButtonPress(R2));
        Serial.print("\t");
      }
      Serial.println();
    }

    // Set rumble effect
    static uint16_t oldL2Value, oldR2Value;
    if (Xbox.getButtonPress(L2) != oldL2Value || Xbox.getButtonPress(R2) != oldR2Value) {
      oldL2Value = Xbox.getButtonPress(L2);
      oldR2Value = Xbox.getButtonPress(R2);
      uint8_t leftRumble = map(oldL2Value, 0, 1023, 0, 255); // Map the trigger values into a byte
      uint8_t rightRumble = map(oldR2Value, 0, 1023, 0, 255);
      if (leftRumble > 0 || rightRumble > 0)
        Xbox.setRumbleOn(leftRumble, rightRumble, leftRumble, rightRumble);
      else
        Xbox.setRumbleOff();
    }

    if (Xbox.getButtonClick(UP))
      Serial.println(F("Up"));
    if (Xbox.getButtonClick(DOWN))
      Serial.println(F("Down"));
    if (Xbox.getButtonClick(LEFT))
      Serial.println(F("Left"));
    if (Xbox.getButtonClick(RIGHT))
      Serial.println(F("Right"));

    if (Xbox.getButtonClick(START))
      Serial.println(F("Start"));
    if (Xbox.getButtonClick(BACK))
      Serial.println(F("Back"));
    if (Xbox.getButtonClick(XBOX))
      Serial.println(F("Xbox"));
    if (Xbox.getButtonClick(SYNC))
      Serial.println(F("Sync"));

    if (Xbox.getButtonClick(L1))
      Serial.println(F("L1"));
    if (Xbox.getButtonClick(R1))
      Serial.println(F("R1"));
    if (Xbox.getButtonClick(L2))
      Serial.println(F("L2"));
    if (Xbox.getButtonClick(R2))
      Serial.println(F("R2"));
    if (Xbox.getButtonClick(L3))
      Serial.println(F("L3"));
    if (Xbox.getButtonClick(R3))
      Serial.println(F("R3"));


    if (Xbox.getButtonClick(A))
      Serial.println(F("A"));
    if (Xbox.getButtonClick(B))
      Serial.println(F("B"));
    if (Xbox.getButtonClick(X))
      Serial.println(F("X"));
    if (Xbox.getButtonClick(Y))
      Serial.println(F("Y"));
  }
  delay(1);
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
