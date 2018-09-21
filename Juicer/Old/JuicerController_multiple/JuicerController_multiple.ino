#define JUICER_LEFT 18
#define JUICER_RIGHT 17
#define JUICER_UP 16
#define JUICER_DOWN 15

bool isPressed;

int baselineLeft;
int currentValLeft;
long startMillisLeft;
long currentMillisLeft;

int baselineRight;
int currentValRight;
long startMillisRight;
long currentMillisRight;

int baselineUp;
int currentValUp;
long startMillisUp;
long currentMillisUp;

int baselineDown;
int currentValDown;
long startMillisDown;
long currentMillisDown;

const long period = 5;

void setup() {
  Serial.begin(9600);

  startMillisLeft = millis();
  startMillisRight = millis();
  startMillisUp = millis();
  startMillisDown = millis();


  // LED output
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // Get juicer baseline readings
  baselineLeft = analogRead(JUICER_LEFT);
  baselineRight = analogRead(JUICER_RIGHT);
  baselineUp = analogRead(JUICER_UP);
  baselineDown = analogRead(JUICER_DOWN);

  Serial.print("Left:");
  Serial.print(baselineLeft);
  Serial.print(", Right: ");
  Serial.print(baselineRight);
  Serial.print(", Up: ");
  Serial.print(baselineUp);
  Serial.print(", Down: ");
  Serial.print(baselineDown);
  Serial.println(" ");
}

void loop() {
  
  //
  // Left
  //
  currentMillisLeft = millis();

  if (currentMillisLeft - startMillisLeft >= period) {
    currentValLeft = analogRead(JUICER_LEFT);
    startMillisLeft = currentMillisLeft;
  }

  if (abs(baselineLeft - currentValLeft) > 30 ) {
    digitalWrite(4, HIGH); // turn on LED
    isPressed = true;
    delay(2);
    Serial.println(abs(baselineLeft - currentValLeft));
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


  //
  // Right
  //
  currentMillisRight = millis();

  if (currentMillisRight - startMillisRight >= period) {
    currentValRight = analogRead(17);
    startMillisRight = currentMillisRight;
  }

  if (abs(baselineRight - currentValRight) > 30 ) {
    digitalWrite(5, HIGH); // turn on LED
    isPressed = true;
    delay(2);
    Serial.println(abs(baselineRight - currentValRight));
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
  
  
  //
  // Up
  //
  currentMillisUp = millis();

  if (currentMillisUp - startMillisUp >= period) {
    currentValUp = analogRead(16);
    startMillisUp = currentMillisUp;
  }

  if (abs(baselineUp - currentValUp) > 30 ) {
    digitalWrite(6, HIGH); // turn on LED
    isPressed = true;
    delay(2);
    Serial.println(abs(baselineUp - currentValUp));
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

  
  //
  // Down
  //
  currentMillisDown = millis();

  if (currentMillisDown - startMillisDown >= period) {
    currentValDown = analogRead(15);
    startMillisDown = currentMillisDown;
  }

  if (abs(baselineDown - currentValDown) > 30 ) {
    digitalWrite(7, HIGH); // turn on LED
    isPressed = true;
    delay(2);
    Serial.println(abs(baselineDown - currentValDown));
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
  }

}



