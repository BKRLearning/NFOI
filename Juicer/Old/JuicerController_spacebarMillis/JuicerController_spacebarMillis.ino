#include <Bounce.h> // mouse functionality
Bounce space = Bounce(4, 10);

bool isPressed;

int baseline;
int currentVal;

long startMillis;
long currentMillis;
const long period = 5;

void setup() {
  Serial.begin(9600);
  startMillis = millis();
  pinMode(4, OUTPUT); // LED
  baseline = analogRead(18);
  Serial.print(baseline);
}

void loop() {
  space.update();

  currentMillis = millis();

  if (currentMillis - startMillis >= period) {
    currentVal = analogRead(18);
    startMillis = currentMillis;
  }

  if (abs(baseline - currentVal) > 30 ) {
    digitalWrite(4, HIGH); // turn on LED
    isPressed = true;
    delay(2);
    Serial.println(abs(baseline - currentVal));
  } else {
    if (isPressed) {
      Keyboard.press(KEY_SPACE);
      delay(30);
      Keyboard.release(KEY_SPACE);
      isPressed = false;
      delay(35);
    }
    digitalWrite(4, LOW);
  }
}


