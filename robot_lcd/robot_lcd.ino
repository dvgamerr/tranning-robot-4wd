#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,15,2);

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  lcd.setCursor(4,0);
  lcd.print("ROBOT");
  lcd.setCursor(0,1);
  lcd.print("System begin...");
}

