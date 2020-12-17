#include <Servo.h>

// Declararea celor 2 servo motoare
Servo servoX;
Servo servoY;

// pinii corespsunzatori servomotoarelor
int pinServoX = 8;
int pinServoY = 9;

// pinii corespunzatori joystick-ului
int VRx = A0;
int VRy = A1;
int SW = 2;

// variabile folosite pentu citirea starii joystick-ului
int xPos = 0;
int yPos = 0;
int SWState = 0;

// variabile folosite la maparea la un anumit interval valorile primite
int mapX = 0;
int mapY = 0;

// pozitia servomotoarelor
int servoPosX = 90;
int servoPosY = 90;

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
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // se se citesc informatii de la joystick
  xPos = analogRead(VRx);
  yPos = analogRead(VRy);
  SWState = digitalRead(SW);

  // se mapeaza X si Y dupa informatiile venite de la joystick, in at interval de valor
  mapX = map(xPos, 0, 1023, -5, 5);
  mapY = map(yPos, 0, 1023, -5, 5);

  // se afiseaza in monitorul serial valorile venite de la joystick
  Serial.print(mapX);
  Serial.print(" ");
  Serial.println(mapY);

  // se compara valorile mapate cu o referinta
  // -1 deoarece aceasta valoare este returnata in pozitia de repaus
  // din cauza intervalului mic de valori stabilit la liniile 43 si 44
  // in cazul in care valorile sunt mai mari decat referinta, se incrementeaza pozitia
  // servomotorului corespunzator, se scrie valoarea si se asteapta
  if (mapX > -1){
    servoPosX++;
    servoX.write(servoPosX);
    delay((abs(mapX) + 10) * 6);
  }
  else if (mapX < -1){
    servoPosX--;
    servoX.write(servoPosX);
    delay((abs(mapX) + 10) * 6);
  }
  
  if (mapY > -1){
    servoPosY++;
    servoY.write(servoPosY);
    delay((abs(mapY) + 10) * 6);
  }
  else if (mapY < -1){
    servoPosY--;
    servoY.write(servoPosY);
    delay((abs(mapY) + 10) * 6);
  }

  delay(10);
}
