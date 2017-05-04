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
String chairUpGood = "ChairUp :)";
String chairUpBad = "ChairUp :(";
String warnedProsture = "";
String sittingTooLongString = "Too long!";
boolean isSittingTooLong = false;
long currentMs, previousMs = 0L;
long alertIntervalMs = 10000L;

void setup() {
  serialSetup();
  LCDSetup();
  wireConnectionSetup();
  
  printLogo(chairUpGood);
  playSystemStaredMelody();
  blink();

  delay(ONE_SECOND);
}

void loop() {
  currentMs = millis();
  if (isSittingTooLong && (currentMs - previousMs > alertIntervalMs)) {
    playAlertMelody();
    previousMs = millis()
    isSittingTooLong = false;
  }
  delay(1000);
}

void serialSetup() {
   Serial.begin(9600);
}

void LCDSetup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);  
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
  if (direction == "FSW") {
    sitMapRow3ToShow = "X|  ";
  } else if (direction == "F S") {
    sitMapRow3ToShow = " X  ";
  } else if (direction == "FSE") {
    sitMapRow3ToShow = " |X ";
  } else if (direction == "F E") {
    sitMapRow2ToShow = "- X ";
  } else if (direction == "FNE") {
    sitMapRow1ToShow = " |X ";
  } else if (direction == "F N") {
    sitMapRow1ToShow = " X  ";
  } else if (direction == "FNW") {
    sitMapRow1ToShow = "X|  ";
  } else if (direction == "F W") {
    sitMapRow2ToShow = "X - ";
  } else if (direction.substring(0, 1) == "T") {
    sitMapRow2ToShow = "-X- ";
  }
  
  String quality = command.substring(0, 1);

  printLogo(chairUpGood);
  if (quality == "T") {
    sitMapRow4ToShow = "GOOD";
    warnedProsture = "";
  } else if (quality == "F") {
    sitMapRow4ToShow = "BAD ";
    printLogo(chairUpBad);
  }
  
  String sittingTooLong = command.substring(3, 4);
  if (sittingTooLong == "T") {
    printToLCD(2, 6, sittingTooLongString);
    printToLCD(3, 8, command.substring(4, 19) + "min");

    if (direction != warnedProsture) {
      warnedProsture = direction;
      isSittingTooLong = true;
    }
  } else {
    printToLCD(2, 6, "          ");
    printToLCD(3, 8, "        ");
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

void printLogo(String text) {
  printToLCD(0, 6, text);
}

void printToLCD(int row, int startColumn, String text) {
  lcd.setCursor(startColumn, row);
  lcd.print(text);
}

void blink() {
/*
  "  /^^^\      /^^^\  "
  " | **  |    | **  | "
  " | **  |    | **  | "
  "  \___/      \___/  "

  "                    "
  " |     |    |     | "
  " |     |    |     | "
  " ///|||\\  ///|||\\ "
*/
  printToLCD(0, 0, "  /^^^i      /^^^i   | **  |    | **  |  | **  |    | **  |   Y___/      Y___/  ");
  delay(1000);
  printToLCD(0, 0, "                     |     |    |     |  |     |    |     |  ///|||||  ///||||| ");
  delay(400);
  printToLCD(0, 0, "  /^^^i      /^^^i   | **  |    | **  |  | **  |    | **  |   Y___/      Y___/  ");
  delay(1000);
  printToLCD(0, 0, "                            lol                                                 ");
  
}

