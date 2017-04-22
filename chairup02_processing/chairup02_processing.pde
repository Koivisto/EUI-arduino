import processing.serial.*;

// Serial port
Serial serialPort;
// Debug mode
boolean DEBUG           = true;
boolean SIMULATE        = false;

// Button variables
int buttonRows          = 3;
int buttonsPerRow       = 4;
int buttons             = buttonRows * buttonsPerRow;
int buttonWidth         = 100;  // Hard-coded for 600x400 window size
int buttonHeight        = 100;  // Hard-coded for 600x400 window size
int buttonCornerRadius  = 5;
int buttonXSpacing      = 166;  // Hard-coded for 600x400 window size
int buttonYSpacing      = 149;  // Hard-coded for 600x400 window size

// General variables for drawing
int centreOfMassWH      = 50;
int updatePeriod        = 80;
int resolution          = 10;

// Button state information (0/1)
int[] buttonStates      = new int[buttons];
// Timing variables used by the randomization function
int lastUpdate          = millis();

float x;
float y;

/**
 * Main initialization routine. Sets the window size and initializes the 
 * serial port communication.
 */
void setup () {
  // Sets the viewport size
  size(600, 400);
  
  // Handles the serial port initialization
  serialPort = new Serial(this, Serial.list()[1], 9600);
  serialPort.bufferUntil('\n');   // Start listening for serial messages
  
  // Sets the text size for button number labels
  textSize(32);
  
  if (DEBUG) {
    // Generates the initial random state (if in DEBUG mode)
    randomizeState();
  }
}

/**
 * Listens to events in the serial port and updates the button state array.
 * The function assumes space separated values from the serial port, e.g., 
 * '0 1 0 0 1 0 0 1 0 1 0 0'.
 * @param myPort serial port to be listened
 */
void serialEvent (Serial myPort) {
  String inString = myPort.readStringUntil('\n');
  inString = trim(inString);
  
  if (DEBUG) {
    println("Received data: " + inString);
  }
  
  // Updates button states (no error handling here for now :3) 
  buttonStates = int(split(inString, ' '));
}

/**
 * Main rendering routine. Clears the window, sets the default parameters
 * and draws the elements.
 */
void  draw () {
  // Clears the window
  background(255,255,255);
  
  // Defines the default stroke settings
  stroke(0, 0, 0);
  strokeWeight(1);

  // Get centre x- and y-position of the center of the mass
  int centerOfMassX = getCenterOfMassX();
  int centerOfMassY = getCenterOfMassY();
  
  // Linear interpolation between previous and new center position
  float dist = 0.5;
  x = lerp(x, centerOfMassX, dist);
  y = lerp(y, centerOfMassY, dist);
  
  // Stores the amount of active buttons
  int activeCount = 0;
  
  for (int i = 0; i < buttonStates.length; i++) {
    if (buttonStates[i] == 1) {
      // Active state fill color
      fill(20, 255, 10);
      activeCount += 1;
    }
    else  {
      // Inactive state fill color
      fill(255, 20, 10);
    }
    
    // Draws the buttons as rectangles into the window
    rect( (i % buttonsPerRow) * buttonXSpacing, // x position
          (i / buttonsPerRow) * buttonYSpacing, // y position
          buttonWidth,               // width
          buttonHeight,              // height
          5                          // corner radius
    );
    
    // Draws button number labels inside the button areas
    fill(0, 0, 0);                           // text color
    textAlign(CENTER, CENTER);
    text( i+1,                               // Label number
          ((i % buttonsPerRow) * buttonXSpacing) + buttonWidth/2,  // x position (+ manual adjustment)
          ((i / buttonsPerRow) * buttonYSpacing) + buttonHeight/2   // y position (+ manual adjustment)
    );
  }
  if (activeCount > 0) {
    stroke(0, 0, 255);
    strokeWeight(4);
    
    for (int i = 0; i < buttonStates.length; i++) {
       if (buttonStates[i] == 1) {
          int x1 = getCenterXPosition(getCol(i+1));
          int y1 = getCenterYPosition(getRow(i+1));
        
          // Draw line from button center to center of mass
          line(x1, y1, x, y);
       }
    }
    
    fill(255, 255, 0); 
    ellipse(x, y, centreOfMassWH, centreOfMassWH);
  }
  
  if (SIMULATE && ((millis() - lastUpdate) >= updatePeriod)) {
    randomizeState();
    lastUpdate = millis();
  }
}

/**
 * Generates a random state for each button by either activating it or
 * deactivating it, depending on its current state.
 */
void randomizeState () {
  for (int i = 0; i < int(random(1, buttons)); i++) {
    int buttonNumber = int(random(1, 13));
    if (buttonStates[buttonNumber - 1] == 0) {
      setButton(buttonNumber, 1);
    }
    else {
      setButton(buttonNumber, 0);
    }
  }
}

/**
 * Sets the button state, i.e., activates or deactivates it.
 * @param labeledNumber button label number.
 * @param val           value to be set, either 0 or 1.
 */
void setButton (int labeledNumber, int val) {
  if (val < 0 || val > 1)
    return;
    
  buttonStates[labeledNumber - 1] = val;
  
  if (DEBUG)
    println("Active: " + labeledNumber + " [" + getRow(labeledNumber) + ", " +  getCol(labeledNumber) + "]");
}

/**
 * Calculates the x-position of the center of a button.
 * @param col column number of the button.
 * @return    the x-position of the center of the button in the given column. 
 */
int getCenterXPosition (int col) {
  int posX = 0;
  
  if (col == 1) {
    posX =  buttonWidth/2;
  }
  else {
   posX = ((col-1) * buttonXSpacing) + (buttonWidth/2);
  }
  
  return posX;
}

/**
 * Calculates the y-position of the center of a button.
 * @param row row number of the button.
 * @return    the y-position of the center of the button in the given row. 
 */
int getCenterYPosition (int row) {
  int posY = 0;
  
  if (row == 1) {
    posY =  buttonHeight/2;
  }
  else {
   posY = ((row-1) * buttonYSpacing) + (buttonHeight/2);
  }
  
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
  
  for (int i = 0; i < buttonStates.length; i++) {
    if (buttonStates[i] == 1) {
      sumX += getCenterXPosition(getCol(i+1));
      count += 1;
    }
  }
  
  if (count == 0) {
    // Avoids division by 0
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
  
  for (int i = 0; i < buttonStates.length; i++) {
    if (buttonStates[i] == 1) {
      sumY += getCenterYPosition(getRow(i+1));
      count += 1;
    }
  }
  
  if (count == 0) {
    // Avoids division by 0
    return 0;
  }
  
  return sumY / count;
}

/**
 * Calculates the row number of the button.
 * @param labeledNumber button label number.
 * @return              the row number of the button, 
 *                      or -1 if the  button number is out of bounds.              
 */
int getRow (int labeledNumber) {
  int row = -1;
  
  if (labeledNumber < 1 || labeledNumber > buttons)
    // Returns -1, out of bounds
    return row;
  if (labeledNumber % buttonsPerRow == 0)
    row = (labeledNumber / buttonsPerRow);
  else if (labeledNumber > buttonsPerRow)
    row = (labeledNumber / buttonsPerRow) + 1;
  else if (labeledNumber < buttonsPerRow)
    row = 1;
    
  return row;
}

/**
 * Calculates the column number of the button.
 * @param labeledNumber button label number.
 * @return              the column number of the button, 
 *                      or -1 if the button number is out of bounds.                 
 */
int getCol (int labeledNumber) {
  int col = -1;
  
  if (labeledNumber < 1 || labeledNumber > buttons)
    // Returns -1, out of bounds
    return col;
  if (labeledNumber % buttonsPerRow == 0)
    col = buttonsPerRow;
  else
    col = (labeledNumber % buttonsPerRow);
    
  return col;
}