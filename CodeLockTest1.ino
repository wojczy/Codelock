// Color Code Lock with Neopixel Ring and Rotary Encoder

#include <Adafruit_NeoPixel.h>
#define PIN 6 // D6 to DATA pin NEOPIXEL
#define NUMPIXELS 12 // NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800); // 4 color LEDs
// pin connection for Rotary Encoder
int CLK = 5;//CLK->D5
int DT = 4;//DT->D4
int SW = 3;//SW->D3
const int interrupt0 = 0;// Interrupt 0 pin 5
int count = 0;//Initial count
int countplus = 1; //Initial count plus one
int countminus = 11; //Initial count minus one
int lastCLK = 0;//CLK initial value 
int mcode[6] = {0, 1, 2, 3, 4, 5}; // Initial Master Code
int tcode[6] = {0, 2, 5, 0, 2, 5}; // Initial Temporary Code
int ccode[6] = {6, 6, 6, 6, 6, 6}; // Initial Current Code (flagged invalid because not 0...5)
int rval[6] = {0, 0  , 25, 0  , 0  , 25}; // red value of colors 1 to 6 (black, dim white, red, green, blue, yellow)
int gval[6] = {0, 0  , 0  , 25, 0  , 25}; // green value of colors 1 to 6 (black, dim white, red, green, blue, yellow)
int bval[6] = {0, 0  , 0  , 0  , 25, 0}; // blue value of colors 1 to 6 (black, dim white, red, green, blue, yellow)
int wval[6] = {0, 25, 0  , 0  , 0  , 0}; // white value of colors 1 to 6 (black, dim white, red, green, blue, yellow)
int dcolors[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //( initial colors of ring 0 to 11 (all black)
int selcolor = 6; // selected color (flagged invalid because not 0...5)
bool rotdirpv = false; // previous direction of roatation; FALSE = counter clockwise
bool rotdirac = false; // actual direction of roatation; FALSE = counter clockwise
bool rotdirch = false; // has direction of roation changed?
int rotdirct = -1; // number of direction changes
bool codefin = false; // entering of code finished? 
bool locked = true; // lock is unlocked


void setup()
{
  pixels.begin(); // INITIALIZE NeoPixel strip object
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(10, OUTPUT); // magnet for lock
  attachInterrupt(interrupt0, ClockChanged, CHANGE);//Set the interrupt 0 handler, trigger level change
  Serial.begin(9600);
  delay(1000);//wait for a second
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();  
}
 
void loop()
{

  
  if (!digitalRead(SW)) //Read the button press and the count value to 0 when the counter reset
  {
    count = 0;
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();  
    Serial.print("count:");
    Serial.println(count);
  }


// Routine after all numbers selected
if (rotdirct > 5)
  {
 delay(100);
 pixels.clear(); // Set all pixel colors to 'off'
 pixels.show(); 
 locked = false;
 for (int i = 0; i < 6; i++)
  {
  if (ccode[i] != tcode[i])
     {
    locked = true;
     }
  }
   if (locked == false)
          {
      Serial.println("unlocked");
        for (int i = -1; i < 7; i++)
      {
      pixels.setPixelColor(i, pixels.Color(0, 255, 0, 0));
      pixels.setPixelColor(11-i, pixels.Color(0, 255, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(500);
      }
      digitalWrite(10, HIGH);
      delay(5000);
      digitalWrite(10, LOW);
      pixels.clear(); // Set all pixel colors to 'off'
      pixels.show(); 
          }
  else
  {
      Serial.println("wrong code");
      for (int i = -1; i < 6; i++)
      {
      pixels.setPixelColor(7+i, pixels.Color(255, 0, 0, 0));
      pixels.setPixelColor(4-i, pixels.Color(255, 0, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(500);
      }
      delay(1000);
      pixels.clear(); // Set all pixel colors to 'off'
      pixels.show(); 
  }
   }
 
  
}
 
//The interrupt handlers
void ClockChanged()
{




  
  int clkValue = digitalRead(CLK);//Read the CLK pin level
  int dtValue = digitalRead(DT);//Read the DT pin level
  if (lastCLK != clkValue)
  {
    lastCLK = clkValue;
    count += (clkValue != dtValue ? -1 : 1);//CLK and inconsistent DT + 1, otherwise - 1
        if (clkValue != dtValue)
    { 
          rotdirac = false;
    }
    else
    {
          rotdirac = true;
    }
    if (rotdirac != rotdirpv)
    { 
          rotdirch = true;

          if (-1 < rotdirct && rotdirct < 6)
          {
            ccode[rotdirct] = selcolor;
          }
                    rotdirct = rotdirct + 1;
    }
        else
    {
          rotdirch = false;
    }
    rotdirpv = rotdirac;

    if (count > 11)  //count pixel boundary condition
 {
  count = 0; 
 }
 if (count < 0)
 {
  count = 11; 
 }
    countplus = count + 1;
    countminus = count - 1;

if (count > 11)  //count pixel boundary condition
 {
  count = 0; 
 }
 if (count < 0)
 {
  count = 11; 
 }
 
 if (countminus > 11)  //count pixel minus one boundary condition
 {
  countminus = 0; 
 }
 if (countminus < 0)
 {
  countminus = 11; 
 }

  if (countplus > 11)  //count pixel plus one boundary condition
 {
  countplus = 0; 
 }
 if (countplus < 0)
 {
  countplus = 11; 
 }
 int dcolors[12] = {random(0,6), random(0,6), random(0,6), random(0,6), random(0,6), random(0,6), random(0,6), random(0,6), random(0,6), random(0,6), random(0,6), random(0,6)};
 pixels.clear(); // Set all pixel colors to 'off'
 for (int i = 0; i < 12; i++)
 {
  pixels.setPixelColor(i, pixels.Color(rval[dcolors[i]], gval[dcolors[i]], bval[dcolors[i]], wval[dcolors[i]]));
 }

 pixels.setPixelColor(countminus, pixels.Color(225, 0, 225, 5));
 pixels.setPixelColor(countplus, pixels.Color(225, 0, 225, 05));
 pixels.show();   // Send the updated pixel colors to the hardware.

selcolor = dcolors[count];
  }
}
