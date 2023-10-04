#include <Arduino.h>
#include <virtuabotixRTC.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <ezButton.h>
#include <FastLED.h>



/*
Pins usage
D4: Button
D5: LEDS
D6: RTC Clk
D7: RTC Data
D8: RTC Rst

*/

// How many leds in your strip?
#define NUM_LEDS  49

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define BUTTON_PIN 4
#define LED_DATA_PIN 5
#define RTC_Clk 6
#define RTC_Rst 7
#define RTC_Data 8


// Define the array of leds
CRGB leds[NUM_LEDS];

int r = 0;
int g = 0;
int b = 0;


/***  RTC Shit ***/
ezButton button(BUTTON_PIN);  // create ezButton object that attach to pin 7;

#define one_day 86400  //1 day has 86400s
int since_days = 0;

// Creation of the Real Time Clock Object
// virtuabotixRTC(uint8_t inSCLK, uint8_t inIO, uint8_t inC_E);
virtuabotixRTC myRTC(RTC_Clk, RTC_Data, RTC_Rst);

tmElements_t T1;
tmElements_t T2;

/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int addr = 0;


void setup() {
  Serial.begin(57600);
  Serial.println("resetting");
  FastLED.addLeds<WS2812, LED_DATA_PIN , GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(84);

   myRTC.updateTime();

  T1.Hour = myRTC.hours;
  T1.Minute = myRTC.minutes;
  T1.Second = myRTC.seconds;
  T1.Day = myRTC.dayofmonth;
  T1.Month = myRTC.month;
  T1.Year = myRTC.year - 1970; // because Year is offset from 1970

  /*
    //Uncomment this block to update the date-time
    // Set the current date, and time in the following format:
    // seconds, minutes, hours, day of the week, day of the month, month, year
    myRTC.setDS1302Time(00, 07, 13, 4, 6, 9, 2023);
    myRTC.updateTime();

  */

  EEPROM.put(addr, T1); // EEPROM.put(eeAddress, customVar);
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
}

//void fadeall() { for(int i = INILED; i < ENDLED; i++) { leds[i].nscale8(250); } }

void calculateRedToGreen(int value) {
  // Ensure the input value is within the range 0-99
  value = constrain(value, 0, 99);

  // Calculate the red, green, and blue components based on the input value
  if (value <= 49) {
    // Transition from red to blue
    r = map(value, 0, 49, 255, 0);
    g = 0;
    b = map(value, 0, 49, 0, 255);
  } else {
    // Transition from blue to green
    r = 0;
    g = map(value, 50, 99, 0, 255);
    b = map(value, 50, 99, 255, 0);
  }
}

void writeSegment(int start_led, int end_led)
{
  for (int i = start_led; i < end_led; i++) {
    leds[i].setRGB(r, g, b);
  }
}


void Units (int i) {
/*
   See how the segments of a 7-seg display are named
   Units Display
   A2 40-43
   B2 37-40
   C2 32-35
   D2 29-32
   E2 26-29
   F2 43-46
   G2 46-49
*/
  switch (i) {
    case 0:
      writeSegment(40, 43); //Segment A
      writeSegment(37, 40); //Segment B
      writeSegment(32, 35); //Segment C
      writeSegment(29, 32); //Segment D
      writeSegment(26, 29); //Segment E
      writeSegment(43, 46); //Segment F

      break;

    case 1:
      writeSegment(37, 40); //Segment B
      writeSegment(32, 35); //Segment C
      break;

    case 2:
      writeSegment(40, 43); //Segment A
      writeSegment(37, 40); //Segment B
      writeSegment(29, 32); //Segment D
      writeSegment(26, 29); //Segment E
      writeSegment(46, 49); //Segment G
      break;

    case 3:
      writeSegment(40, 43); //Segment A
      writeSegment(37, 40); //Segment B
      writeSegment(32, 35); //Segment C
      writeSegment(29, 32); //Segment D

      writeSegment(46, 49); //Segment G
      break;

    case 4:
      writeSegment(37, 40); //Segment B
      writeSegment(32, 35); //Segment C
      writeSegment(43, 46); //Segment F
      writeSegment(46, 49); //Segment G
      break;

    case 5:
      writeSegment(40, 43); //Segment A
      writeSegment(32, 35); //Segment C
      writeSegment(29, 32); //Segment D
      writeSegment(43, 46); //Segment F
      writeSegment(46, 49); //Segment G
      break;

    case 6:
      writeSegment(40, 43); //Segment A
      writeSegment(32, 35); //Segment C
      writeSegment(29, 32); //Segment D
      writeSegment(26, 29); //Segment E
      writeSegment(43, 46); //Segment F
      writeSegment(46, 49); //Segment G
      break;

    case 7:
      writeSegment(40, 43); //Segment A
      writeSegment(37, 40); //Segment B
      writeSegment(32, 35); //Segment C
      break;

    case 8:
      writeSegment(40, 43); //Segment A
      writeSegment(37, 40); //Segment B
      writeSegment(32, 35); //Segment C
      writeSegment(29, 32); //Segment D
      writeSegment(26, 29); //Segment E
      writeSegment(43, 46); //Segment F
      writeSegment(46, 49); //Segment G
      break;

    case 9:
      writeSegment(40, 43); //Segment A
      writeSegment(37, 40); //Segment B
      writeSegment(32, 35); //Segment C
      writeSegment(29, 32); //Segment D
      writeSegment(43, 46); //Segment F
      writeSegment(46, 49); //Segment G
      break;

    case 99:
      r = 0;
      g = 0;
      b = 0;
      writeSegment(40, 43); //Segment A
      writeSegment(37, 40); //Segment B
      writeSegment(32, 35); //Segment C
      writeSegment(29, 32); //Segment D
      writeSegment(26, 29); //Segment E
      writeSegment(43, 46); //Segment F
      writeSegment(46, 49); //Segment G
      break;
  }//switch
}


void Tens (int i) {
/*
   See how the segments of a 7-seg display are named
   10ths display:
   A1 6-9
   B1 3-6
   C1 20-23
   D1 17-20
   E1 14-17
   F1 9-12
   G1 0-3
*/

  switch (i) {
    case 0:
      writeSegment(6, 9); //Segment A
      writeSegment(3, 6); //Segment B
      writeSegment(20, 23); //Segment C
      writeSegment(17, 20); //Segment D
      writeSegment(14, 17); //Segment E
      writeSegment(9, 12); //Segment F
      break;

    case 1:
      writeSegment(3, 6); //Segment B
      writeSegment(20, 23); //Segment C
      break;

    case 2:
      writeSegment(6, 9); //Segment A
      writeSegment(3, 6); //Segment B
      writeSegment(17, 20); //Segment D
      writeSegment(14, 17); //Segment E
      writeSegment(0, 3); //Segment G
      break;

    case 3:
      writeSegment(6, 9); //Segment A
      writeSegment(3, 6); //Segment B
      writeSegment(20, 23); //Segment C
      writeSegment(17, 20); //Segment D
      writeSegment(0, 3); //Segment G
      break;

    case 4:
      writeSegment(3, 6); //Segment B
      writeSegment(20, 23); //Segment C
      writeSegment(9, 12); //Segment F
      writeSegment(0, 3); //Segment G
      break;

    case 5:
      writeSegment(6, 9); //Segment A
      writeSegment(20, 23); //Segment C
      writeSegment(17, 20); //Segment D
      writeSegment(9, 12); //Segment F
      writeSegment(0, 3); //Segment G
      break;

    case 6:
      writeSegment(6, 9); //Segment A
      writeSegment(20, 23); //Segment C
      writeSegment(17, 20); //Segment D
      writeSegment(14, 17); //Segment E
      writeSegment(9, 12); //Segment F
      writeSegment(0, 3); //Segment G
      break;

    case 7:
      writeSegment(6, 9); //Segment A
      writeSegment(3, 6); //Segment B
      writeSegment(20, 23); //Segment C
      break;

    case 8:
      writeSegment(6, 9); //Segment A
      writeSegment(3, 6); //Segment B
      writeSegment(20, 23); //Segment C
      writeSegment(17, 20); //Segment D
      writeSegment(14, 17); //Segment E
      writeSegment(9, 12); //Segment F
      writeSegment(0, 3); //Segment G
      break;

    case 9:
      writeSegment(6, 9); //Segment A
      writeSegment(3, 6); //Segment B
      writeSegment(20, 23); //Segment C
      writeSegment(17, 20); //Segment D
      writeSegment(9, 12); //Segment F
      writeSegment(0, 3); //Segment G
      break;

    case 99:
      r = 0;
      g = 0;
      b = 0;
      writeSegment(6, 9); //Segment A
      writeSegment(3, 6); //Segment B
      writeSegment(20, 23); //Segment C
      writeSegment(17, 20); //Segment D
      writeSegment(14, 17); //Segment E
      writeSegment(9, 12); //Segment F
      writeSegment(0, 3); //Segment G
      break;
  }//switch
}

void Sender (int num)
{
  int tens = int(num / 10.0);
  int units = num - tens * 10;
  Units(99);
  Tens(99);
  FastLED.show();
  calculateRedToGreen(num);
  Units(units);
  Tens(tens);
  FastLED.show();
}

/***RTC Function***/
void compute_RTC(void)
{
// This allows for the update of variables for time or accessing the individual elements.
  myRTC.updateTime();
  // get T2
  T2.Hour = myRTC.hours;
  T2.Minute = myRTC.minutes;
  T2.Second = myRTC.seconds;
  T2.Day = myRTC.dayofmonth;
  T2.Month = myRTC.month;
  T2.Year = myRTC.year - 1970; // because Year is offset from 1970

  /*
    // Start printing elements as individuals
    Serial.print("Current Date / Time: ");
    Serial.print(myRTC.dayofmonth);
    Serial.print("/");
    Serial.print(myRTC.month);
    Serial.print("/");
    Serial.print(myRTC.year);
    Serial.print("  ");
    Serial.print(myRTC.hours);
    Serial.print(":");
    Serial.print(myRTC.minutes);
    Serial.print(":");
    Serial.println(myRTC.seconds);
  */

  // Test EPROM get fcn
  EEPROM.get(0, T1); // EEPROM.get(eeAddress, customVar);
  // convert T1 and T2 to seconds since 1/1/1970
  time_t T1sec = makeTime( T1 );
  time_t T2sec = makeTime( T2 );
  Serial.print( "T1 in seconds since 1970: " );
  Serial.println( T1sec );
  Serial.print( "T2 in seconds since 1970: " );
  Serial.println( T2sec );

  // differences in seconds
  int32_t diff = T2sec - T1sec;
  Serial.print( "Difference between T1 and T2 in seconds: " );
  Serial.println( diff );

  since_days = int(diff / one_day);
  Serial.print( "*** Days since last shit:  " );
  Serial.println( since_days );

  // show difference in hours, minutes and seconds
  uint32_t hours = diff / 3600;
  uint8_t minutes = (diff / 60) % 60;
  uint8_t seconds = diff % 60;
  Serial.print( "T1 was " );
  Serial.print( hours );
  Serial.print( " hour(s), " );
  Serial.print( minutes );
  Serial.print( " minute(s) and " );
  Serial.print( seconds );
  Serial.println( " second(s) before T2." );

  // Delay so the program doesn't print non-stop
  //delay( 1000);
  button.loop(); // MUST call the loop() function first

  if (button.isPressed())
  {
    Serial.println("***************************The button is pressed********************************");
    myRTC.updateTime();

    T1.Hour = myRTC.hours;
    T1.Minute = myRTC.minutes;
    T1.Second = myRTC.seconds;
    T1.Day = myRTC.dayofmonth;
    T1.Month = myRTC.month;
    T1.Year = myRTC.year - 1970; // because Year is offset from 1970
    EEPROM.put(addr, T1); // EEPROM.put(eeAddress, customVar);
    button.setDebounceTime(50); // set debounce time to 50 milliseconds
  }

  delay(1000);
  }

void loop() {
  for (int j = 0; j <= 99; j++) {
    Sender(j);
    delay (250);
  }

}