/*
 * MobiKit.c
 *
 * Created: 11/8/2020 3:49:30 PM
 * Author : jasitharan
 */ 

#define F_CPU 8000000UL // Define CPU Frequency 8MHz
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "keypad.h"
#include "i2cmaster.h"
#include "i2c_lcd.h"


// Servo Motors Pins
#define TEMP_SENSOR 0b00000000
#define OXY_SENSOR 0b00000010
#define HEART_SENSOR 0b00000001
#define ECG_SENSOR 0b00000011

int age = 0;  // Age
int gender = 0;  // Gender
int num = 0;
Keypad key;





void getAge();
void getGender();
void sensor();
void sensorInstruction(char *c);


void pushSensor(int n);
void pullSensor(int n);


int main(void)
{
	
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
		getAge();                         // For Getting Age
		getGender();                      // For Getting Gender
		sensor();                         // For Sensor Selection And Functions
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
		num *= 10;      // num = 0 initial value
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
		  sensorInstruction("Place Your Finger");
		  // Push The Sensor code
		  pushSensor(TEMP_SENSOR);
		  
		  // Temperature Sensor Code 
		  
		  
		  _delay_ms(10000);
		  pushSensor(TEMP_SENSOR);
		  break;
		  case BTN_1:
		  sensorInstruction("Place Your Finger");
		  // Push The Sensor code
		  pushSensor(OXY_SENSOR);
		  // Oxygen Saturation Code
		  
		   _delay_ms(10000);
		   pushSensor(OXY_SENSOR);
		  break;
		  case BTN_2:
		  sensorInstruction("Place Your Thump");
		  // Push The Sensor code
		  pushSensor(ECG_SENSOR);
		  
		  // ECG Code
		  
		  
		   _delay_ms(10000);
		   pushSensor(ECG_SENSOR);
		  break;
		  case BTN_3:
		  sensorInstruction("Put it on your chest");
		  // Push The Sensor code
		  pushSensor(HEART_SENSOR);
		  
		  // Heart Rate Code
		  
		  
		   _delay_ms(10000);
		   pushSensor(HEART_SENSOR);
		  break;
	      default:
		  lcd_puts_at("Wrong input",0,0);
		  _delay_ms(200);
		 //continue;
		  break;
	  }
  }
  
  void sensorInstruction(char *c)
  {
	  lcd_clear();
	  _delay_ms(100);
	  lcd_goto_xy(0,0);
	  lcd_puts(c);
  }
  
  

  
  
  
  void pushSensor(int n)
  {
	 PORTD = n;
	 DDRD |= 0b00100011;
     TCCR1A |= 1<<WGM11 | 1<<COM1A1;
     TCCR1B |= 1<<WGM13 | 1<<WGM12 | 1<<CS10 | 1<<CS11;
     ICR1 = 2499;

     OCR1A = 65;	/* Set servo shaft at -90° position */
    _delay_ms(1500);
     OCR1A = 175;	/* Set servo shaft at 0° position */
    _delay_ms(1500);
     OCR1A = 300;	/* Set servo at +90° position */
    _delay_ms(1500);
	  
  }

  void pullSensor(int n)
  {
	  
	  PORTD = n;
	  DDRD |= 0b00100011;
	  TCCR1A |= 1<<WGM11 | 1<<COM1A1;
	  TCCR1B |= 1<<WGM13 | 1<<WGM12 | 1<<CS10 | 1<<CS11;
	  ICR1 = 2499;

	  OCR1A = 65;	/* Set servo shaft at -90° position */
	  _delay_ms(1500);
	  
  }