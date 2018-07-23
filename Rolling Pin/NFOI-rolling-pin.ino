#include <Encoder.h>
#include <Bounce.h>

const int buttonPin = 12;

Bounce pushbutton = Bounce(buttonPin, 5);  // 5ms debounce
byte buttonState, previousState;

Encoder knob(6, 7);
long currentPos, secondPos;

long startMillis;
long currentMillis;
const long period = 10; // time length (ms)

void setup() {
  Serial.begin(9600);
  startMillis = millis();

  pinMode(buttonPin, INPUT_PULLUP);

  currentPos = knob.read();
  secondPos = currentPos - 1;

  buttonState = digitalRead(buttonPin);
  previousState = HIGH;
}

void loop() {
  pushbutton.update();

  currentMillis = millis();
  if (currentMillis - startMillis >= period)
  {
    currentPos = knob.read();
    startMillis = currentMillis;
  }

  if (pushbutton.fallingEdge()) {    // button was pressed
    Serial.println("button pressed");
    Keyboard.press(KEY_A);
    delay(20);
    Keyboard.release(KEY_A);
  }

  if (currentPos != secondPos) {      // knob has been turned
    if (currentPos > secondPos) {     // moving forward
      Serial.println("Moving forward...");
      Keyboard.press(KEY_RIGHT);
    } else {                          // moving backwards
      Serial.println("Moving backward...");
      Keyboard.press(KEY_LEFT);
    }
    secondPos = currentPos;

  } else {
    delay(100);
    Keyboard.release(KEY_LEFT);
    Keyboard.release(KEY_RIGHT);
  }
}
