/* CNC RPM Gauge using 128 x 32 oled screen and Arduino Pro Micro
Pin Connections
2 - SDA screen
3 - SCL screen
7 - Input from sensor board
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


long rev=0;
long rpm;



void isr() //interrupt service routine
{
rev++;
}


void setup()
{
display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
pinMode(7, INPUT_PULLUP);
display.clearDisplay();

//attachInterrupt(0,isr,RISING);  //attaching the interrupt
attachInterrupt(digitalPinToInterrupt(7), isr, RISING);  //attaching the interrupt
}


void loop()
{
delay(1000);// this is how long the measurement takes
//detachInterrupt(0);           //detaches the interrupt
detachInterrupt(digitalPinToInterrupt(7));           //detaches the interrupt
 
rpm=rev*60;

rev=0;
ShowToScreen();
//attachInterrupt(0,isr,RISING);
attachInterrupt(digitalPinToInterrupt(7), isr, RISING);  //attaching the interrupt
}


void ShowToScreen(void) {
  display.clearDisplay();

  display.setTextSize(3); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  if(rpm<10){
     display.setCursor(58, 8);
  }
  if((rpm>9)&&(rpm<100)){
    display.setCursor(48, 8);
  }
  if((rpm>99)&&(rpm<1000)){
    display.setCursor(38, 8);
  }
  if((rpm>999)&&(rpm<10000)){
    display.setCursor(28, 8);
  }
  if((rpm>9999)&&(rpm<100000)){
    display.setCursor(18, 8);
  }
  if(rpm>99999){
    display.setCursor(0, 8);
  }
  
 
  display.println(rpm);
  display.display();      // Show the text
}
