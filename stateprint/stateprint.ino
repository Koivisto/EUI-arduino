/*
 * Track and draw 12 button states
 */

// Define digital outputs for the buttons
// Constant is for program
// Value comes from the arduino board, 1 reserverd for output
const int btn1 = 2;
const int btn2 = 3;
const int btn3 = 4;
const int btn4 = 5;
const int btn5 = 6;
const int btn6 = 7;
const int btn7 = 8;
const int btn8 = 9;
const int btn9 = 10;
const int btn10 = 11;
const int btn11 = 12;
const int btn12 = 13;

// Button state storage
int stateOf1, stateOf2, stateOf3, stateOf4, stateOf5, stateOf6, 
stateOf7, stateOf8, stateOf9, stateOf10, stateOf11, stateOf12;

// Called once when the code is run
void setup() {

 // Set pin modes for the used digital pins
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  pinMode(btn4, INPUT);
  pinMode(btn5, INPUT);
  pinMode(btn6, INPUT);
  pinMode(btn7, INPUT);
  pinMode(btn8, INPUT);
  pinMode(btn9, INPUT);
  pinMode(btn10, INPUT);
  pinMode(btn11, INPUT);
  pinMode(btn12, INPUT);

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
  stateOf1 = digitalRead(btn1);
  stateOf2 = digitalRead(btn2);
  stateOf3 = digitalRead(btn3);
  stateOf4 = digitalRead(btn4);
  stateOf5 = digitalRead(btn5);
  stateOf6 = digitalRead(btn6);
  stateOf7 = digitalRead(btn7);
  stateOf8 = digitalRead(btn8);
  stateOf9 = digitalRead(btn9);
  stateOf10 = digitalRead(btn10);
  stateOf11 = digitalRead(btn11);
  stateOf12 = digitalRead(btn12);

  /* Print state diagram 
  *  1-4 | _ _ _ _ |
  *  5-8 | X X X _ |
  *  9-12| X X X _ |
  *  ---------------
  *  X marks pressed state. 
  */

  // Row 1
  Serial.print("1-4 |");
  //Everytime this button is pressed
  if (stateOf1 == HIGH) {printI();} else {printO();}
  if (stateOf2 == HIGH) {printI();} else {printO();}
  if (stateOf3 == HIGH) {printI();} else {printO();}
  if (stateOf4 == HIGH) {printI();} else {printO();}
  Serial.println(" |");
  // Row 2
  Serial.print("5-8 |");
  if (stateOf5 == HIGH) {printI();} else {printO();}
  if (stateOf6 == HIGH) {printI();} else {printO();}
  if (stateOf7 == HIGH) {printI();} else {printO();}
  if (stateOf8 == HIGH) {printI();} else {printO();}
  Serial.println(" |");
  // Row 3
  Serial.print("5-8|");
  if (stateOf9 == HIGH) {printI();} else {printO();}
  if (stateOf10 == HIGH) {printI();} else {printO();}
  if (stateOf11 == HIGH) {printI();} else {printO();}
  if (stateOf12 == HIGH) {printI();} else {printO();}
  Serial.println(" |");
  Serial.println("---------------");
}
