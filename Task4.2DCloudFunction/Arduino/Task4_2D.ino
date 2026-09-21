const int livingRoomLED = 2;
const int bathroomLED = 3;
const int closetLED = 4;

bool livingRoomState = false;
bool bathroomState = false;
bool closetState = false;

void setup() {
  pinMode(livingRoomLED, OUTPUT);
  pinMode(bathroomLED, OUTPUT);
  pinMode(closetLED, OUTPUT);

  digitalWrite(livingRoomLED, LOW);
  digitalWrite(bathroomLED, LOW);
  digitalWrite(closetLED, LOW);

  Serial.begin(9600);
}

void loop() {
  // Arduino Cloud handles communication.
}

// Function required for the task
void toggleLight(String room) {

  if (room == "living room") {

    livingRoomState = !livingRoomState;
    digitalWrite(livingRoomLED, livingRoomState);

    Serial.print("Living room: ");
    Serial.println(livingRoomState ? "ON" : "OFF");
  }

  else if (room == "bathroom") {

    bathroomState = !bathroomState;
    digitalWrite(bathroomLED, bathroomState);

    Serial.print("Bathroom: ");
    Serial.println(bathroomState ? "ON" : "OFF");
  }

  else if (room == "closet") {

    closetState = !closetState;
    digitalWrite(closetLED, closetState);

    Serial.print("Closet: ");
    Serial.println(closetState ? "ON" : "OFF");
  }
}