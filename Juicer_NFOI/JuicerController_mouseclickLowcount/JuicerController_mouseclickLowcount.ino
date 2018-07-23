#include <Bounce.h> // mouse functionality
Bounce space = Bounce(4, 10);

bool isPressed;

int baseline;

void setup() {
  Serial.begin(9600);
  pinMode(4, OUTPUT); // LED
  baseline = analogRead(18); // initial reading from CT Sensor
  Serial.print(baseline);
}

void loop() {
  space.update();

  if (abs(baseline - analogRead(18)) > 10 ) { // when sensor reading exceeds initial reading by +/- 10,
    digitalWrite(4, HIGH); // turn on LED
    isPressed = true;
    delay(2);
    Serial.println(abs(baseline - analogRead(18)));
  } else {
    if (isPressed) {
      // mouse click here
      Serial.println("Mouse clicked");
      Mouse.click();
      isPressed = false;
      delay(35);
    }
    digitalWrite(4, LOW);
  }
}


