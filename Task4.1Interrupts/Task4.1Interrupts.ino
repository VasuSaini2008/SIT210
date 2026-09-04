const int pirPin = 2;
const int switchPin = 3;

const int led1 = 8;
const int led2 = 9;

volatile bool motionDetected = false;
volatile bool switchActivated = false;

void motionISR() {
  motionDetected = true;
}

void switchISR() {
  switchActivated = true;
}

void setup() {
  Serial.begin(9600);

  pinMode(pirPin, INPUT);
  pinMode(switchPin, INPUT_PULLUP);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(pirPin),
                  motionISR, RISING);

  attachInterrupt(digitalPinToInterrupt(switchPin),
                  switchISR, FALLING);

  Serial.println("Task 4.1P - Interrupt System Started");
}

void loop() {

  if (motionDetected) {
    motionDetected = false;

    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);

    Serial.println("Motion detected - Lights ON");
  }

  if (switchActivated) {
    switchActivated = false;

    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);

    Serial.println("Slider switch activated - Lights ON");
  }
}
