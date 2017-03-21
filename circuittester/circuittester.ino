/*
 * Alerts when one of the button is pressed more than 10 seconds
 */

// Define digital outputs for the buttons
//constant is for program
//value is from the arduino
const int btnPin2 = 2;
const int btnPin3 = 3;
const int btnPin4 = 4;

// Timer variables
unsigned long timeStill;
unsigned long lastTime1, newTime1;
unsigned long lastTime2, newTime2;
unsigned long lastTime3, newTime3;

// Store button readings 
int reading2, reading3, reading4;

// Called once when the code is run
void setup() {

 // Set pin modes for the used digital pins
  pinMode(btnPin2, INPUT);
  pinMode(btnPin3, INPUT);
  pinMode(btnPin4, INPUT);
  
  Serial.begin(9600);
}


// This method is called continuously by the Arduino
void loop() {

  

 // Read the button inputs
  reading2 = digitalRead(btnPin2);
  reading3 = digitalRead(btnPin3);
  reading4 = digitalRead(btnPin4);

  // Everytime this button is pressed
  if (reading2 == HIGH) {
    Serial.print(" 2 is working ");
  }
  else {
     Serial.print(" 2 ***fail*** ");
  }
  if (reading3 == HIGH) {
    Serial.print(" 3 is working ");
  }
  else {
     Serial.print(" 3 ***fail*** ");
  }
  if (reading4 == HIGH) {
    Serial.print(" 4 is working ");
  }
  else {
     Serial.println(" 4 ***fail*** ");
  }

}
