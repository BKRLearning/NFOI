#include <Bounce.h> // mouse functionality
Bounce space = Bounce(4, 10);


bool isPressed;

int baseline0;
int currentVal0;
long startMillis0;
long currentMillis0;

int baseline1;
int currentVal1;
long startMillis1;
long currentMillis1;

int baseline2;
int currentVal2;
long startMillis2;
long currentMillis2;

int baseline3;
int currentVal3;
long startMillis3;
long currentMillis3;

const long period = 5;

void setup() {
  Serial.begin(9600);
  startMillis0 = millis();
  startMillis1 = millis();
  startMillis2 = millis();
  startMillis3 = millis();


  pinMode(4, OUTPUT); // LED
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  baseline0 = analogRead(18);
  baseline1 = analogRead(17);
  baseline2 = analogRead(16);
  baseline3 = analogRead(15);

  Serial.print("Left:");
  Serial.print(baseline0);
  Serial.print(", Right: ");
  Serial.print(baseline1);
  Serial.print(", Up: ");
  Serial.print(baseline2);
  Serial.print(", Down: ");
  Serial.print(baseline3);
  Serial.println(" ");
}

void loop() {
  space.update();

  //Left (0)
  currentMillis0 = millis();

  if (currentMillis0 - startMillis0 >= period) {
    currentVal0 = analogRead(18);
    startMillis0 = currentMillis0;
  }

  if (abs(baseline0 - currentVal0) > 30 ) {
    digitalWrite(4, HIGH); // turn on LED
    isPressed = true;
    delay(2);
    Serial.println(abs(baseline0 - currentVal0));
  } else {
    if (isPressed) {
      Keyboard.press(KEY_LEFT);
      delay(30);
      Keyboard.release(KEY_LEFT);
      isPressed = false;
      Serial.println("LEFT");
      delay(35);
    }
    digitalWrite(4, LOW);
  }

  //Right (1)
  currentMillis1 = millis();

  if (currentMillis1 - startMillis1 >= period) {
    currentVal1 = analogRead(17);
    startMillis1 = currentMillis1;
  }

  if (abs(baseline1 - currentVal1) > 30 ) {
    digitalWrite(5, HIGH); // turn on LED
    isPressed = true;
    delay(2);
    Serial.println(abs(baseline1 - currentVal1));
  } else {
    if (isPressed) {
      Keyboard.press(KEY_RIGHT);
      delay(30);
      Keyboard.release(KEY_RIGHT);
      isPressed = false;
      Serial.println("RIGHT");
      delay(35);
    }
    digitalWrite(5, LOW);
  }

  //Up (2)
  currentMillis2 = millis();

  if (currentMillis2 - startMillis2 >= period) {
    currentVal2 = analogRead(16);
    startMillis2 = currentMillis2;
  }

  if (abs(baseline2 - currentVal2) > 30 ) {
    digitalWrite(6, HIGH); // turn on LED
    isPressed = true;
    delay(2);
    Serial.println(abs(baseline2 - currentVal2));
  } else {
    if (isPressed) {
      Keyboard.press(KEY_UP);
      delay(30);
      Keyboard.release(KEY_UP);
      isPressed = false;
      Serial.println("UP");
      delay(35);
    }
    digitalWrite(6, LOW);
  }
/*
  //Down (3)
  currentMillis3 = millis();

  if (currentMillis3 - startMillis3 >= period) {
    currentVal3 = analogRead(15);
    startMillis3 = currentMillis3;
  }

  if (abs(baseline3 - currentVal3) > 30 ) {
    digitalWrite(7, HIGH); // turn on LED
    isPressed = true;
    delay(2);
    Serial.println(abs(baseline3 - currentVal3));
  } else {
    if (isPressed) {
      Keyboard.press(KEY_DOWN);
      delay(30);
      Keyboard.release(KEY_DOWN);
      isPressed = false;
      Serial.println("DOWN");
      delay(35);
    }
    digitalWrite(7, LOW);
  }*/
  
}


