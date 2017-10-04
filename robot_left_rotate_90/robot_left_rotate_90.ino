#include <LiquidCrystal_I2C.h> 
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);


#define SensorLeft    6    //input pin of left sensor
#define SensorMiddle  5    //input pin of middle sensor
#define SensorRight   4    //input pin of right sensor
#define Lpwm_pin      13   //pin of controlling speed---- ENA of motor driver board
#define Rpwm_pin      3    //pin of controlling speed---- ENA of motor driver board

int pinLB = 7;            //pin of controlling diversion----IN1 of motor driver board
int pinLF = 8;            //pin of controlling diversion----IN2 of motor driver board
int pinRB = 10;           //pin of controlling diversion----IN3 of motor driver board
int pinRF = 9;            //pin of controlling diversion----IN4 of motor driver board

unsigned char Lpwm_val = 130;   //the speed of left wheel at 180 in initialization
unsigned char Rpwm_val = 250;   //the speed of right wheel at 180 in initialization
int Car_state = 0;

unsigned char SL;
unsigned char SM;
unsigned char SR;

unsigned char old_left,old_middle,old_right;

//int _STEP[6] = {"L3","L2","L1","L1"};
int MISSION[4] = {3,2,1,1};
int elasped_sensor = 0;
int elasped_lineout = 0;
int elasped_turn = 0;
int elasped_back = 0;

const int DELAY = 20;
const int DELAY_OUT_LINE = 80; 
const int DELAY_ROTATE = 20; 
const int DELAY_BACK = 40; 
const int DELAY_TURN_LEFT = 950;

//const int DELAY = 0;
//const int DELAY_OUT_LINE = 25; 
//const int DELAY_ROTATE = 8; 
//const int DELAY_BACK = 10; 
//const int DELAY_TURN_LEFT = 300;

byte IDIE = 0;
byte BACK = 1;
byte FORWARD = 2;
byte LEFT = 3;
byte RIGHT = 4;

byte SENSOR_STATE = IDIE;
byte SENSOR_LAST = IDIE;

int COUNT_SENSOR = 0;
int LOOP_STEP = 0;
bool tag_sensor = false;


long rotate_begin = 0;
void Sensor_Scan(void)
{
  SL = digitalRead(SensorLeft);
  SM = digitalRead(SensorMiddle);
  SR = digitalRead(SensorRight);

  lcd.setCursor(0,0);
  lcd.print((String)SL+":"+SM+":"+SR+" - "+SENSOR_LAST+"->"+SENSOR_STATE);
}



void setup() 
{
  Serial.begin(9600);
  LCD1602_init();
  Sensor_IO_Config();
  M_Control_IO_config();        //motor controlling the initialization of IO
  Set_Speed(Lpwm_val,Rpwm_val); //setting initialization of speed
  stopp();
}

void LCD1602_init(void)
{
  lcd.init();                     
  lcd.backlight(); 
  lcd.clear();  
}
void Sensor_IO_Config()
{
  pinMode(SensorLeft,INPUT);
  pinMode(SensorMiddle,INPUT);
  pinMode(SensorRight,INPUT);
}

void M_Control_IO_config(void)//initialized function of IO of motor driver
{
  pinMode(pinLB,OUTPUT); // pin 2--IN1 of motor driver board
  pinMode(pinLF,OUTPUT); // pin 4--IN2 of motor driver board
  pinMode(pinRB,OUTPUT); // pin 7--IN3 of motor driver board
  pinMode(pinRF,OUTPUT); // pin 8--IN4 of motor driver board
  pinMode(Lpwm_pin,OUTPUT); // pin 5  (PWM) --ENA of motor driver board
  pinMode(Rpwm_pin,OUTPUT); // pin 10 (PWM) --ENB of motor driver board  
}

void Set_Speed(unsigned char Left,unsigned char Right)//setting function of speed
{
  analogWrite(Lpwm_pin,Left);   
  analogWrite(Rpwm_pin,Right);
}
void advance()     // going forwards
{
   digitalWrite(pinRB,HIGH);  // making motor move towards right rear
   digitalWrite(pinRF,LOW);
   digitalWrite(pinLB,HIGH);  //  making motor move towards left rear
   digitalWrite(pinLF,LOW); 
   Car_state = 1; 
//   show_state();   
}
void turnR()        //turning on the right(dual wheels)
    {
     digitalWrite(pinRB,HIGH);  //making motor move towards right rear
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,LOW);
     digitalWrite(pinLF,HIGH);  //making motor move towards left front
     Car_state = 4;
//     show_state();
    }
void turnL()        //turning on the left(dual wheels)
    {
     digitalWrite(pinRB,LOW);
     digitalWrite(pinRF,HIGH);   //making motor move towards right front
     digitalWrite(pinLB,HIGH);   //making motor move towards left rear
     digitalWrite(pinLF,LOW);
     Car_state = 3;
//     show_state();
    }    
void stopp()         //stop
    {
     digitalWrite(pinRB,HIGH);
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinLF,HIGH);
//     Car_state = 5;
//     show_state();
    }
void back()          //back
    {
     digitalWrite(pinRB,LOW);  //making motor move towards right rear
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,LOW);  //making motor move towards left rear
     digitalWrite(pinLF,HIGH);
     Car_state = 2;
//     show_state() ;    
    }
void show_state(void)  //showing current state of the car
{
  lcd.setCursor(0, 1); //showing from second row
   switch(Car_state)
   {
     case 1:lcd.print(" Go  ");
     break;
     case 2:lcd.print("Back ");
     break;
     case 3:lcd.print("Left ");
     break;
     case 4:lcd.print("Right");
     break;
     case 5:lcd.print("Stop "); 
     break;
     default:
     break;
   }
}
void leftside() {
  digitalWrite(pinRB,HIGH);
  digitalWrite(pinRF,LOW);
  digitalWrite(pinLB,LOW);
  digitalWrite(pinLF,LOW);
  Set_Speed(Lpwm_val, Rpwm_val);
  Car_state = 3;
//  show_state();
}

void rightside() {
  digitalWrite(pinRB,LOW);
  digitalWrite(pinRF,LOW);
  digitalWrite(pinLB,HIGH);
  digitalWrite(pinLF,LOW);
  Set_Speed(Lpwm_val,Rpwm_val);
  Car_state = 4;
//  show_state();
}

void saved_state() {
  old_middle = SM == HIGH;
  old_left = SL == HIGH;
  old_right = SR == HIGH;
}
void setSensorState(int state) {
  SENSOR_LAST = SENSOR_STATE;
  SENSOR_STATE = state;
}
int LAP = 0;

bool LOOP_TWO = false;
bool sensor_middle = false;
bool toggle_middle = false;
void loop()
{
  Sensor_Scan();
  
  int sensor_milis = millis();
  bool middle = SM == HIGH;
  bool left = SR == HIGH;
  bool right = SL == HIGH;

  if (sensor_milis - elasped_sensor > DELAY) {
    if (SENSOR_STATE == IDIE) {
      stopp();
      if (left || middle || right) setSensorState(FORWARD);
    } else if (SENSOR_STATE == LEFT) {
      Serial.println((String)"TURN LEFT -- SL:" +left + " SM:" + middle + " SR:" + right);
      long elasped_rotate = millis();
      if (rotate_begin == 0) rotate_begin = millis();
      if (elasped_rotate - rotate_begin <= DELAY_TURN_LEFT && !(middle && right & LOOP_TWO)) {
        turnR();
      } else {
        LOOP_TWO = false;
        COUNT_SENSOR = 0;
        rotate_begin = 0;
        setSensorState(FORWARD);
      }
    } else if (SENSOR_STATE == FORWARD) {
      if (!left && !middle && !right) {
        if (SENSOR_LAST == LEFT && old_right) {
          rightside();
        } else if (SENSOR_LAST == LEFT && old_left && COUNT_SENSOR == 1) {
          setSensorState(LEFT);
          COUNT_SENSOR = 0;
          LOOP_TWO = true;
        }
      } else if (!middle || !right) {
        Serial.println("forward.");
        rightside();
        saved_state();
      } else if (middle) {
        Serial.println("forward.");
        leftside();
        saved_state();
      } else {
        advance();
        saved_state();
      }
      
      if (left && !tag_sensor && !LOOP_TWO) {
        COUNT_SENSOR++;
        tag_sensor = true;
        if (COUNT_SENSOR == MISSION[LAP]) {
          setSensorState(LEFT);
          LAP++;
          if (sizeof(MISSION) / 2 <= LAP) LAP = 0;
        }
      }
      if (!left) tag_sensor = false;
    }
    lcd.setCursor(0, 1); //showing from second row
    lcd.print((String)" M:"+ (LAP+1) + " LAP:" + COUNT_SENSOR + "/" + MISSION[LAP]);
    elasped_sensor = millis();
  }
}

