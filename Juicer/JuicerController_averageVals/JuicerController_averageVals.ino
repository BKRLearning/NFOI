#define JUICER_LEFT 18

bool juicerPressed = false;
bool juicerReleased = false;

int baselineLeft;
int currentValLeft;
int previousValLeft;

int pushedVals[10];
int readIndex = 0;
int total = 0;
int average = 0;

long startMillisLeft;
long currentMillisLeft;

const long period = 10;

void setup() {
  Serial.begin(9600);

  startMillisLeft = millis();

  for (int i = 0; i < 10; i++) {
    pushedVals[i] = 0;
  }

  // LED output
  pinMode(4, OUTPUT);

  // Get juicer baseline readings
  baselineLeft = analogRead(JUICER_LEFT);

  Serial.print("Baseline Left:");
  Serial.print(baselineLeft);
  Serial.println();

}

void loop() {

  currentMillisLeft = millis();

  if (currentMillisLeft - startMillisLeft >= period) {
    previousValLeft = currentValLeft;
    currentValLeft = analogRead(JUICER_LEFT);

    if (juicerPressed) {
      total = total - pushedVals[readIndex];
      pushedVals[readIndex] = currentValLeft;  //
      total = total + pushedVals[readIndex];
      readIndex++;

      if (readIndex >= 10) {
        readIndex = 0;
      }

      average = total / 10;
      Serial.print("avg: ");
      Serial.print(average);
      Serial.println();
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
    Serial.println("LEFT");


    for (int i = 0; i < 10; i++) {
      pushedVals[i] = 0;
    }

    Serial.println("We got here");
    average = 0;

    juicerPressed = false;
    juicerReleased = false;
  }

}
