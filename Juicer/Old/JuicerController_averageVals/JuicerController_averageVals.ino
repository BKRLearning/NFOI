#define JUICER_LEFT 18

bool juicerPressed = false;
bool juicerReleased = false;

int baselineLeft;
int currentValLeft;
int previousValLeft;

int const arraySize = 6;
int pushedVals[arraySize];
int readIndex = 0;
int total = 0;
int average = 0;

long startMillisLeft;
long currentMillisLeft;

const long period = 7;

void setup() {
  Serial.begin(9600);

  startMillisLeft = millis();

  for (int i = 0; i < arraySize; i++) {
    pushedVals[i] = 0;
  }

  // LED output
  pinMode(4, OUTPUT);

  // Get juicer baseline readings
  baselineLeft = analogRead(JUICER_LEFT);
  Serial.print("Baseline Left:");
  Serial.print(baselineLeft);
  Serial.println();

  delay(1);
}

void loop() {

  currentMillisLeft = millis();

  if (currentMillisLeft - startMillisLeft >= period) {
    previousValLeft = currentValLeft;
    currentValLeft = analogRead(JUICER_LEFT);

    if (juicerPressed) {
      total = total - pushedVals[readIndex];
      pushedVals[readIndex] = currentValLeft;
      total = total + pushedVals[readIndex];
      readIndex++;

      if (readIndex >= arraySize) {
        readIndex = 0;
      }

      average = total / arraySize;
      delay(2);

      if (abs(average - baselineLeft) <= 2) {
        juicerReleased = true;
        juicerPressed = false;
      }
    }

    startMillisLeft = currentMillisLeft;
  }


  if (!juicerReleased && abs(previousValLeft - currentValLeft) >= 5) { // user is actively pressing on juicer
    juicerPressed = true;
  }

  if (!juicerPressed && juicerReleased) {
    Serial.println("Mouse click");
    Mouse.click();
    
    // reset array setup
    for (int i = 0; i < arraySize; i++) {
      pushedVals[i] = 0;
    }
    total = 0;

    // reset state vals
    juicerPressed = false;
    juicerReleased = false;
  }

  delay(1);
}
