#include <LiquidCrystal.h>   //Include the LiquidCrystal library, which gives us all the functions to control the LCD.

// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);  //Create an LCD object and tell the Arduino which pins are connected.

void setup() {
  lcd.begin(16, 2);   // Initialize the LCD with 16 columns and 2 rows
  lcd.clear();         // Clear the display to remove any residual characters
  lcd.setCursor(0, 0);  // Set cursor to column 0, row 0
  lcd.print("Hello UNO!");  // Print initial message on the first row
}

void loop() {
  lcd.setCursor(0, 1);   // Set cursor to column 0, row 1
  lcd.print("Seconds: ");    // Print label on the second row
  lcd.print(millis() / 1000);    // Print elapsed seconds since Arduino started
  delay(1000);       // Wait for 1 second
}