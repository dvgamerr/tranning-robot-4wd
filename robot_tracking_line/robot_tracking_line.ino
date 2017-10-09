#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define pinEcho   A3
#define pinTriger A2
#define pinServo  3

#define pinSL     A0
#define pinSML    13
#define pinSMM    12
#define pinSMR    11
#define pinSR     A1

#define pinL_PWM  5
#define pinR_PWM  6

#define pinLB     4
#define pinLF     2
#define pinRB     7
#define pinRF     8

unsigned char SPEED1 = 90;
unsigned char SPEED2 = 100;
unsigned char SPEED3 = 150;

unsigned char LPWM = SPEED2;
unsigned char RPWM = SPEED2;

unsigned char SML, SMM, SMR, SL, SR;
bool oSMM, oSML, oSMR, oSR, oSL;
bool cSMM, cSML, cSMR, cSR, cSL;

const byte STOP = 0;
const byte BACK = 1;
const byte FORWARD = 2;
const byte SLIDE_LEFT = 3;
const byte SLIDE_RIGHT = 4;
const byte TURN_LEFT = 5;
const byte TURN_RIGHT = 6;
const byte UTURN_LEFT = 7;
const byte UTURN_RIGHT = 8;

byte SENSOR_STATE = STOP;
byte SENSOR_LAST = STOP;

#define DELAY           20
#define DELAY_OUT_LINE  80
#define DELAY_ROTATE    20
#define DELAY_BACK      40
#define DELAY_TURN_LEFT 950
#define DELAY_SONIC     50
#define RATE_SONIC      58.65
#define SERVO_CENTOR    1880

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(9600);
  InitLCD();
  lcdCreateChar();
  // motor controlling the initialization of IO
  InitControl();
  InitSensor();
  // setting initialization of speed
  setSpeedMotor(LPWM, RPWM);
  stopp();
}

int elasped_sensor = 0;
int elasped_lineout = 0;

bool TURN_BACK = false;

int COUNT_SENSOR = 0;
int LOOP_STEP = 0;
bool tag_sensor = false;


long rotate_begin = 0;

int LAP = 0;

bool LOOP_TWO = false;
bool sensor_middle = false;
bool toggle_middle = false;

char* raw = "L3-L2-L1-L1";
int MISSION[4] = {3,2,1,1};

void loop()
{
  SensorScan();
  int sensor_milis = millis();
  if (sensor_milis - elasped_sensor > DELAY) {
    switch (SENSOR_STATE) {
      case STOP:
        stopp();
        if (cSML || cSMR) setSensorState(FORWARD);
        break;
      case FORWARD:
        if (!cSML && !cSMM && !cSMR) {
          if (SENSOR_LAST == TURN_LEFT && oSMR) {
            rightside();
          } else if (SENSOR_LAST == TURN_LEFT && oSML && COUNT_SENSOR == 1) {
            setSensorState(TURN_LEFT);
            COUNT_SENSOR = 0;
            LOOP_TWO = true;
          }
        } else if (!cSMR) {
          leftside();
          SensorState();
        } else if (!cSML) {
          rightside();
          SensorState();
        } else {
          going();
          SensorState();
        }
        
        if (cSML && !tag_sensor && !LOOP_TWO) {
          COUNT_SENSOR++;
          tag_sensor = true;
          if (COUNT_SENSOR == MISSION[LAP]) {
            setSensorState(TURN_LEFT);
            LAP++;
            if (sizeof(MISSION) / 2 <= LAP) LAP = 0;
          }
        }
        if (!cSML) tag_sensor = false;
        break;
      case TURN_LEFT:
//        int elasped_turn = millis();
//        if (rotate_begin == 0) rotate_begin = millis();
//        if (elasped_turn - rotate_begin <= DELAY_TURN_LEFT && !(cSMM && cSMR & LOOP_TWO)) {
//          turnR();
//        } else {
//          LOOP_TWO = false;
//          COUNT_SENSOR = 0;
//          rotate_begin = 0;
//          setSensorState(FORWARD);
//        }
        break;
      case TURN_RIGHT:

        break;
      case BACK:

        break;
    }

    lcd.setCursor(0, 1); //showing from second row
    lcd.print((String)" M:"+ (LAP+1) + " LAP:" + COUNT_SENSOR + "/" + MISSION[LAP]);
    elasped_sensor = millis();
  }
}

