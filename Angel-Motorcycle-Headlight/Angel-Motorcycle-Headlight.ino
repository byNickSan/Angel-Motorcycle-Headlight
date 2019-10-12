/*  ________________________________________________________________________________________________________
 *  | Circuit  Author: @kirillbatisto                                                                      |
 *  | Software Author: @byNickSan                                                                          |
 *  |                                                                                                      |
 *  | Chip: ATtiny44                                                                                       |
 *  |                                                                                                      |
 *  | FuseBits > Default: LOW-> 0xE4 , HIGH-> 0xDE , EXTENDED-> 0xFF (New Chip) -> its real!               |
 *  | FuseBits > New:     LOW-> 0xE2 , HIGH-> 0xDF , EXTENDED-> 0xFF (without Oscillator) -> 8Mhz Internal |
 *  |______________________________________________________________________________________________________|
 */
#define F_CPU 8000000L
#include "LightModes.h"

void setup() {
  detachInterrupt(HOLD_ON);
  pinMode(HOLD_ON  ,OUTPUT); // hold on
  pinMode(BTN_ANGEL       ,INPUT);  // A0 | PA0 | 13 package pin -  Register ON Angel eye
  pinMode(BTN_LIGHT_LVL   ,INPUT);  // A1 | PA1 | 12 package pin -  Register Front parking light (GABARIT) and high beam (LONG LIGHT)
  pinMode(BTN_ON_ALL      ,INPUT);  // A2 | PA2 | 11 package pin -  Force enable All lights
  pinMode(LIGHT_ANGEL     ,OUTPUT); // -- | PB2 | 05 package pin -  Enable Angel eye
  pinMode(LIGHT_GABARIT   ,OUTPUT); // A4 | PA4 | 09 package pin -  Front parking light (GABARIT)
  pinMode(LIGHT_LONG      ,OUTPUT); // A5 | PA5 | 08 package pin -  High beam (LONG LIGHT)
  pinMode(LED_INDI_ANGEL  ,OUTPUT); // A6 | PA6 | 07 package pin -  Indication button of ANGEL EYE
  pinMode(LED_INDI_GABAR  ,OUTPUT); // A7 | PA7 | 06 package pin -  Indication button of GABARIT
  digitalWrite(HOLD_ON, HIGH);
  if( (analogRead(BTN_LIGHT_LVL) > 350) or (analogRead(BTN_ANGEL) > 350) ){
    digitalWrite(HOLD_ON, HIGH);
    buttonWork();
  }else if( analogRead(BTN_ON_ALL) > 350){
    while(1){
      if( (analogRead(BTN_LIGHT_LVL) > voltBtnSTAT) or (analogRead(BTN_ANGEL) > voltBtnSTAT) ){
        digitalWrite(HOLD_ON, HIGH);
        buttonWork();
        break;
      }
      if( (analogRead(BTN_ON_ALL) > voltBtnSTAT ) ){
        if(stateAll == false){
          stateAll = true;
          setAllLight();
          digitalWrite(HOLD_ON, HIGH);
        }
      }else{
        if(stateAll == true){
          stateAll = false;
          offAllLight();
          digitalWrite(HOLD_ON, LOW);
        }
      }
    }
  }else{
    digitalWrite(HOLD_ON, LOW);
  }
}

void loop() {
  if(forceOff == true){
    if( ( (angelMode == 0) and (mainMode == 0) and (stateAll == false) ) or (forceOffByBTN == true) ){
      if(millis() - timeToOFF > 999){
        digitalWrite(HOLD_ON, LOW);
        delay(5000);
      }else{
        if(forceOffByBTN == false){
          preventOff();
        }
      }
    }else{
      forceOff = false;
      needleOff = false;
      indioff = 0;
      needleOff = false;
      if(digitalRead(HOLD_ON) == LOW){
        digitalWrite(HOLD_ON, HIGH);
      }
    }
  }else{
    if(needleOff == false){
      if(btnState == false){
        buttonWork();
      }else{
        if(millis() - btnTime > 100){
          changeModeAccess();
          if(stateAll == false){
            if( (stateAngel == false) and (stateLvl == false) ){
              if( (angelMode == 0) and (mainMode == 0) ){
                needleOff = true;
                indioff = 0;
              }else{
                if(proofMirg == true) proofMirg = false;
                if(needleOff == true) needleOff = false;
              }
              btnState = false;
            }
          }
          btnTime = millis();
          if(btnStart == true) {
            if( (btnTime - btnPressedTime) > 1999 ){
              needleOff = true;
              indioff = 0;
              btnStart = false;
              if(forceOffByBTN == false) { forceOffByBTN = true; offAllLight(); }
              if(proofMirg == true) proofMirg = false;
            }
          }
        }
      }
      doWork();
    }else{
      if(proofMirg == false){
        if(indioff < 6){
          offPrepare();
          if(forceOffByBTN == false){
            preventOff();
          }
        }
      }else{
        analogWrite(LED_INDI_ANGEL, 0);
        analogWrite(LED_INDI_GABAR, 0);
        needleOff = false;
        forceOff = true;
        timeToOFF = millis();
      }
    }
  }
}
