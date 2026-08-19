int RedLed = 8;   //pin for Red LED
int BlueLed = 6;  // pin for Blue LED
int BUTTON = 2;   //pin for Button

unsigned long startTime; //creates a variable and will start the timer when button is pressed

void setup() { //This code will run only once
  pinMode(RedLed, OUTPUT); //sets RedLed as the output as ardiuno controls it
  pinMode(BlueLed, OUTPUT); //sets BlueLed as the output as arduino controls it
  pinMode(BUTTON, INPUT_PULLUP); //sets Button as the input as arduino reads it

  digitalWrite(RedLed, LOW); //RedLed will be OFF at start
  digitalWrite(BlueLed, LOW); //BlueLed will be OFF at start
}

void loop() { //this code will repeat in loop

  if (digitalRead(BUTTON) == LOW) { //when the button is clicked
    digitalWrite(RedLed, HIGH); //turn the RedLed ON+
    digitalWrite(BlueLed, HIGH); //turn the BlueLed ON
    startTime = millis(); //start the timer

    while (millis() - startTime < 30000) { //LEDs/porch light stays ON for 30 seconds
    }

    digitalWrite(RedLed, LOW); //turn OFF RedLed after 30 seconds

    while (millis() - startTime < 60000) { //BlueLed/hallway light stay ON for 60 seconds
    }

    digitalWrite(BlueLed, LOW); //turn OFF BlueLed after 60 seconds
  }
}
