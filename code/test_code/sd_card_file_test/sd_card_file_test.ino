/*******************************************************************
 * Test code for sd card
 * 
 * Arduino UNO
 * Data Logger shield with SD Card
 * 
 * SD card attached to SPI bus as follows:
 * MOSI     -----> 11
 * MISO     -----> 12
 * CLK      -----> 13
 * CS       -----> 10
 * 
 * Code to check status of SD card
 * Opens a file called datalog.txt
 * Writes time pulled from RTC to the file
 * Closes the file
 * 
 * Libraries:    
 *        - Arduno SD Library: https://www.arduino.cc/reference/en/libraries/sd/
 *        
 * 2022-10-21
 * 
 *******************************************************************/

#include <SPI.h>
#include <SD.h>
#include <RTClib.h>

RTC_DS1307 rtc;

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

const int chipSelect = 10;    // CS pin in definition

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;       //  wait for serial port to open
  }
  
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) 
  {
    Serial.println("RTC is NOT running, set the time!");
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // Set the RTC with an explicit date & time, e.g to set January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  Serial.print("\nInitializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  else
  {
    Serial.println("Wiring is correct and a card is present.");
  }

}

void loop(void)
{
  // Make a strig to assemble the date in
  String dataString = "";
  
  DateTime now = rtc.now();

  dataString = "Now it's, ";
  dataString += now.year();
  dataString += "/";
  dataString += now.month();
  dataString += "/";
  dataString += now.day();
  dataString += " at";
  dataString += ';';
  dataString += " "; 
  dataString += now.hour();
  dataString += ":";
  dataString += now.minute();
  dataString += ":";
  dataString += now.second();

  
  // Open the file. NOte only one file open at a time and it needs to be closed before opening another
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // Write to the file, if available
  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
    delay(1000);
  }
  else
  {
    Serial.println("error opening datalog.txt");        // SHow error if file did not open
  }
}
