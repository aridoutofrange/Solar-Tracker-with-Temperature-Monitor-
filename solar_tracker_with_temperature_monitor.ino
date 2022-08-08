//Importing Libraries
#include <Servo.h>

#include <LiquidCrystal.h>

//Initializing LEDs
#define ledRed 4
#define ledGreen 2

//Initializing slide switch
#define slideSwitch 13

//Initializing The Horizontal Servo (Y-Axis)
Servo servoYAxis;
int servoY = 0;
int servoYMax = 160;
int servoYMin = 20;

//Initializing The Vertical Servo (X-Axis)
Servo servoXAxis;
int servoX = 0;
int servoXMax = 160;
int servoXMin = 20;

//Initializing Lights' Directions
int ldrNorth = A0;
int ldrSouth = A1;
int ldrEast = A2;
int ldrWest = A3;

LiquidCrystal lcd(12, 11, 10, 9, 8, 7); //Initializing pins used by LCD
float value;
int tmp = A4;
int buzzer = 6;

void setup() {
  pinMode(tmp, INPUT);
  lcd.begin(16, 2);
  pinMode(buzzer, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(slideSwitch, INPUT);

  servoXAxis.attach(5);
  servoXAxis.write(0);
  Serial.begin(9600);
  servoYAxis.attach(3);
  servoYAxis.write(0);
  delay(1000);

}

void loop() {
  int state = digitalRead(slideSwitch);
  value = (value - 0.5) * 100.0;
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.print(value);
  delay(90);
  lcd.clear();

  if (value > 85 && state == 0) { //Maximum temperature that most solar panels tolerate
    lcd.setCursor(0, 2);
    lcd.print("Alert! High Temp!");
    tone(buzzer, 500, 700);
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
  } else if (value < 85 && state == 0) {
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, HIGH);
  } else if (state == 1) {
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
  }

  servoX = servoXAxis.read();
  servoY = servoYAxis.read();

  int east = analogRead(ldrEast);
  int south = analogRead(ldrSouth);
  int west = analogRead(ldrWest);
  int north = analogRead(ldrNorth);
  value = analogRead(tmp) * 0.004882814;

  int avgNorth = (east + south) / 2;
  int avgSouth = (west + north) / 2;
  int avgWest = (east + west) / 2;
  int avgEast = (south + north) / 2;

  if (avgNorth < avgSouth) {
    servoYAxis.write(servoY + 1);
    if (servoY > servoYMax) {
      servoY = servoYMax;
    }
    delay(5);
  } else if (avgSouth < avgNorth) {
    servoYAxis.write(servoY - 1);

    if (servoY < servoYMin) {
      servoY = servoYMin;
    }
    delay(5);
  } else {
    servoYAxis.write(servoY);
  }
  delay(5);

  if (avgWest > avgEast) {
    servoXAxis.write(servoX + 1);

    if (servoX > servoXMax) {
      servoX = servoXMax;
    }
    delay(5);
  } else if (avgEast > avgWest) {
    servoXAxis.write(servoX - 1);

    if (servoX < servoXMin) {
      servoX = servoXMin;
    }
    delay(5);
  } else {
    servoXAxis.write(servoX);
  }
  delay(10);
}