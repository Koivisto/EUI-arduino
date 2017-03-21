/*
 * Alerts when one of the button is pressed more than 10 seconds
 */

// Define digital outputs for the buttons
//constant is for program
//value is from the arduino
const int btnPin2 = 2;
const int btnPin3 = 3;
const int btnPin4 = 4;
const int btnPin5 = 5;
const int btnPin6 = 6;
const int btnPin7 = 7;

// Store button readings 
int reading2, reading3, reading4, reading5, reading6, reading7;

// Called once when the code is run
void setup() {

 // Set pin modes for the used digital pins
  pinMode(btnPin2, INPUT);
  pinMode(btnPin3, INPUT);
  pinMode(btnPin4, INPUT);
  pinMode(btnPin5, INPUT);
  pinMode(btnPin6, INPUT);
  pinMode(btnPin7, INPUT);
  
  Serial.begin(9600);
}


// This method is called continuously by the Arduino
void loop() {

  

 // Read the button inputs
  reading2 = digitalRead(btnPin2);
  reading3 = digitalRead(btnPin3);
  reading4 = digitalRead(btnPin4);
  reading5 = digitalRead(btnPin5);
  reading6 = digitalRead(btnPin6);
  reading7 = digitalRead(btnPin7);

  // Everytime this button is pressed
  if (reading2 == HIGH) {
    Serial.print(" 2 is pressed ");
  }
  else {
     Serial.print(" 2 __________ ");
  }
  if (reading3 == HIGH) {
    Serial.print(" 3 is pressed ");
  }
  else {
     Serial.print(" 3 __________ ");
  }
  if (reading4 == HIGH) {
    Serial.print(" 4 is pressed ");
  }
  else {
     Serial.print(" 4 __________ ");
  }
  if (reading5 == HIGH) {
    Serial.print(" 5 is pressed ");
  }
  else {
     Serial.print(" 5 __________ ");
  }
  if (reading6 == HIGH) {
    Serial.print(" 6 is pressed ");
  }
  else {
     Serial.print(" 6 __________ ");
  }
  if (reading7 == HIGH) {
    Serial.print(" 7 is pressed ");
  }
  else {
     Serial.print(" 7 __________ ");
  }
  Serial.println(" </end>");
}
