#include <Bounce.h> // mouse functionality
Bounce space = Bounce(4, 10);

bool isPressed;

int baseline;
int currentVal;

long startMillis;
long currentMillis;
const long period = 20;

void setup() {
  Serial.begin(9600);
  startMillis = millis();
  pinMode(4, OUTPUT); // LED
  baseline = analogRead(18); 
  Serial.println("Baseline: " + baseline);
}

void loop() {
  space.update();

  currentMillis = millis();

  if (currentMillis - startMillis >= period) {
    currentVal = analogRead(18);
    startMillis = currentMillis;
  }

  if (abs(baseline - currentVal) > 20 ) { 
    digitalWrite(4, HIGH); // turn on LED
    isPressed = true;
    delay(10);
    Serial.print("Baseline: ");
    Serial.print(baseline);
    Serial.println();
    Serial.print("Current: ");
    Serial.print(currentVal);
    Serial.println();
    Serial.println(abs(baseline - currentVal));

  } else {
    if (isPressed) {
      Serial.println("Mouse clicked");
      // mouse click here
      Mouse.click();
      isPressed = false;
      delay(30);
    }
    digitalWrite(4, LOW);
  }
}


