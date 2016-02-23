/*
   Tomasz Pasierbski
   Scetch dedicated to bootloader:  Arduino Pro Mini 
   Tools: AtMega328 8MHz 3,3V
 */
#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9);  // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
#include <Time.h>

const int  btnForward = 2;  // button to move time forward
const int  btnBack = 0;     // button to move time back
const int ekran = 3;  int WlaczWylEkran = 1;
const int analog = A0;

const int przesuniecie = 12;
const int przesuniecied = 8;

// Variables will change:
int OledState = LOW;        // the current state of the output pin
int lastButtonState = LOW;   // the previous reading from the input pin


unsigned long  prevtime;   // when the clock was last displayed

void setup()
{
  pinMode(btnForward, INPUT_PULLUP);
  pinMode(btnBack, INPUT_PULLUP);
  pinMode(ekran, INPUT_PULLUP);
  setTime(7,43,0,19,2,16); // start with the time set to noon Jan 1 2011

//pinMode (0, OUTPUT);
pinMode (1, OUTPUT);
pinMode (4, OUTPUT);
pinMode (5, OUTPUT);
pinMode (6, OUTPUT);
pinMode (7, OUTPUT);
pinMode (12, OUTPUT);

}

void loop()
{
  WlaczWylEkran = digitalRead(ekran);
  
  prevtime = now();   // note the time
  while( prevtime == now() )    // stay in this loop till the second changes
  {
      // check if the set button pressed while waiting for second to roll over
     if(checkSetTime())
         prevtime = now();   //  time changed so reset start time
  }
//  digitalClockDisplay();

    u8g.firstPage();  
  do 
  {
      draw();
  } 
  
  while( u8g.nextPage() );


if (WlaczWylEkran == LOW)
{
  OledState = !OledState;
}

  if (OledState == HIGH)
  {
    u8g.sleepOn();
  }
  if (OledState == LOW)
  {
    u8g.sleepOff();
  }

//ADC Off
ADCSRA &= ~(1<<7);

//Sleep mode
SMCR |= (1<<2);
SMCR |= 1;

//BOD Disable
MCUCR |= (3<<5);
MCUCR = (MCUCR & ~(1<<5)) | (1<<6);

//__asm__ __volatile__("sleep");


}


 
// functions checks to see if the time should be adjusted
// returns true if time was changed
boolean checkSetTime()
{
int step;   // the number of seconds to move (backwards if negative 
boolean isTimeAdjusted = false;  // set to true if the time is adjusted
  step = 1;   // ready to step forwards
  while(digitalRead(btnForward)== LOW)
  {
     adjustTime(step);
     isTimeAdjusted = true; // to tell the user that the time has changed
     step = step + 1; // next step will be bigger
//     digitalClockDisplay(); // update clock   
     delay(100); 
  }
  step = -1;   // negative numbers step backwards
  while(digitalRead(btnBack)== LOW)
  {
     adjustTime(step);
     isTimeAdjusted = true; // to tell the user that the time has changed
     step = step - 1; // next step will be a bigger negative number
     //digitalClockDisplay(); // update clock   
     delay(100); 
  }
  return isTimeAdjusted;  // tell the user if the time was adjusted


}

void printDigits(int digits){
  // utility function for clock display: prints preceding colon and leading 0
    if(digits < 10)
    {
      u8g.print("0");
      u8g.print(digits);
    }
   else
   {
      u8g.print(digits);
   }
}

void draw(void) 
{   
    WyswietlanieGodziny();
}

void WyswietlanieGodziny()
{
//     u8g.setFont(u8g_font_5x8);
//  int sensorValue = analogRead(analog);
//  float voltage = sensorValue * (5.0 / 1023.0);
//    u8g.setPrintPos( 10, 10);    u8g.print(voltage);
  
    u8g.drawFrame(112,2,12,8);
    u8g.drawBox(112,2,8,8);
    u8g.drawBox(124,4,2,4);
    u8g.drawLine(0, 12, 128, 12);
    
    //u8g.setFont(u8g_font_unifont);
    u8g.setFont(u8g_font_fur14);
    u8g.setPrintPos( przesuniecie + 8, 34); printDigits(hour());//u8g.print(hour());
    u8g.drawStr( przesuniecie + 26, 34, " : ");
    
    u8g.setPrintPos(przesuniecie + 40, 34); printDigits(minute());  //u8g.print(minute());
    u8g.drawStr( przesuniecie + 58, 34, " : ");
    
    u8g.setPrintPos( przesuniecie + 74, 34); printDigits(second());  // u8g.print(second());

    //u8g.setFont(u8g_font_unifont);
    u8g.setFont(u8g_font_profont12);
    u8g.setPrintPos( przesuniecied + 18, 50);    u8g.print(day());
    u8g.drawStr( przesuniecied + 28, 50, " / ");
    
    u8g.setPrintPos(przesuniecied + 45, 50);  printDigits(month()); //  u8g.print(month());  
    u8g.drawStr( przesuniecied + 54, 50, " / ");
    
    u8g.setPrintPos( przesuniecied + 70, 50);    u8g.print(year());

    
    u8g.drawLine(0, 54, 128, 54);
    u8g.setFont(u8g_font_5x8);
    u8g.drawStr( 18, 63, "tomasz.pasierbski");
}
