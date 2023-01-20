/*
  

*/

#include <SoftwareSerial.h>  // https://github.com/PaulStoffregen/SoftwareSerial
#include <SPI.h>
#include <SD.h>

#define RX        8    //Serial Receive pin
#define TX        9    //Serial Transmit pin
#define RTS_pin    7    //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

SoftwareSerial RS485Serial(RX, TX);

const int chipSelect = 10;

void setup() {

  pinMode(RTS_pin, OUTPUT);  
  
  // Start the built-in serial port, for Serial Monitor
  Serial.begin(9600);
  Serial.println("Soil_sensor"); 

  // Start the Modbus serial Port, for Soil_sensor
  RS485Serial.begin(4800);   
  delay(1000);

  // Initialize SD Card
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {

  String dataString = "";

  digitalWrite(RTS_pin, RS485Transmit);     // init Transmit
  byte Soil_sensor_request[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x04, 0x44, 0x09}; // inquiry frame
  RS485Serial.write(Soil_sensor_request, sizeof(Soil_sensor_request));
  RS485Serial.flush();
  
  digitalWrite(RTS_pin, RS485Receive);      // Init Receive
  byte Soil_sensor_buf[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  RS485Serial.readBytes(Soil_sensor_buf, 16);
 
  Serial.print("Sensor reading : ");
  for( byte i=0; i<15; i++ ) {
  Serial.print(Soil_sensor_buf[i], HEX);
  Serial.print(" ");
  }
  Serial.println();
  Serial.print("Humidity --> ");
  float humidity = word(Soil_sensor_buf[3], Soil_sensor_buf[4]);
  humidity = humidity/10;
  Serial.print(humidity);
  Serial.print("   ");
  //Serial.println();
  Serial.print("Temperature --> ");
  float temp = word(Soil_sensor_buf[5], Soil_sensor_buf[6]);
  temp = temp/10;
  Serial.print(temp);
  Serial.print("   ");
  //Serial.println();
  Serial.print("Conductivity --> ");
  int conductivity = word(Soil_sensor_buf[7], Soil_sensor_buf[8]);
  Serial.print(conductivity);
  Serial.print("   ");
  //Serial.println();
  Serial.print("PH --> ");
  int ph = word(Soil_sensor_buf[9], Soil_sensor_buf[10]);
  ph = ph/10;
  Serial.print(ph);
  Serial.println();

  dataString += humidity;
  dataString +=",";
  dataString += temp;
  dataString +=",";
  dataString += conductivity;
  dataString +=",";
  dataString += ph;
  dataString +=",";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

   // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
                   
  delay(100);

}
