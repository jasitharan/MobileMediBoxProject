/*
 * MobiKit.c
 *
 * Created: 11/8/2020 3:49:30 PM
 * Author : jasit
 */ 

#define F_CPU 8000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "keypad.h"
#include "i2cmaster.h"
#include "i2c_lcd.h"

#define TEMP_PORT_PIN 0b10000000
#define OXY_PORT_PIN 0b00100000
#define HEART_PORT_PIN 0b00010000
#define ECG_PORT_PIN 0b00001000

int age = 0;  // Age
int gender = 0;  // Gender
int num = 0;
Keypad key;
char *result = "72bpm"; // Final Result
int selected = -1;

int *p1 = &PORTD;
int *p2 = &PORTB;


void getAge();
void getGender();
void sensor();
void sensorInstruction(int n,char *c);
void resultFunc();

void pushSensor(int *port,int n);
void pullSensor(int *port,int n);


int main(void)
{
	//OC For Servo Motors
	DDRD = 0b10110000; //make output oc pins
	DDRB = 0b00001000; //make output oc pins
	
	PORTD = 0x00;
	PORTB = 0x00;
	

	
	
	keypad_init();                                  // KEYPAD Initialize
	lcd_init(LCD_BACKLIGHT_ON);                     // LCD Initialize
	
	
	lcd_clear();			/* Clear LCD */
	_delay_ms(2000);
	lcd_goto_xy(0,0);		/* Enter column and row position */
	lcd_puts("WELCOME...." );
	_delay_ms(2000);
	lcd_puts_at("Mobile Medi Box",0,1);
	_delay_ms(2000);
	lcd_clear();
	_delay_ms(300);
	
	while (1)
	{
		getAge();
		getGender();
		sensor();
		if (selected != -1) resultFunc();
	}
}



void getAge()
{
	
	int i = 0;
	lcd_clear();
	_delay_ms(100);
	lcd_goto_xy(0,0);
	lcd_puts("Age: ");
	_delay_ms(100);
	lcd_goto_xy(0,1);
	while(1){
		char *numString;
		key = waitForKeyPress();
		if (key == BTN_OK) {
			age = num;
			lcd_clear();
			_delay_ms(50);
			break;
		}
		sprintf(numString,"%d",keypad_get_number(key));
		num *= 10;
		num += keypad_get_number(key);
		lcd_puts(numString);
		_delay_ms(100);
		lcd_goto_xy(i++,1);
	}
}


  void getGender()
  {
	  _delay_ms(100);
	  lcd_goto_xy(0,0);
	  lcd_puts("Gender: ");
	  lcd_puts_at("0.Male 1.Female",0,1);
	  key = waitForKeyPress();
	  if (key == BTN_0) gender = 0;
	  if (key == BTN_1) gender = 1;
	  _delay_ms(100);
	  lcd_clear();
	  _delay_ms(100);
  }
  
  void sensor()
  {
	  _delay_ms(100);
	  lcd_goto_xy(0,0);
	  lcd_puts("Select: ");
	  lcd_puts_at("0.T 1.O 2.E 3.H",0,1);
	  key = waitForKeyPress();
	  switch(key)
	  {
		  case BTN_0:
		  sensorInstruction(0,"Place Your Finger");
		  // Push The Sensor code
		  pushSensor(p1,TEMP_PORT_PIN);
		  
		  // Temperature Sensor Code 
		  
		  break;
		  case BTN_1:
		  sensorInstruction(1,"Place Your Finger");
		  // Push The Sensor code
		  pushSensor(p1,OXY_PORT_PIN);
		  // Oxygen Saturation Code
		  
		  break;
		  case BTN_2:
		  sensorInstruction(2,"Place Your Thump");
		  // Push The Sensor code
		  pushSensor(p2,ECG_PORT_PIN);
		  
		  // ECG Code
		  
		  break;
		  case BTN_3:
		  sensorInstruction(3,"Put it on your chest");
		  // Push The Sensor code
		  pushSensor(p1,HEART_PORT_PIN);
		  
		  // Heart Rate Code
		  
		  break;
	      default:
		  lcd_puts_at("Wrong input",0,0);
		  _delay_ms(200);
		 //continue;
		  break;
	  }
  }
  
  void sensorInstruction(int n,char *c)
  {
	  selected = n;
	  lcd_clear();
	  _delay_ms(100);
	  lcd_goto_xy(0,0);
	  lcd_puts(c);
  }
  
  void resultFunc()
  {
	  _delay_ms(2000);
	  lcd_clear();
	  lcd_goto_xy(0,0);
	  lcd_puts("Result: ");
	  lcd_puts_at(result,0,1);
	  _delay_ms(10000);
	  switch(selected)
	  {
		  case 0:
		  pullSensor(p1,TEMP_PORT_PIN);
		  break;
		  case 1:
		  pullSensor(p1,OXY_PORT_PIN);
		  break;
		  case 2:
		  pullSensor(p2,ECG_PORT_PIN);
		  break;
		  case 3:
		  pullSensor(p1,HEART_PORT_PIN);
		  break;
		  default:
		  break;
		   
	  }
	  
  }
  
  
  
  void pushSensor(int *port,int n)
  {
	  //Rotate Motor to 0 degree
	  *port = n;
	  _delay_us(1000);
	  *port = 0x00;
	  
	  _delay_ms(500);
	  
	  //Rotate Motor to 90 degree
	  *port = n;
	  _delay_us(1500);
	  *port = 0x00;
	  
	  _delay_ms(500);
	  
	  //Rotate Motor to 180 degree
	  *port = n;
	  _delay_us(2000);
	  *port = 0x00;
	  
	  _delay_ms(500);
	  
  }

  void pullSensor(int *port,int n)
  {
	  
	  //Rotate Motor to 0 degree
	  port = n;
	  _delay_us(1000);
	  port = 0x00;
	  
  }