/*
 * Track and draw 12 button states
 */

// Define button input pins in arduino
const int btnsInput[12] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
// Initialize button states
boolean btnsState[12] = {false, false, false, false, false, false, false, false, false, false, false, false};
// Initialize button temporal storages
// Initialize counter on how long a button has been in the same state
long btnsSameStateMs[12] = {0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L};
// Initialize timestamp when last change happened
long btnsLastStateChangeMs[12] = {0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L};

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

// Calculates how long it has been since movement happened, print warning if over 10 sec
void checkImmobility () {

  //Init latest movement to 60 seconds
  long latestMovementMs = 60000L;
  // Update new values to btnsSameStateMs, find 
  for (int i = 0; i <= 11; i++){
    btnsSameStateMs[i] = millis() - btnsLastStateChangeMs[i];
    if (btnsSameStateMs[i] < latestMovementMs) {latestMovementMs = btnsSameStateMs[i]}
  }

  // If all movements have been since 10 seconds has passed, print warning
  if (latestMovementMs > 10000 && isPressed()){
    Serial.println("MOVE! YOU HAVE BEEN STILL FOR TOO LONG.");
  }
}

// Returns 1 if any of the sensors is pressed, else 0
int isPressed () {
  int isPressed = 0;
  for (int i = 0; i <= 11; i++){
    if (btnsState[i]) {isPressed = 1;}
  }
  return isPressed;
}

// This method is called continuously by the Arduino
void loop() {

  // Read the button inputs, if state has changed update also btnsLastStateChangeMs array
  for (int i = 0; i <= 11; i++){
    boolean fetchedState = if (digitalRead(btnsInput[i]) == HIGH){return true;} else {return false;};
    // Update button state to new state if not the same as before. 
    if (fetchedState != btnsState[i]) {
      btnsState[i] = fetchedState; 
      // Update timestamp btnsLastStateChangeMs
      btnsLastStateChangeMs[i] = millis();
    }
  }

  checkImmobility();

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
