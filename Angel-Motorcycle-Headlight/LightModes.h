#define LIGHT_GABARIT      4    //      A4 | PA4 | 09 package pin -> NOT PWM
#define LIGHT_LONG         5    // PWM  A5 | PA5 | 08 package pin
#define LIGHT_ANGEL        8    // PWM  #8 | PB2 | 05 package pin
#define LED_INDI_ANGEL     6    // PWM  A6 | PA6 | 07 package pin
#define LED_INDI_GABAR     7    // PWM  A7 | PA7 | 06 package pin
#define BTN_ON_ALL         2    //      A2 | PA2 | 11 package pin
#define BTN_LIGHT_LVL      1    //      A1 | PA1 | 12 package pin
#define BTN_ANGEL          0    //      A0 | PA0 | 13 package pin
#define HOLD_ON            3    //         | PA3 | 10 package pin
//      CONFIG
#define voltBtnSTAT        450
#define voltBtnSTATmin     100
#define POLICE_WAIT 30

unsigned long angelTime = 0, policeTime = 0, btnTime = 0, btnPressedTime = 0, timeToOFF = 0, offLedSwitchTime = 0;

uint8_t G_light = 0, L_light = 0, A_light = 0, angelMode = 0, mainMode = 0, police_count = 0, POLICE_PIN, angelWait = 10, indioff = 0;

bool btnStart = false, forceOffByBTN = false, proofMirg = false, angelRoute = true, angelState = false, police_a = true, police_b = true, police_c = true, police_state = true, btnState = false, stateAll = false, stateLvl = false, stateAngel = false, policeActive = false, angelFadeActive = false, forceOff = false, offLedSwitch = false, needleOff = false;
     
void policeStrobe(){
  if(police_count == 0){
    if(millis() - policeTime > 2000){
      police_count = 1;
    }
  }else{
    if(millis() - policeTime > POLICE_WAIT){
      police_count++;
      if(police_a == true){
        if(police_count < 7){
          POLICE_PIN = LIGHT_GABARIT;
        }else{
          police_a = false;
          police_b = true;
        }
      }else if(police_b == true){
        if(police_count < 13){
          POLICE_PIN = LIGHT_LONG;
        }else{
          police_b = false;
          police_c = true;
        }
      }else if(police_c == true){
        if(police_count < 19){
          POLICE_PIN = LIGHT_GABARIT;
        }else{
          police_c = false;
          police_a = true;
          police_count = 0;
        }
      }
      if(police_state == true){
        police_state = false;
        analogWrite(POLICE_PIN, 255);
        analogWrite(LED_INDI_GABAR,255);
      }else{
        police_state = true;
        analogWrite(POLICE_PIN, 0);
        analogWrite(LED_INDI_GABAR,0);
      }
      policeTime = millis();
    }
  }
}

void setAngelMode(){
  switch(angelMode){
    case 1:{
      A_light = 127;
      if(angelFadeActive == true) { angelFadeActive = false; }
      analogWrite(LIGHT_ANGEL,    A_light);
      analogWrite(LED_INDI_ANGEL, A_light);
    } break;
    case 2:{
      A_light = 255;
      if(angelFadeActive == true) { angelFadeActive = false; }
      analogWrite(LIGHT_ANGEL,    A_light);
      analogWrite(LED_INDI_ANGEL, A_light);
    } break;
    case 3:{
      angelWait = 15;
      if(angelFadeActive == false) { angelFadeActive = true; }
    } break;
    case 4:{
      angelWait = 1;
      if(angelFadeActive == false) { angelFadeActive = true; }
    } break;
    default:{
      angelMode = 0;
      A_light = 0;
      if(angelFadeActive == true) { angelFadeActive = false; }
      analogWrite(LIGHT_ANGEL,    A_light);
      analogWrite(LED_INDI_ANGEL, 0);
      if(proofMirg == true) proofMirg = false;
    } break;
  }
}

void angelWork(){
  if(millis() - angelTime > angelWait){
    if(angelRoute == true){
      if(A_light < 255){
        A_light++;
      }else{
        angelRoute = false;
        A_light--;
      }
    }else{
      if(A_light > 0){
        A_light--;
      }else{
        angelRoute = true;
        A_light++;
      }
    }
    analogWrite(LIGHT_ANGEL,    A_light);
    analogWrite(LED_INDI_ANGEL, A_light);
    angelTime = millis();
  }
}

void setMainMode(){
  switch(mainMode){
    case 1:{
      G_light = 255; 
      L_light = 0;
      analogWrite(LED_INDI_GABAR,63);
    } break;
    case 2:{
      G_light = 0; 
      L_light = 255;
      analogWrite(LED_INDI_GABAR,127);
    } break;
    case 3:{
      G_light = 255; 
      L_light = 255;
      analogWrite(LED_INDI_GABAR,255);
    } break;
    case 4:{
      if(policeActive == false){
        policeActive = true;
        police_count = 0;
        if(police_a == false) police_a = true;
        if(police_b == false) police_b = true;
        if(police_c == false) police_c = true;
        if(police_state == false) police_state = true;
      }
    } break;
    default:{
      if(policeActive == true){
        policeActive = false;
      }
      G_light = 0;
      L_light = 0;
      analogWrite(LED_INDI_GABAR,0);
      mainMode = 0;
      if(proofMirg == true) proofMirg = false;
    }break;
  }
  analogWrite(LIGHT_GABARIT, G_light);
  analogWrite(LIGHT_LONG,    L_light);
}

void setAllLight(){
  G_light = 255;
  L_light = 255;
  A_light = 255;
  analogWrite(LED_INDI_ANGEL,A_light);
  analogWrite(LED_INDI_GABAR,G_light);
  analogWrite(LIGHT_GABARIT,G_light);
  analogWrite(LIGHT_ANGEL,A_light);
  analogWrite(LIGHT_LONG,L_light);
}

void offAllLight(){
  G_light = 0;
  L_light = 0;
  A_light = 0;
  analogWrite(LED_INDI_ANGEL,A_light);
  analogWrite(LED_INDI_GABAR,G_light);
  analogWrite(LIGHT_GABARIT,G_light);
  analogWrite(LIGHT_ANGEL,A_light);
  analogWrite(LIGHT_LONG,L_light);
}

void doWork(){
  if(forceOff == false){
    if(stateAll == false){
      if(policeActive == true){
        policeStrobe();
      }
      if(angelFadeActive == true){
        angelWork();
      }
    }
  }
}

void buttonStart(){
  if(btnStart == false) {
    btnStart = true;
    btnPressedTime = millis();
  }
}

void buttonsEnd(){
  if(btnStart == true){
    btnStart = false;
    if(proofMirg == true) proofMirg = false;
    if(needleOff == true) needleOff = false;
  }
}

void changeModeAccess(){
  bool doIt = false;
  if(stateAll == true){
    if(analogRead(BTN_ON_ALL) < voltBtnSTATmin){
      stateAll = false;
      doIt = true;
      setAngelMode();
      setMainMode();
    }
  }
  if(stateAngel == true){
    if(analogRead(BTN_ANGEL) < voltBtnSTATmin){
      stateAngel = false;
      doIt = true;
      angelMode++;
      setAngelMode();
    }
  }
  if(stateLvl == true){
    if(analogRead(BTN_LIGHT_LVL) < voltBtnSTATmin){
      stateLvl = false;
      doIt = true;
      mainMode++;
      setMainMode();
    }
  }
  if(doIt){
    buttonsEnd();
  }
}

void buttonWork(){
  bool btnAct = false;
  if(analogRead(BTN_ON_ALL) > voltBtnSTAT){
    if(stateAll == false){
      stateAll = true;
      setAllLight();
      btnAct = true;
    }
  }else if(analogRead(BTN_ANGEL) > voltBtnSTAT){
    if(stateAngel == false){
      stateAngel = true;
      btnAct = true;
      buttonStart();
    }
  }else if(analogRead(BTN_LIGHT_LVL) > voltBtnSTAT){
    if(stateLvl == false){
      stateLvl = true;
      btnAct = true;
      buttonStart();
    }
  }
  if(btnAct == true){
    btnState = true;
    btnTime = millis();
  }
}

void preventOff(){
  bool doIt = false;
  if(analogRead(BTN_ANGEL) > voltBtnSTAT){
    doIt = true;
  }else if(analogRead(BTN_LIGHT_LVL) > voltBtnSTAT){
    doIt = true;
  }else if(analogRead(BTN_ON_ALL) > voltBtnSTAT){
    doIt = true;
  }
  if(doIt == true){
    if(forceOff == true) forceOff = false;
    if(proofMirg == true) proofMirg = false;
    needleOff = false;
    indioff = 0;
    buttonWork();
  }
}

void offPrepare(){
  if(millis() - offLedSwitchTime > 99){
    if(offLedSwitch == false){
      analogWrite(LED_INDI_ANGEL, 255);
      analogWrite(LED_INDI_GABAR, 255);
      offLedSwitch = true;
    }else{
      analogWrite(LED_INDI_ANGEL, 0  );
      analogWrite(LED_INDI_GABAR, 0  );
      offLedSwitch = false;
    }
    offLedSwitchTime = millis();
    indioff++;
  }
  if(indioff == 6){
    timeToOFF = millis();
    forceOff = true;
    analogWrite(LED_INDI_ANGEL, 0);
    analogWrite(LED_INDI_GABAR, 0);
    needleOff = false;
  }
}
