#include <LiquidCrystal.h>
#include <Wire.h>
#include "pitches.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int PIEZO_BUFFER_PIN = 9;

int ONE_SECOND = 1000;
double pauseBetweenNotesConstant = 1.3;

int SYSTEM_STARTED_MELODY[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int SYSTEM_STARTED_NOTE_DURATIONS[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int ALERT_MELODY[] = {
  NOTE_C4, NOTE_C4, NOTE_C4
};
int ALERT_NOTE_DURATIONS[] = {
  8, 8, 8
};

boolean badPosture = false;
void setup() {
  serialSetup();
  LCDSetup();
  wireConnectionSetup();
  
  printToLCD("ChairUp :)");
  playSystemStaredMelody();

  delay(ONE_SECOND);
}

void loop() {
  if (badPosture) {
    playAlertMelody();
    badPosture = false;
  }
  delay(ONE_SECOND);
}

void serialSetup() {
   Serial.begin(9600);
}

void LCDSetup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);  
}

void printToLCD(String text) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(text);
}

//prints " " to 5 columns x 4 rows from startColumn onward
void clear5Columns(Int startColumn) {
  lcd.setCursor(startColumn, 0);
  lcd.print("     ");
  lcd.setCursor(startColumn, 1);
  lcd.print("     ");
  lcd.setCursor(startColumn, 2);
  lcd.print("     ");
  lcd.setCursor(startColumn, 3);
  lcd.print("     ");
}

/*
Left columns 15 to 20, 4 rows
*/
void printTo5Columns(Char charArray, Int startColumn) {
  clear5Columns(startColumn);
  int maxChars = 20;
  int maxInRowRow = 20/4;
  int len = sizeof(charArray);
  for (i = 0; i < len %% i >= maxChars; i++){
    if (i = 0){
      lcd.setCursor(startColumn, 0);
    }
    else if (i = maxInRowRow + 1){
      lcd.setCursor(startColumn, 1);
    }
    else if (i = maxInRowRow * 2 + 1){
      lcd.setCursor(startColumn, 2);
    }
    else if (i = maxInRowRow * 3 + 1){
      lcd.setCursor(startColumn, 3);
    }
    lcd.print(charArray[i]);
    //Do I need to setCursor to next position?
  }
}

void printHelloWorld(){
  char *strToChar = "HelloWorld";
  printTo5Columns(strToChar, 15);
}

void playSystemStaredMelody() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = ONE_SECOND / SYSTEM_STARTED_NOTE_DURATIONS[thisNote];
    tone(PIEZO_BUFFER_PIN, SYSTEM_STARTED_MELODY[thisNote], noteDuration);
    
    delay(noteDuration * pauseBetweenNotesConstant);
    
    noTone(PIEZO_BUFFER_PIN);
  }
}

void playAlertMelody() {
  for (int thisNote = 0; thisNote < 3; thisNote++) {
    int noteDuration = ONE_SECOND / ALERT_NOTE_DURATIONS[thisNote];
    tone(PIEZO_BUFFER_PIN, ALERT_MELODY[thisNote], noteDuration);

    delay(noteDuration * pauseBetweenNotesConstant);
    
    noTone(PIEZO_BUFFER_PIN);
  }
}

void wireConnectionSetup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int howMany) {
  String textToDisplay = "";
  while (1 <= Wire.available()) { 
    char c = Wire.read();
    textToDisplay += c;
  }
  Serial.println(textToDisplay);
  printToLCD(textToDisplay);

  //Hellowordl testing
  printHelloWorld();

  if (textToDisplay == "Bad") {
    badPosture = true;
  }

}
