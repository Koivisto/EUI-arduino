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

String sitMapRow1 = " |  ";
String sitMapRow2 = "- - ";
String sitMapRow3 = " |  ";
String sitMapRow4 = "    ";

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
  String command = "";
  while (1 <= Wire.available()) { 
    char c = Wire.read();
    command += c;
  }
  Serial.println(command);

  String sitMapRow1ToShow = sitMapRow1;
  String sitMapRow2ToShow = sitMapRow2;
  String sitMapRow3ToShow = sitMapRow3;
  String sitMapRow4ToShow = sitMapRow4;

  String direction = command.substring(0, 3);
  if (direction == "FNW") {
    sitMapRow3ToShow = " |X ";
  } else if (direction == "F N") {
    sitMapRow3ToShow = " X  ";
  } else if (direction == "FNE") {
    sitMapRow3ToShow = "X|  ";
  } else if (direction == "F E") {
    sitMapRow2ToShow = "X - ";
  } else if (direction == "FSE") {
    sitMapRow1ToShow = "X|  ";
  } else if (direction == "F S") {
    sitMapRow1ToShow = " X  ";
  } else if (direction == "FSW") {
    sitMapRow1ToShow = " |X ";
  } else if (direction == "F W") {
    sitMapRow2ToShow = "- X ";
  } else if (direction == "T  ") {
    sitMapRow2ToShow = "-X- ";
  }
  
  String quality = command.substring(0, 1);
  if (quality == "T") {
    sitMapRow4ToShow = "GOOD";
  } else if (quality == "F") {
    sitMapRow4ToShow = "BAD ";
    playAlertMelody();
  }
  
  printSitMapRow1(sitMapRow1ToShow);
  printSitMapRow2(sitMapRow2ToShow);
  printSitMapRow3(sitMapRow3ToShow);
  printSitMapRow4(sitMapRow4ToShow);
}

void printSitMapRow1(String text) {
  printToLCD(0, 0, text);
}
void printSitMapRow2(String text) {
  printToLCD(1, 0, text);
}
void printSitMapRow3(String text) {
  printToLCD(2, 0, text);
}
void printSitMapRow4(String text) {
  printToLCD(3, 0, text);
}

void printToLCD(int row, int startColumn, String text) {
  lcd.setCursor(startColumn, row);
  lcd.print(text);
}
