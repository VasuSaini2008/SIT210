const int pirPin = 2;
const int switchPin = 3;

const int led1 = 8;
const int led2 = 9;

volatile bool motionDetected = false;
volatile bool switchPressed = false;

bool lightsOn = false;

// PIR interrupt
void pirISR() {
  motionDetected = true;
}

// Switch interrupt
void switchISR() {
  switchPressed = true;
}

void setup() {

  Serial.begin(9600);

  pinMode(pirPin, INPUT);
  pinMode(switchPin, INPUT_PULLUP);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  // PIR interrupt
  attachInterrupt(
    digitalPinToInterrupt(pirPin),
    pirISR,
    RISING
  );

  // Button interrupt
  attachInterrupt(
    digitalPinToInterrupt(switchPin),
    switchISR,
    FALLING
  );

  Serial.println("Task 4.1P - Interrupt System Started");
  Serial.println("System ready...");
}

void loop() {

  // PIR detected movement
  if (motionDetected) {

    motionDetected = false;

    lightsOn = !lightsOn;

    digitalWrite(led1, lightsOn);
    digitalWrite(led2, lightsOn);

    if (lightsOn) {
      Serial.println("Motion detected - Lights ON");
    } else {
      Serial.println("Motion detected - Lights OFF");
    }
  }

  // Switch pressed
  if (switchPressed) {

    switchPressed = false;

    delay(100);  // button debounce

    lightsOn = !lightsOn;

    digitalWrite(led1, lightsOn);
    digitalWrite(led2, lightsOn);

    if (lightsOn) {
      Serial.println("Switch pressed - Lights ON");
    } else {
      Serial.println("Switch pressed - Lights OFF");
    }
  }
}
