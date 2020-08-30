/*
   Simple sketch to read 5 RC Channels connectet to Pin D3 - D7

   If you want to change the Pins you want to use, you can simply change the pin definitions
   down below. But don't forget to use the right pin definition in the FastRCReader.h (At PINS_TO_USE)!
*/
//code pour le récepteur rc)
#include <FastRCReader.h>

//Time definitions
#define REFRESHRATE 50

FastRCReader RC;

#define DIR 2 // channel direction
#define MOT 3 // channel moteur

#define fowardPwmG 10 // sorti pwm pour moteur gauche
#define reversePwmG 11 // sorti pwm pour moteur gauche
#define fowardPwmD 5
#define reversePwmD 6


int varSpeed = 0;
int varDirection = 0;
int varLimitsp = 100; //limit vitesse de 255= aucune limite, 0= pas de mouvement
int varTemp = 0 ; //variable temporaire pour calcul de la vitesse

void setup() {
  //Initalise the Serial communication and the RC RX Reader
  Serial.begin(115200);
  RC.begin();
  RC.addChannel(DIR);
  RC.addChannel(MOT);
  // pinMode(enA, OUTPUT);
  //pinMode(enB, OUTPUT);
  analogWrite(fowardPwmG, 0);
  analogWrite(reversePwmG, 0);
  analogWrite(fowardPwmD, 0);
  analogWrite(reversePwmD, 0);
  //digitalWrite(in1, 0);
  //digitalWrite(in2, 0);
  //digitalWrite(in3, 0);
  //digitalWrite(in4, 0);
  delay(500);
}

void loop() {
  Serial.print("Freq Moteur: ");
  Serial.println(RC.getFreq(MOT));
  Serial.print("Freq Servo: ");
  Serial.println(RC.getFreq(DIR));
  varSpeed = RC.getFreq(MOT);
  varDirection = RC.getFreq(DIR);
  Serial.println("");

  //TRAITEMENT DE LA VARIABLE VITESSE
  Serial.print("MOTEUR: ");
  varSpeed = map(varSpeed, 1000, 2000, -varLimitsp, varLimitsp); //center over zer0
  //varSpeed = constrain(varSpeed, -varLimitsp, varLimitsp); //only pass values whose absolutes are
  Serial.println(varSpeed);

  if (varSpeed < 10 && varSpeed > -10)varSpeed = 0;

  // FIN DE TRAITEMENT DE LA VARIABLE VITESSE

  //TRAITEMENT DE LA VARIABLE DIRECTION
  Serial.print("DIRECTION: ");
  varDirection = map(varDirection, 1000, 2000, -varLimitsp, varLimitsp); //center over zero
  //varDirection = constrain(varDirection, -varLimitsp, varLimitsp); //only pass values whose absolutes are
  Serial.println(varDirection);


  if (varDirection < 10 && varDirection > -10)varDirection = 0;

  // FIN DE TRAITEMENT DE LA VARIABLE DIRECTION

  //debut du code pour le mouvement
  varTemp = (abs(varSpeed)) - abs(varDirection); // variation de la vitesse de la roue pour tourne
  Serial.print("varTemp : ");
  Serial.println(varTemp);
  if (varSpeed == 0) {
    Serial.println("Neutral");
    analogWrite(fowardPwmG, 0);
    analogWrite(fowardPwmD, 0);
    analogWrite(reversePwmG, 0);
    analogWrite(reversePwmD, 0);

  } else { // si la vitesse n'est pas nul alors on bouge

    if (varSpeed < 0) { //code pour reculer--------------------------------------

      if (varDirection < 0) { //code pour tourner a gauche
        analogWrite(fowardPwmG, 0);
        analogWrite(fowardPwmD, 0);
        analogWrite(reversePwmG, abs(varTemp));
        analogWrite(reversePwmD, abs(varSpeed));
      }
      if (varDirection > 0) { // code pour tourner a droite
        analogWrite(fowardPwmG, 0);
        analogWrite(fowardPwmD, 0);
        analogWrite(reversePwmG, abs(varSpeed));
        analogWrite(reversePwmD, abs(varTemp));
      }
      if (varDirection == 0) {// code pour simplement reculer
        analogWrite(fowardPwmG, 0);
        analogWrite(fowardPwmD, 0);
        analogWrite(reversePwmG, abs(varSpeed));
        analogWrite(reversePwmD, abs(varSpeed));
      }

    } else { // code pour avancer

      if (varDirection < 0) { //code pour tourner a gauche
        
        
        analogWrite(fowardPwmG, varTemp);
        analogWrite(fowardPwmD, abs(varSpeed));
        analogWrite(reversePwmG, 0 );
        analogWrite(reversePwmD, 0 );
      }
      if (varDirection > 0) { // code pour tourner a droite
        analogWrite(fowardPwmG, abs(varSpeed));
        Serial.print("varSpeed tour a droite");
        Serial.println(varTemp);
        analogWrite(fowardPwmD, (varTemp));
        analogWrite(reversePwmG, 0 );
        analogWrite(reversePwmD, 0 );
      }
      if (varDirection == 0) {// code pour simplement reculer
        analogWrite(fowardPwmG, abs(varSpeed));
        analogWrite(fowardPwmD, abs(varSpeed));
        analogWrite(reversePwmG, 0 );
        analogWrite(reversePwmD, 0 );
      }

    }

  }





  //Wait till the refreshrate is expiered
  unsigned long waitSince = millis();
  while ((millis() - waitSince) < REFRESHRATE);
}
