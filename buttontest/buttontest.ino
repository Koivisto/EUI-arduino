/*
 * Alerts when one of the button is pressed more than 10 seconds
 */

// Define digital outputs for the buttons
const int btnPin3 = 3;
const int btnPin4 = 4;
const int btnPin5 = 5;

// Timer variables
unsigned long timeStill;
unsigned long lastTime1, newTime1;
unsigned long lastTime2, newTime2;
unsigned long lastTime3, newTime3;

// Store button readings 
int reading3, reading4, reading5;

// Called once when the code is run
void setup() {

 // Set pin modes for the used digital pins
  pinMode(btnPin3, INPUT);
  pinMode(btnPin4, INPUT);
  pinMode(btnPin5, INPUT);
  
  Serial.begin(9600);
}

void checkImmobility (long lastMoveTime) {
  timeStill = millis() - lastMoveTime;
  Serial.println(timeStill/1000);
  if (timeStill > 10000){
    Serial.println("MOVE 1! YOU HAVE BEEN STILL FOR TOO LONG.");
  }
}

// This method is called continuously by the Arduino
void loop() {

 // Read the button inputs
  reading3 = digitalRead(btnPin3);
  reading4 = digitalRead(btnPin4);
  reading5 = digitalRead(btnPin5);

  //
  if (reading3 == HIGH) {
    checkImmobility(lastTime1);
  }
  else {
     // Store the time the button was let go
     lastTime1 = millis();  
  }

  if (reading4 == HIGH) {
    checkImmobility(lastTime2);
  }
  else {
   lastTime2 = millis();  
  }

  if (reading5 == HIGH) {
    checkImmobility(lastTime3);
  }
  else {
     lastTime3 = millis();  
  }

}
