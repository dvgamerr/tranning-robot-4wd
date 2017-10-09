byte one[8] = {0b00100,0b00100,0b00100,0b00000,0b00000,0b00000,0b00000,0b00000};
byte two[8] = {0b01010,0b01010,0b01010,0b00000,0b00000,0b00000,0b00000,0b00000};
byte three[8] = {0b10101,0b10101,0b10101,0b00000,0b00000,0b00000,0b00000,0b00000};

byte char_p[8] = {0b01110,0b01110,0b01110,0b01110,0b01110,0b01110,0b01110,0b01110};
byte char_s[8] = {0b00000,0b00000,0b11011,0b11011,0b11011,0b11011,0b00000,0b00000};
byte char_fw[8] = {0b00100,0b01110,0b11011,0b10001,0b00100,0b01110,0b11011,0b10001};
byte char_b[8] = {0b10001,0b11011,0b01110,0b00100,0b10001,0b11011,0b01110,0b00100};

void lcdCreateChar() {
  lcd.createChar(11, one);
  lcd.createChar(12, two);
  lcd.createChar(13, three);
  lcd.createChar(14, char_p);
  lcd.createChar(STOP, char_s);
  lcd.createChar(FORWARD, char_fw);
  lcd.createChar(BACK, char_b);
  lcd.home();
}

void setSensorState(int state) {
  SENSOR_LAST = SENSOR_STATE;
  SENSOR_STATE = state;
}

void lcdMotor(byte state) {
  lcd.setCursor(11, 0);
  lcd.write(state == STOP ? 254 : LPWM == SPEED1 ? 11 : (LPWM == SPEED2 ? 12 : 13));
  lcd.write(state);
  lcd.write(14); 
  lcd.write(state);
  lcd.write(state == STOP ? 254 : LPWM == SPEED1 ? 11 : (LPWM == SPEED2 ? 12 : 13));
}

void stopp()         //stop
{
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, HIGH);
  lcdMotor(STOP);
}
void back()          //back
{
  digitalWrite(pinRB, LOW); //making motor move towards right rear
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, LOW); //making motor move towards left rear
  digitalWrite(pinLF, HIGH);
  lcdMotor(BACK);
}
void going()
{
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);
  lcdMotor(FORWARD);
}


void turnR()        //turning on the right(dual wheels)
{
  digitalWrite(pinRB, HIGH); //making motor move towards right rear
  digitalWrite(pinRF, LOW);
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH); //making motor move towards left front

}
void turnL()        //turning on the left(dual wheels)
{
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH);  //making motor move towards right front
  digitalWrite(pinLB, HIGH);  //making motor move towards left rear
  digitalWrite(pinLF, LOW);

}
void leftside() {
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, LOW);
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, LOW);
  // Set_Speed(Lpwm_val, Rpwm_val);

}

void rightside() {
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, LOW);
  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, LOW);
  // Set_Speed(Lpwm_val, Rpwm_val);

}

