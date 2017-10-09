void InitLCD(void)
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
}
void InitSensor()
{
  pinMode(pinSML, INPUT);
  pinMode(pinSMM, INPUT);
  pinMode(pinSMR, INPUT);
  pinMode(pinSL, INPUT);
  pinMode(pinSR, INPUT);
}
void SensorScan(void)
{
  SML = digitalRead(pinSML);
  SMM = digitalRead(pinSMM);
  SMR = digitalRead(pinSMR);
  SL = digitalRead(pinSL);
  SR = digitalRead(pinSR);

  cSMM = SMM == HIGH;
  cSML = SML == HIGH;
  cSMR = SMR == HIGH;
  cSL = SL == HIGH;
  cSR = SR == HIGH;

  lcd.setCursor(0, 0);

  lcd.write(SL == 0 ? 255 : 254);
  lcd.print(":");
  lcd.write(SML == 0 ? 255 : 254);
  lcd.write(SMM == 0 ? 255 : 254);
  lcd.write(SMR == 0 ? 255 : 254);
  lcd.print(":");
  lcd.write(SR == 0 ? 255 : 254);
}
void SensorState(void) {
  oSMM = SMM == HIGH;
  oSML = SML == HIGH;
  oSMR = SMR == HIGH;
  oSL = SL == HIGH;
  oSR = SR == HIGH;
}

void InitControl(void) {
  pinMode(pinLB, OUTPUT); // pin 2--IN1 of motor driver board
  pinMode(pinLF, OUTPUT); // pin 4--IN2 of motor driver board
  pinMode(pinRB, OUTPUT); // pin 7--IN3 of motor driver board
  pinMode(pinRF, OUTPUT); // pin 8--IN4 of motor driver board
  pinMode(pinL_PWM, OUTPUT); // pin 5  (PWM) --ENA of motor driver board
  pinMode(pinR_PWM, OUTPUT); // pin 10 (PWM) --ENB of motor driver board
}

void setSpeedMotor(unsigned char Left, unsigned char Right) {
  analogWrite(pinL_PWM, Left);
  analogWrite(pinR_PWM, Right);
}
