#include <LiquidCrystal_I2C.h> 
#include <Wire.h> 
#define SensorLeft    6   //input pin of left sensor
#define SensorMiddle  5   //input pin of middle sensor
#define SensorRight   4   //input pin of right sensor
unsigned char SL;        //state of left sensor
unsigned char SM;        //state of middle sensor
unsigned char SR;        //state of right sensor
#define Lpwm_pin  13     //pin of controlling speed---- ENA of motor driver board
#define Rpwm_pin  3    //pin of controlling speed---- ENA of motor driver board
int pinLB=7;            //pin of controlling diversion----IN1 of motor driver board
int pinLF=8;            //pin of controlling diversion----IN2 of motor driver board
int pinRB=10;            //pin of controlling diversion----IN3 of motor driver board
int pinRF=9;            //pin of controlling diversion----IN4 of motor driver board
unsigned char Lpwm_val = 130;//the speed of left wheel at 180 in initialization
unsigned char Rpwm_val = 250;//the speed of right wheel at 180 in initialization
int Car_state=0;             //state of car moving

LiquidCrystal_I2C lcd(0x27,16,2);
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
void Sensor_Scan(void)
{
  SL = digitalRead(SensorLeft);
  SM = digitalRead(SensorMiddle);
  SR = digitalRead(SensorRight);

  lcd.setCursor(0,0);
  lcd.print("L:");
  lcd.setCursor(2,0);
  lcd.print(SL);
  lcd.setCursor(4,0);
  lcd.print("- M:");
  lcd.setCursor(8,0);
  lcd.print(SM);
  lcd.setCursor(10,0);
  lcd.print("- R:");
  lcd.setCursor(14,0);
  lcd.print(SR);
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

unsigned char old_left,old_middle,old_right;
void saved_state() {
  old_middle = SM == HIGH;
  old_left = SL == HIGH;
  old_right = SR == HIGH;
}

void setup() 
{
  Serial.begin(9600);
  LCD1602_init();
  Sensor_IO_Config();
  M_Control_IO_config();        //motor controlling the initialization of IO
  Set_Speed(Lpwm_val,Rpwm_val); //setting initialization of speed
  lcd.clear();
  stopp();
}
int elasped = 0;
int elasped_lineout = 0;
int elasped_turn = 0;
int elasped_back = 0;

const int DELAY = 20;
const int DELAY_OUT_LINE = 80; 
const int DELAY_ROTATE = 20; 
const int DELAY_BACK = 40; 

bool TURN_BACK = false;

int COUNT_SENSOR = 0;
bool tag_sensor = false;
void loop()
{
  Sensor_Scan();
  int current = millis();
  bool middle = SM == HIGH;
  bool left = SR == HIGH;
  bool right = SL == HIGH;

  if (TURN_BACK) {
    Serial.println("stopppppppp.");
  } else if (current - elasped > DELAY) {
    if (!left && !middle && !right) {
       stopp(); 
    } else if (!middle && !right) {
      rightside();
    } else if (middle ) {
      Serial.println("forward.");
      leftside();
      saved_state();
    } else if (!middle) {
      Serial.println("right slide.");
      rightside();
      saved_state();
    }
    
    if (left && !tag_sensor) {
      COUNT_SENSOR++;
      tag_sensor = true;
    }
    if (!left) tag_sensor = false;
      
    lcd.setCursor(0, 1); //showing from second row
    lcd.print((String)"LEFT: " + COUNT_SENSOR);
    elasped = millis();
  } else {
    advance();
  }
}

