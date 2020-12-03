#include "pitches.h"

int melodyGameStart[] = {  NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_E5};
int melodyGameWon[] = {NOTE_B5, NOTE_E6, NOTE_E7};
int melodyGameLost[] = {NOTE_G4, NOTE_G5, NOTE_G6};

int durationsGameStart[] = {8, 8, 8, 4, 4, 4, 4, 5, 8, 8, 8, 8};
int durationsGameWon[] = {8, 8, 8};
int durationsGameLost[] = {8, 8, 8};

int songLengthGameStart = sizeof(melodyGameStart)/sizeof(melodyGameStart[0]);
int songLengthGameWon = sizeof(melodyGameWon)/sizeof(melodyGameWon[0]);
int songLengthGameLost = sizeof(melodyGameLost)/sizeof(melodyGameLost[0]);

int b1;
int b2;
int b3;

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(8, INPUT);
}

void loop() {
  b1 = digitalRead(2);
  b2 = digitalRead(3);
  b3 = digitalRead(4);

  if (!b1)
    singGameStart();
  else if (!b2)
    singGameWon();
  else if (!b3)
    singGameLost();
}

void singGameStart(){
  for (int thisNote = 0; thisNote < songLengthGameStart; thisNote++){
    int duration = 1000/ durationsGameStart[thisNote];
    tone(8, melodyGameStart[thisNote], duration);
    int pause = duration * 1.3;
    delay(pause);
    noTone(8);
  }
  pinMode(8, INPUT);
}

void singGameWon(){
  for (int thisNote = 0; thisNote < songLengthGameWon; thisNote++){
    int duration = 1000/ durationsGameWon[thisNote];
    tone(8, melodyGameWon[thisNote], duration);
    int pause = duration * 1.3;
    delay(pause);
    noTone(8);
  }
  pinMode(8, INPUT);
}

void singGameLost(){
  for (int thisNote = 0; thisNote < songLengthGameLost; thisNote++){
    int duration = 1000/ durationsGameLost[thisNote];
    tone(8, melodyGameLost[thisNote], duration);
    int pause = duration * 1.3;
    delay(pause);
    noTone(8);
  }
  pinMode(8, INPUT);
}
