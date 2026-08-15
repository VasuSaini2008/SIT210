// SIT210 Task 1.1P
const int buttonpin = 2; //The button is connected to D2.
const int porchpin = 3; // The porchpin is connected to D3.
const in hallwaypin = 4; //The halwaypin is connected to D4.

void setup() //This run when the ardiuno start.
{
  pinMode (buttonpin, input); //Its tells that the button sends information to ardiuno.
  pinMode (porchpin, output); //Its tells that the ardiuno is controlled by ardiuno.
  pinMode (hallwaypin, output); //Its tells that the hallwapin is controlled by ardiuno.
}

void loop()
{

  int buttonState = digitalRead(buttonPin); //Read the current state of the push button

  if (buttonState == HIGH) // Check whether the button has been pressed
  {
    // Turn ON both lights
    digitalWrite(porchPin, HIGH);
    digitalWrite(hallwayPin, HIGH);

    delay(30000); //Keep both lights ON for 30 seconds

    digitalWrite(porchPin, LOW); //Turn OFF the porch light after 30 seconds

    delay(30000); //Keep the hallway light ON for another 30 seconds
  
    digitalWrite(hallwayPin, LOW); //Turn OFF the hallway light after a total of 60 seconds
  }

