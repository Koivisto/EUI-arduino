/*
 * Alerts when a button is pressed more than 10 seconds
 */

// Define digital outputs for the buttons
const int btnPin3 = 3;
const int btnPin4 = 4;
const int btnPin5 = 5;

// Timer variables
unsigned long lastTime, newTime;

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

// This method is called continuously by the Arduino
void loop() {

 // Read the button inputs
  reading3 = digitalRead(btnPin3);
  reading4 = digitalRead(btnPin4);
  reading5 = digitalRead(btnPin5);

 if (reading3 == HIGH) {
      // Increase timer if button is pressed continuously
      newTime = millis() - lastTime;

     // Print the duration in seconds
      Serial.println(newTime/1000);

     if (newTime > 10000){
        Serial.println("MOVE! YOU HAVE BEEN STILL FOR TOO LONG.");
      }
  }
  else {
     // Store the time the button was let go
     lastTime = millis();  
  }

 // Print button inputs
  //Serial.println("--------");
  //Serial.println(reading3);
  //Serial.println(reading4);
  //Serial.println(reading5);
  //Serial.println("========");
}
