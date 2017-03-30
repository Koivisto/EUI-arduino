import processing.serial.*;

Serial myPort;

// Debug mode
boolean DEBUG     = false;

// Stores 
int buttonRows    = 3;
int buttonsPerRow = 4;
int buttons       = buttonRows * buttonsPerRow;
int[] buttonStates = new int[buttons];

void setup() {
  // Set viewport size
  size(800, 600);
  
  // Handle serial port
  myPort = new Serial(this, Serial.list()[1], 9600);
  myPort.bufferUntil('\n');   // Start listening for serial messages
  
  if (DEBUG) {
    setButton(5, 1);
  }
}

void  draw() {
  
  background(255,255,255);

 for (int i = 0; i < buttonStates.length; i++) {
    
    if (buttonStates[i] == 1) {
      // Active state fill color
      fill(20, 255, 10);
    }
    
    else {
       // Inactive state fill color
      fill(255, 20, 10);
    }
    
    println(buttonStates[10]);
  
    // Draw the buttons with correct
    rect( ((i % buttonsPerRow)) * 160,              // col 1 -> 4
          (i / buttonsPerRow) * 160, // row 1 -> 3
          120,
          120
    );
    
    if (DEBUG) {
      //println("row: " + ((i / buttonsPerRow)) + " col: " + (i % buttonsPerRow));
    }
  }
}

void setButton (int labeledNumber, int num) {
  buttonStates[labeledNumber - 1] = num;
}

/*
 * Updates the button state array per Serial data.
 * NOTE: Assumes space separated values from Serial!
 *       i.e., '0 1 0 0 1 0 0 1 0 1 0 0'
 */
void serialEvent (Serial myPort) {
  
  String inString = myPort.readStringUntil('\n');
  inString = trim(inString);
  
  if (DEBUG) {
    println("From serial: " + inString);
  }
  
  // Update button states (no error handling here for now :3)
  buttonStates = int(split(inString, ' '));
}