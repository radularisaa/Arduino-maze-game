#include "pitches.h"
#include "SevSeg.h"
#include <Servo.h>

// declararea melodiilor
int melodyGameStart[] = {NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_E5};
int melodyGameWon[] = {NOTE_B5, NOTE_E6, NOTE_E7};
int melodyGameLost[] = {NOTE_G4, NOTE_C4};

// declararea duratelor melodiilor
int durationsGameStart[] = {7, 7, 7, 4, 4, 4, 4, 5, 7, 7, 7, 7};
int durationsGameWon[] = {8, 8, 8};
int durationsGameLost[] = {4, 4};

// declararea lungimilor melodiilor
int songLengthGameStart = sizeof(melodyGameStart) / sizeof(melodyGameStart[0]);
int songLengthGameWon = sizeof(melodyGameWon) / sizeof(melodyGameWon[0]);
int songLengthGameLost = sizeof(melodyGameLost) / sizeof(melodyGameLost[0]);

//pinul corespunzator buzzerului
int buzzer_pin = 10;
// variabila pentru a marca inceputul jocului
int sing_start_buzzer = 0;

// declararea celor 2 servo motoare
Servo servoX;
Servo servoY;

// pinii corespunzatori servomotoarelor
int pinServoX = 8;
int pinServoY = 9;

// pinii corespunzatori joystick-ului
int VRx = A0;
int VRy = A1;
int SW = 2;

// pinul corespunzator switch-ului
int switch_pin = 3;

// variabile folosite pentu citirea starii joystick-ului
int xPos = 0;
int yPos = 0;
int SWState = 0;

// variabile folosite la maparea la un anumit interval valorile primite
int mapX = 0;
int mapY = 0;

// pozitia servomotoarelor
int servoPosX = 87;
int servoPosY = 84;

// decararea afisorului 7 segmente folosind bibloteca SevSeg
SevSeg sevseg;
// initializarea unui sir de caractere in care se va stoca scorul
char nr[5] = "B0R0";

// pinii corespunzatori senzorului de culoare
int s0 = 4;
int s1 = 5;
int s2 = 6;
int s3 = 7;
int out_sensor = 11;

// variabile in care se vor stoca valorile frecventelor vazute de senzor
unsigned int frequencyR = 0; // pentru rosu
unsigned int frequencyB = 0; // albastru
unsigned int frequencyG = 0; // verde

void setup() {
  // put your setup code here, to run once:

  // se incepe comunicarea seriala
  Serial.begin(9600);

  // se ataseaza motoarele servo la pinii 8 si 9 de pe placa arduino Mega
  servoX.attach(pinServoX);
  servoY.attach(pinServoY);

  // se seteaza servomotoarele la 90 grade
  servoX.write(servoPosX);
  servoY.write(servoPosY);

  // se seteaza pinii pentru joystick
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP);

  // se seteaza pinul pentru switch
  pinMode(switch_pin, INPUT_PULLUP);

  // declararea semnalelor necesare folosirii afisorului si initializarea lor
  byte numDigits = 4; // numarul de afisoare
  byte digitPins[] = {50, 51, 52, 53}; //pinii de pe placa la care este conectat afisorul
  byte segmentPins[] = {22, 23, 24, 25, 26, 27, 28, 29};

  bool resistorsOnSegments = true; // se activeaza rezistentele pe pinii afisorului (pentru a nu fi nevoie de adaugarea rezistentelor fizic)
  bool updateWithDelaysIn = true; // se activeaza update ul cu intarzieri
  byte hardwareConfig = COMMON_CATHODE; // se seteaza pe catod comun
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments); // initializarea afisorului
  sevseg.setBrightness(90); // setarea luminozitatii afisorului

  // se seteaza pinii pentru senzorul de culoare
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out_sensor, INPUT);

  // se seteaza frecventa de selectie la 20%
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);

  SWState = 1;
}

void loop() {
  // put your main code here, to run repeatedly:

  // daca switch-ul are valoarea 1 (adica daca s-a pornit jocul) se executa:
  if (digitalRead(switch_pin)) {
    if (!sing_start_buzzer) // daca buzzerul nu a mai fost activat de la ultima pornire
    {
      singGameStart(); // sunetul de start
      sing_start_buzzer = 1;
    }

    if (!SWState) { // daca se apasa butonul joystick-ului
      for (int i = 0; i < 200; i++) {
        sevseg.setChars(nr); // se seteaza numarul pe afisor
        sevseg.refreshDisplay(); // se reimprospateaza afisorul
      }
      digitalWrite(6, LOW);  //combinatia pentru detectarea
      digitalWrite(7, LOW);  // frecventei culorii rosu
      frequencyR = pulseIn(out_sensor, LOW); // se citeste frecventa pulsului
      for (int i = 0; i < 200; i++) { // o bucla for in care se reimprospateaza afisorul pentru a crea un delay
        sevseg.setChars(nr);
        sevseg.refreshDisplay();
      }
      digitalWrite(6, LOW); // combinatia pentru detectarea
      digitalWrite(7, HIGH); // frecventei culorii albastru
      frequencyB = pulseIn(out_sensor, LOW); // se citeste frecventa pulsului
      for (int i = 0; i < 200; i++) { // o bucla for in care se reimprospateaza afisorul pentru a crea un delay
        sevseg.setChars(nr);
        sevseg.refreshDisplay();
      }
      digitalWrite(6, HIGH); // combinatia pentru detectarea
      digitalWrite(7, HIGH); // frecventei culorii verde
      frequencyG = pulseIn(out_sensor, LOW); // se citeste frecventa pulsului
      // se trimit serial valorile frecventelor celor 3 culori
      Serial.print("Red ");
      Serial.println(frequencyR);
      Serial.print(" Blue ");
      Serial.println(frequencyB);
      Serial.print(" Green ");
      Serial.println(frequencyG);

      // daca se detecteaza culoarea albastra
      if (frequencyG <= 78) ////// trebuie modificata valoarea in functie de noi masuratori
      {
        if (nr[1] == 9) // daca scorul a ajuns la 9 se reseteaza tabelul
        {
          nr[1] = 0;
          nr[3] = 0;
        }
        else // altfel se incrementeaza punctajul pentru albastru
          nr[1]++;
        singGameWon(); // se activeaza melodia care indica castigarea jocului
      } // altfel daca se detecteaza culoarea rosie
      else {
        if (nr[3] == 9) // daca scorul a ajuns la 9 se reseteaza tabelul
        {
          nr[3] = 0;
          nr[1] = 0;
        }
        else
          nr[3]++; // altfel se incrementeaza punctajul pentru rosu
        singGameLost(); // se activeaza melodia care indica pierderea jocului
      }
    }

    // se se citesc informatii de la joystick
    xPos = analogRead(VRx);
    yPos = analogRead(VRy);
    SWState = digitalRead(SW);

    // se mapeaza X si Y dupa informatiile venite de la joystick, in at interval de valor
    mapX = map(xPos, 0, 1023, -5, 5);
    mapY = map(yPos, 0, 1023, -5, 5);

    // se afiseaza in monitorul serial valorile venite de la joystick
    //    Serial.print(mapX);
    //    Serial.print(" ");
    //    Serial.println(mapY);

    // se compara valorile mapate cu o referinta
    // -1 deoarece aceasta valoare este returnata in pozitia de repaus
    // din cauza intervalului mic de valori stabilit la liniile 43 si 44
    // in cazul in care valorile sunt mai mari decat referinta, se incrementeaza pozitia
    // servomotorului corespunzator, se scrie valoarea si se asteapta
    if (mapX > -1) {
      servoPosX++;
      servoX.write(servoPosX);
      // in loc de delay, se foloseste peste tot o bucla in care tot ce se intampla este reimprospatarea afisorului
      // din cauza ca aceasta biblioteca nu functioneaza corespunzator cu intarzierile clasice
      for (int i = 0; i < 100; i++) {
        sevseg.setChars(nr);
        sevseg.refreshDisplay();
      }
    }
    else if (mapX < -1) { // in cazul in care valorile sunt mai mici decat referinta, se decrementeaza pozitia servomotorului corespunzator
      servoPosX--;
      servoX.write(servoPosX);
      for (int i = 0; i < 100; i++) {
        sevseg.setChars(nr);
        sevseg.refreshDisplay();
      }
    }
    if (mapY > -1) {
      servoPosY++;
      servoY.write(servoPosY);
      for (int i = 0; i < 100; i++) {
        sevseg.setChars(nr);
        sevseg.refreshDisplay();
      }
    }
    else if (mapY < -1) {
      servoPosY--;
      servoY.write(servoPosY);
      for (int i = 0; i < 100; i++) {
        sevseg.setChars(nr);
        sevseg.refreshDisplay();
      }
    }

    // din nou, in loc de un delay se foloseste un for
    for (int i = 0; i < 300; i++) {
      sevseg.setChars(nr);
      sevseg.refreshDisplay();
    }
  }
  else // in caz contrar, se transmite serial un mesaj
  {
    Serial.println("Jocul este oprit");
    sing_start_buzzer = 0; // se marcheaza pe 0 sunetul buzzerului ca la urmatoarea pornire sa cante dinnou
    sevseg.blank(); // nu se mai afiseaza nimic pe afisor
    nr[3] = 0; // se reseteaza scorul pentru ambele bile
    nr[1] = 0;
  }
}

void singGameStart() { // functie care activeaza melodia de inceput de joc
  for (int thisNote = 0; thisNote < songLengthGameStart; thisNote++) { //se parcurge melodia
    int duration = 1000 / durationsGameStart[thisNote]; // calculeaza cat trebuie sa dureze nota
    tone(buzzer_pin, melodyGameStart[thisNote], duration);
    int pause = duration * 1.3;
    delay(pause);
    noTone(buzzer_pin); // se opreste tonul
  }
  pinMode(buzzer_pin, INPUT); // se opreste cantatul
}

void singGameWon() { // functie care activeaza melodia de castig de joc
  for (int thisNote = 0; thisNote < songLengthGameWon; thisNote++) { //se parcurge melodia
    int duration = 1000 / durationsGameWon[thisNote]; // calculeaza cat trebuie sa dureze nota
    tone(buzzer_pin, melodyGameWon[thisNote], duration);
    int pause = duration * 1.3;
    delay(pause);
    noTone(buzzer_pin);  // se opreste tonul
  }
  pinMode(buzzer_pin, INPUT);  // se opreste cantatul
}

void singGameLost() { // functie care activeaza melodia de pierdere de joc
  for (int thisNote = 0; thisNote < songLengthGameLost; thisNote++) { //se parcurge melodia
    int duration = 1000 / durationsGameLost[thisNote]; // calculeaza cat trebuie sa dureze nota
    tone(buzzer_pin, melodyGameLost[thisNote], duration);
    int pause = duration * 1.3;
    delay(pause);
    noTone(buzzer_pin); // se opreste tonul
  }
  pinMode(buzzer_pin, INPUT);  // se opreste cantatul
}
