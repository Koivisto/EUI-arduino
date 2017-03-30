/*
 * Track and draw 12 button states
 */

// Define button input pins in arduino
const int btnsInput[12] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
// Initialize button states
boolean btnsState[12] = {false, false, false, false, false, false, false, false, false, false, false, false};

// Called once when the code is run
void setup() {

 // Set pin modes for the used digital pins
  for (int i = 0; i <= 11; i++){
    pinMode(btnsInput[i], INPUT);
  }

  Serial.begin(9600);
}

void printI(){
  Serial.print(" X");
}
void printO(){
  Serial.print(" _");
}

// This method is called continuously by the Arduino
void loop() {

  // Read the button inputs
  for (int i = 0; i <= 11; i++){
    btnsState[i] = if (digitalRead(btnsInput[i]) == HIGH){return true;} else {return false;};
  }

  /* Print state diagram 
  *  1-4 | _ _ _ _ |
  *  5-8 | X X X _ |
  *  9-12| X X X _ |
  *  ---------------
  *  X marks pressed state. 
  */
 
  for (int i = 0; i <= 11; i++){
    if (i=0) {Serial.print("1-4 |");} //Row 1 start
    if (i=3) {Serial.println(" |");Serial.print("5-8 |");} //Row 1 end, Row 2 start
    if (i=7) {Serial.println(" |");Serial.print("9-12|");} //Row 2 end, Row 3 start
    if (btnsState[i]) {printI();} else {printO();}
    if (i=11) {Serial.print("1-4 |");Serial.println("---------------");} //Row 3 end
  }
}
