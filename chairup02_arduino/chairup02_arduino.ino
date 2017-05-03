#include <Wire.h>

/*-------------------------------------------
    CONSTANTS & VARIABLES
-------------------------------------------*/


/*  
 *  Use Processing for debug.
 *  
 *  NOTE: If true, all the other printing into Serial will be disabled!
 *  
 *  IMPORTANT: Set this to false when deploying to real use!
 */
const bool PROCESSING_DEBUB_MODE = false;

/* 
    Prints the overall state including: 
      - active buttons
      - current center of balance
      - posture validity
      - center of balance offset direction
      - sitting duration

    NOTE: This will be automatically set to false, if 
    PROCESSING_DEBUB_MODE is set to true! This prevents problems 
    in reading data from Serial in the Processing visualization. In this
    case, only the button state information is printed into the Serial output.
 */
bool PRINT_STATE = true;

// Use wire transmission instead of USB Serial
bool USE_WIRE_TRANSMISSION = true;

/* 
    The delay after each loop iteration in milliseconds.
    
    NOTE: This is a very important value in determining, e.g., the 
    battery life if the Arduino is run with an external power source
    with limited power capacity.
 */ 
const int LOOP_DELAY = 100;

// In minutes
const int SITTING_TIME_THRESHOLD = 1;

// Number of buttons on board
const int buttons  = 12;

// Button area width in mm 
int buttonWidth = 20;

// Button area height in mm 
int buttonHeight = 20;

// Button x offsets in mm from platform top left corner
const int btnsOffsetsX[12] = {58, 140, 218, 305, 55, 113, 230, 307, 79, 128, 240, 288};

// Button y offsets in mm from platform top left corner
const int btnsOffsetsY[12] = {38, 38, 33, 32, 125, 120, 120, 125, 232, 234, 236, 235}; 

// Refence center of mass x offset in mm
int refX = 0;

// Refence center of mass y offset in mm
int refY = 0;

// Distance threshold in mm for determining bad posture w.r.t reference posture
const int distThreshold = 15;

// Define button input pins in arduino
const int btnsInput[12] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

// Initialize button states
boolean btnsState[12] = {false, false, false, false, false, false, false, false, false, false, false, false};

// Initialize counter on how long a button has been in the same state
long btnsSameStateMs[12] = {0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L};

// Initialize timestamp when last change happened
long btnsLastStateChangeMs[12] = {0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L};

// Initialize counter how long sitting session has been
long lastStandMs, sinceLastStandMs, sincePauseStart = 0L;





/*-------------------------------------------
    SETUP
-------------------------------------------*/

// Called once when the code is run
void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
 
  for (int i = 0; i <= buttons-1; i++){
    
    // Sets the button pin modes to input mode
    pinMode(btnsInput[i], INPUT);

    // Turns on the pull-up resistors for each pin
    digitalWrite(btnsInput[i], HIGH);
  }

  // Calculates and stores the reference center of mass
  refX = getReferenceCenterOfMassX();
  refY = getReferenceCenterOfMassY();

  // Begins serial communication at given baud rate
  Serial.begin(9600);
}





/*-------------------------------------------
    HELPER FUNCTIONS
-------------------------------------------*/


/**
 * Calculates the x-position of the center of a button.
 * @param col column number of the button, starting from 1.
 * @return    the x-position of the center of the button in the given column. 
 */
int getCenterXPosition (int btnIndex) {
  int posX = posX = btnsOffsetsX[btnIndex] + (buttonWidth/2);
  return posX;
}

/**
 * Calculates the y-position of the center of a button.
 * @param row row number of the button, starting from 1.
 * @return    the y-position of the center of the button in the given row. 
 */
int getCenterYPosition (int btnIndex) {
  int posY = posY = btnsOffsetsY[btnIndex] + (buttonHeight/2);
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
    // Avoids division by 0
    return refX;
  }
  
  return sumX / count;
}

/**
 * Calculates the x-position of the reference center of the mass, i.e., all buttons as active .
 * @return    the x-position of the center of the mass, 
 *            or 0 if indeterminable.
 */
int getReferenceCenterOfMassX() {
  int sumX = 0;
  
  for (int i = 0; i < buttons; i++) {
    sumX += getCenterXPosition(i);
  }
  
  return sumX / buttons;
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
    // Avoids division by 0
    return refY;
  }
  
  return sumY / count;
}

/**
 * Calculates the y-position of the reference center of the mass, i.e., all buttons as active .
 * @return    the y-position of the center of the mass, 
 */
int getReferenceCenterOfMassY() {
  int sumY = 0;
  
  for (int i = 0; i < buttons; i++) {
    sumY += getCenterYPosition(i);
  }
  
  return sumY / buttons;
}

/**
 * Calculates the distance of the given coordinates,  in mm,  from the position 
 * of the reference posture.
 * @return    the distance from the reference center of mass in mm. 
 */
int getDistance(int x, int y) {
  return int(sqrt(pow(refX-x, 2) +  pow(refY-y, 2)));
}

/**
 * Determines the direction of the given coordinates from the reference posture.
 * @return    the direction abbreviation of the direction from the reference center of mass.
 */
String getDirectionString(int x, int y) {
  String str = "";
  
  if ((x < refX) && (y < refY)) {
      str = "NW";
  }
  else if ((x == refX) && (y < refY)) {
      str = " N";
  }
  else if ((x > refX) && (y < refY)) {
      str = "NE";
  }  
  else if ((x > refX) && (y == refY)) {
      str = " E";
  }
  else if ((x > refX) && (y > refY)) {
      str = "SE";
  }
  else if ((x == refX) && (y > refY)) {
      str = " S";
  }
  else if ((x < refX) && (y > refY)) {
      str = "SW";
  }
  else if ((x < refX) && (y == refY)) {
      str = " W";
  }

  return str;
} 

/**
 * 
 */
String getCenterOfMassString(int centerOfMassX, int centerOfMassY) {
  String str = "";
  
  // String format is [x,y]
  str = str + "[" + centerOfMassX + "," + centerOfMassY + "]";
  
  return str;
}

/**
 * 
 */
String  getButtonStateString() {
  String str = "";

  // Print output stream into Serial
  for (int i = 0; i <= 11; i++){
    if (btnsState[i]){
      str = str + "1 ";
    }
    else {
      str = str + "0 ";
    }
  }

  return str;
}

/**
 *  Prints the actual data to the Serial. This data presents the input for the desk module is deployment mode.
 *  If the program is run in PROCESSING_DEBUB_MODE, the data includes only there relevant button state 
 *  information for the Processing visualization.
 */
void printDataToSerial (boolean isValidPosture, String directionString, int sittingDuration) {
  String str = "";
  char strr[20];

  if (PROCESSING_DEBUB_MODE) {
    str = getButtonStateString();
    Serial.println(str);
  }
  else {
    /*
        The format should be, e.g.: "FNW180"
        In this case, it would mean false posture towards northwest for 180 seconds.
     */
  
    // Adds the posture correctness character ("F" or "T")
    if (isValidPosture)  str = str + "T";
    else                 str = str + "F";
  
    // Adds the direction abbreviation (e.g., NW)
    str = str + directionString;

    if (directionString == "") {
      str = str + "  ";
    }

    if (sittingDuration >= SITTING_TIME_THRESHOLD) {
      str = str + "T";
    }
    else {
      str = str + "F";
    }
  
    // Adds the sitting duration in seconds
    str = str + sittingDuration;

    if (USE_WIRE_TRANSMISSION) {
       Wire.beginTransmission(8);
       if (!isPressed()) {
          Wire.write("   ");  
          Serial.println("   "); 
       }
       else {
          str.toCharArray(strr, 20);
          Wire.write(strr);   
          Serial.println(strr); 
       }
       Wire.endTransmission();
    }
    else {
       Serial.println(str);
    }
    
  }
}

// Calculates how long it has been since movement happened, print warning if over 10 sec
long checkImmobility () {
  

  //Init latest movement to 60 seconds
  long latestMovementMs = 60000L;
  // Update new values to btnsSameStateMs, find 
  for (int i = 0; i <= buttons-1; i++){
    btnsSameStateMs[i] = millis() - btnsLastStateChangeMs[i];
    if (btnsSameStateMs[i] < latestMovementMs) {latestMovementMs = btnsSameStateMs[i];}
  }

  // If all movements have been since 10 seconds has passed, print warning
  if (latestMovementMs > 10000 && isPressed()){
    //Serial.println("MOVE! YOU HAVE BEEN STILL FOR TOO LONG.");                              // TODO
  }

  return latestMovementMs;
}

// Returns 1 if any of the sensors is pressed, else 0
int isPressed () {
  int isPressed = 0;
  for (int i = 0; i < buttons; i++){
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
    Serial.println("FIX POSTURE! PUT YOUR BACK TO THE CHAIR.");                             // TODO
  }
}

/**
 * Checks that regular pauses from sittings are kept.
 * Gives a warning if last standing pause has been over 60 seconds ago.
 * Counts down from 10 seconds for minimum pause duration.
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
    Serial.println("STAND UP! YOU HAVE BEEN SITTING FOR TOO LONG.");                        // TODO
  }
  // Counts down from 10 seconds for minimum pause duration
  else if (sincePauseStart > 0 && sincePauseStart <= 10000) {
    Serial.print("STAY AWAY FOR " );                                                        // TODO
    Serial.print((10000 - sincePauseStart)/1000);                                           // TODO
    Serial.println(" SECONDS" );                                                            // TODO
    // Warns if violated
    if (isPressed()) {
      Serial.println("I SEE YOU SITTING!" );                                                // TODO
    }
  }
  //else chair is abandoned and feels lonely. It no longer wants to be lonely.
  else if (!isPressed()){
    Serial.println("SIT DOWN!");                                                            // TODO
  }
}





/*-------------------------------------------
    MAIN LOOP
-------------------------------------------*/

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

  // Check various conditions, print warnings if needed
  
  /*
  checkLastRow();
  checkLastPause();
  */
  int centerOfMassX, centerOfMassY, distance;
  boolean isCorrectPosture = true;
  String direction;
  long duration;
  
  // Gets the x- and y-positions of the center of the mass from top left corner
  centerOfMassX = getCenterOfMassX();
  centerOfMassY = getCenterOfMassY();


  // Get the distance from the reference center of mass
  distance = getDistance(centerOfMassX, centerOfMassY);

  // Checks if the current center of mass is within the allowed boundary from the reference position
  if (distance >= distThreshold) {
    isCorrectPosture = false;
  }

  duration = checkImmobility() / 1000 / 60;

  // Gets the direction string of the current center of mass w.r.t. the reference position
  direction = getDirectionString(centerOfMassX, centerOfMassY);

  //Serial.println(duration/1000/60);
  // Prints the data into the serial for the desk module
  printDataToSerial(isCorrectPosture, direction, duration);                                      // TODO: Add duration for how long in the same area on the board

  // Sleep for given time to reduce consumed power
  delay(LOOP_DELAY);
}
