//This code controls the speed and direction of a Motor. There are two modes which are
//manual mode and automatic mode. In the manual mode, the buttons are used to control 
//the speed and direction of the Motor. In the automatic mode, a phototransistor is used
//to control the speed and direction of the Motor.

//Code written by Puvithan Velauthan
//Date the code was written : 16/05/21

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the 
//interface pins 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//The variables which are used in the manual mode are declared

int ModeType; // The variable ModeType is used to switch from manual mode to automatic mode
int lcd_key;
int Mopin1 = 3;
int Mopin2 = 11;
int count; // The variable count is used to control the speed of the motor in the manual mode

//The variables which are used in the automatic mode are declared

int val = 0; //The variable val is used to store the light level measured by the phototransistor
int spd = 0; //The variable spd is used to control the speed of the motor in the automatic mode
int result = 0; //The variable result is used to store the value of the light level from 0 to 100
int analogPin = A1;

//The pin numbers are declared to each of the buttons
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

//All the  buttons are connected to the analogue zero port on the Arduino via a chain of resistors.
//Therefore, the resistance changes depending on which button you press
int read_LCD_buttons(){
int adc_key_in = analogRead(0);
if (adc_key_in > 1000) return btnNONE;
if (adc_key_in < 50) return btnRIGHT;
if (adc_key_in < 250) return btnUP;
if (adc_key_in < 350) return btnDOWN;
if (adc_key_in < 450) return btnLEFT;
if (adc_key_in < 750) return btnSELECT;
return btnNONE;
}

void setup(){
  // put your setup code here, to run once:
 lcd.begin(16, 2); //This tells the Arduino that the screen size is 16 characters by 2
 // Print a message to the LCD.
 lcd.clear();

 pinMode(Mopin1, OUTPUT);
 pinMode(Mopin2, OUTPUT);

 ModeType = 1; //The variable ModeType is assigned to a value of 1
}

void loop() {
  lcd_key = read_LCD_buttons();

//The value of the variable ModeType changes when the select button is pressed

if (lcd_key == btnSELECT){
  ModeType = -ModeType;
}
delay(100);

//If the variable ModeType has a value of 1, then the manual part of the code is executed
if (ModeType == 1){
lcd.setCursor(0,0);
lcd.print("Speed:");
lcd.setCursor(7, 0);
lcd.print ("     ");
lcd.setCursor(10, 0);
lcd.print ("%");
lcd.setCursor(13, 0);
lcd.print ("  ");
lcd.setCursor(15, 0);
lcd.print ("M"); //The LCD displays M to indicate that it is in the manual mode

//If the right button is pressed then, the motor spins in the clockwise direction
if (lcd_key == btnRIGHT){
  Mopin1 = 3;
  Mopin2 = 11;
  lcd.setCursor(0,1);
  lcd.print ("Clockwise");
  delay(50);
  lcd.print("                ");
}

//If the left button is pressed then, the motor spins in the anti-clockwise direction
if (lcd_key == btnLEFT){
  Mopin1 = 11;
  Mopin2 = 3;
  lcd.setCursor(0,1);
  lcd.print ("Anti-Clockwise");
  delay(50);
  lcd.print("                ");
}

//If the up button is pressed then, the speed of the motor is increased
if (lcd_key == btnUP) {
  lcd_key = read_LCD_buttons();
  lcd.setCursor(7,0);
  //Displays the speed of the motor as a percentage
  lcd.print((++count)*100/255); //The "++" increments the count by 1 
  delay (100);
  lcd.print(" ");
}

//If the down button is pressed then, the speed of the motor is decreased
if (lcd_key == btnDOWN) {
  lcd_key = read_LCD_buttons();
  lcd.setCursor(7,0);
  //Displays the speed of the motor as a percentage
  lcd.print((--count)*100/255); //The "--" decrements the count by 1
  delay (100);
  lcd.print(" ");
}

//Controls the speed of the motor
if (count > 0) {
 analogWrite(Mopin1, count);
 analogWrite(Mopin2, LOW);
}
 
}

//If the variable ModeType has a value of -1, then the automatic part of the code is executed
if (ModeType == -1){
lcd.setCursor(0,0);
lcd.print("Speed:");
lcd.setCursor(7, 0);
lcd.print ("     ");
lcd.setCursor(10, 0);
lcd.print ("%");
lcd.setCursor(13, 0);
lcd.print ("  ");
lcd.setCursor(15, 0);
lcd.print ("A"); //The LCD displays A to indicate that it is in the automatic mode

val = analogRead(A1); //The variable val stores the light level measured by the phototransistor
result = map(val, 0, 1023, 0, 100); //The variable result stores the value of the light level from 0 to 100
lcd.setCursor(12,0);
lcd.print(result); //Displays the value of the light level from 0 to 100

//if the light level is above the normal ambient room level, the motor turns
//clockwise
if (result > 17) {
 spd = map(result, 17 , 100, 0, 255);//makes sure that motor starts from 0 to 100%
 analogWrite(Mopin1, spd);
 analogWrite(Mopin2, LOW);
 lcd.setCursor(7,0);
 lcd.print((spd)*100/255); //Displays the speed of the motor as a percentage
 lcd.setCursor(0,1);
 lcd.print ("Clockwise"); //Displays that the motor is spinning in the clockwise direction
 delay(50);
 lcd.print("                ");
}


//if the light level is below the ambient light level, the motor turns anti-clockwise
if (result < 12) {
 spd = map(val, 0, 1023, 0, 255); //The variable spd stores the value of the light level from 0 to 255
 analogWrite(Mopin2, (255-spd));
 analogWrite(Mopin1, LOW);
 lcd.setCursor(7,0);
 lcd.print((255-spd)*100/255); //Displays the speed of the motor as a percentage
 lcd.setCursor(0,1);
 lcd.print ("Anti-Clockwise"); //Displays that the motor is spinning in the anti-clockwise direction
 delay(50);
 lcd.print("                ");
}

//if the light level is at the ambient level the motor does not turn at all
if (result == 13) { //Defining ambient level as 13
  analogWrite(Mopin1, 0); //Turns the motor off 
  analogWrite(Mopin2, LOW);
  lcd.setCursor(0,1);
  lcd.print ("                ");
  delay(50);
}
}
 
}
