//==================================================================================

int buttonRows = 3;
int buttonsPerRow = 4;
int buttons  = buttonRows * buttonsPerRow;
// Button area width in mm 
int buttonWidth = 20;
// Button area height in mm 
int buttonHeight = 20;

// Button x offsets in mm from platform top left corner
const int btnsOffsetsX[12] = {58, 140, 218, 305, 55, 113, 230, 307, 79, 128, 240, 288};
// Button y offsets in mm from platform top left corner
const int btnsOffsetsY[12] = {38, 38, 33, 32, 125, 120, 120, 125, 232, 234, 236, 235}; 

// Print center of mass into Serial (NOTE: Breaks the visualization in Processing if set to true!)
const bool PRINT_CENTER_OF_MASS = false;

//==================================================================================

// Define button input pins in arduino
const int btnsInput[12] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
// Initialize button states
boolean btnsState[12] = {false, false, false, false, false, false, false, false, false, false, false, false};
// Initialize button temporal storages
// Initialize counter on how long a button has been in the same state
long btnsSameStateMs[12] = {0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L};
// Initialize timestamp when last change happened
long btnsLastStateChangeMs[12] = {0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L};
// Initialize counter how long sitting session has been
long lastStandMs, sinceLastStandMs, sincePauseStart = 0L;


// Called once when the code is run
void setup() {

 // Set pin modes for the used digital pins
  for (int i = 0; i <= buttons-1; i++){
    pinMode(btnsInput[i], INPUT);
    digitalWrite(btnsInput[i], HIGH);
  }

  Serial.begin(9600);
}

//==================================================================================

/**
 * Calculates the x-position of the center of a button.
 * @param col column number of the button, starting from 1.
 * @return    the x-position of the center of the button in the given column. 
 */
int getCenterXPosition (int btnIndex) {
  int posX = 0;
  posX = btnsOffsetsX[btnIndex] + (buttonWidth/2);
  
  return posX;
}

/**
 * Calculates the y-position of the center of a button.
 * @param row row number of the button, starting from 1.
 * @return    the y-position of the center of the button in the given row. 
 */
int getCenterYPosition (int btnIndex) {
  int posY = 0;
  posY = btnsOffsetsY[btnIndex] + (buttonHeight/2);
  
  return posY;
}

/**
 * Calculates the x-position of the center of the mass, i.e., the active buttons.
 * @return    the x-position of the center of the mass, 
 *            or 0 if indeterminable.
 */
int getCenterOfMassX () {
  int sumX = 0;
  int count = 0;
  
  for (int i = 0; i < buttons; i++) {
    if (btnsState[i] == true) {
      sumX += getCenterXPosition(i);
      count += 1;
    }
  }
  
  if (count == 0) {
    // Avoid division by 0
    return 0;
  }
  
  return sumX / count;
}

/**
 * Calculates the y-position of the center of the mass, i.e., the active buttons.
 * @return    the y-position of the center of the mass, 
 *            or 0 if indeterminable.
 */
int getCenterOfMassY () {
  int sumY = 0;
  int count = 0;
  
  for (int i = 0; i < buttons; i++) {
    if (btnsState[i] == true) {
      sumY += getCenterYPosition(i);
      count += 1;
    }
  }
  
  if (count == 0) {
    // Avoid division by 0
    return 0;
  }
  
  return sumY / count;
}

//==================================================================================

void printI(){
  Serial.print("1 ");
}

void printO(){
  Serial.print("0 ");
}

// Calculates how long it has been since movement happened, print warning if over 10 sec
void checkImmobility () {

  //Init latest movement to 60 seconds
  long latestMovementMs = 60000L;
  // Update new values to btnsSameStateMs, find 
  for (int i = 0; i <= buttons-1; i++){
    btnsSameStateMs[i] = millis() - btnsLastStateChangeMs[i];
    if (btnsSameStateMs[i] < latestMovementMs) {latestMovementMs = btnsSameStateMs[i];}
  }

  // If all movements have been since 10 seconds has passed, print warning
  if (latestMovementMs > 10000 && isPressed()){
    Serial.println("MOVE! YOU HAVE BEEN STILL FOR TOO LONG.");
  }
}

// Returns 1 if any of the sensors is pressed, else 0
int isPressed () {
  int isPressed = 0;
  for (int i = 0; i <= buttons-1; i++){
    if (btnsState[i]) {isPressed = 1;}
  }
  return isPressed;
}

// Makes sure that last sensor row has some activation or prints warning
void checkLastRow() {
  int isLastRowPressed = 0;
  for (int i = 0; i <= 3; i++){
    if (btnsState[i]) {isLastRowPressed = 1;}
  }

  // If front is pressed, but the last row is not
  if (isPressed() && !isLastRowPressed){
    Serial.println("FIX POSTURE! PUT YOUR BACK TO THE CHAIR.");
  }
}

/*
 * Check that regular pauses from sittings are kept 
 * Gives warning if last standing pause has been over 60 seconds ago
 * Counts down from 10 seconds for minimum pause duration
 */
void checkLastPause() {
  if (isPressed()) {
    sinceLastStandMs = millis() - lastStandMs;
    sincePauseStart = millis(); //updated here until pause really starts
  }
  else {
    lastStandMs = millis(); //updated here until sitting really starts
    sincePauseStart = millis() - sinceLastStandMs; // unintuitive naming for this
  }

  // Gives warning if last standing pause has been over 60 seconds ago
  if (sinceLastStandMs > 60000 && isPressed()){
    Serial.println("STAND UP! YOU HAVE BEEN SITTING FOR TOO LONG.");
  }
  // Counts down from 10 seconds for minimum pause duration
  else if (sincePauseStart > 0 && sincePauseStart <= 10000) {
    Serial.print("STAY AWAY FOR " );
    Serial.print((10000 - sincePauseStart)/1000);
    Serial.println(" SECONDS" );
    // Warns if violated
    if (isPressed()) {Serial.println("I SEE YOU SITTING!" );}
  }
  //else chair is abandoned and feels lonely. It no longer wants to be lonely.
  else if (!isPressed()){
    Serial.println("SIT DOWN!");
  }
}

// This method is called continuously by the Arduino
void loop() {

  // Read the button inputs, if state has changed update also btnsLastStateChangeMs array
  for (int i = 0; i <= buttons-1; i++){
    boolean fetchedState = false;
    if (digitalRead(btnsInput[i]) == LOW){
      fetchedState = true;
    } 
    // Update button state to new state if not the same as before. 
    if (fetchedState != btnsState[i]) {
      btnsState[i] = fetchedState; 
      // Update timestamp btnsLastStateChangeMs
      btnsLastStateChangeMs[i] = millis();
    }
  }

  // Print output stream into Serial
  for (int i = 0; i <= 11; i++){
    if (btnsState[i]){
      printI();
    }
    else {
      printO();
    }
  }
  // Print end of sequence
  Serial.println("");

  // Check various conditions, print warnings if needed
  //checkImmobility();
  //checkLastRow();
  //checkLastPause();

  //==================================================================================

  // Get centre x- and y-position of the center of the mass from top left corner
  int centerOfMassX = getCenterOfMassX();
  int centerOfMassY = getCenterOfMassY();

  // Print calculated center of mass in format [x,y] (in millimeters from top left corner)
  if (PRINT_CENTER_OF_MASS) {
    Serial.print("[");
    Serial.print(centerOfMassX);
    Serial.print(",");
    Serial.print(centerOfMassY);
    Serial.println("]");
  }
  
  //==================================================================================
}
