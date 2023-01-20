/*
Soil_sensor with a RS485 wind sensor

from an idea of https://arduino.stackexchange.com/questions/62327/cannot-read-modbus-data-repetitively
https://www.cupidcontrols.com/2015/10/software-serial-modbus-master-over-rs485-transceiver/

_________________________________________________________________
|                                                               |
|       author : Philippe de Craene <dcphilippe@yahoo.fr        |
|       Any feedback is welcome                                 |
                                                                |
_________________________________________________________________

Materials :
 1* Arduino Uno R3 - tested with IDE version 1.8.7 and 1.8.9
 1* wind sensor - RS485 MODBUS protocol of communication
 1* MAX485 DIP8

Versions chronology:
version 1 - 7 sept  2019   - first test 

*/

#include <SoftwareSerial.h>  // https://github.com/PaulStoffregen/SoftwareSerial

#define RX        8    //Serial Receive pin
#define TX        9    //Serial Transmit pin
#define RTS_pin    7    //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

SoftwareSerial RS485Serial(RX, TX);

void setup() {

  pinMode(RTS_pin, OUTPUT);  
  
  // Start the built-in serial port, for Serial Monitor
  Serial.begin(9600);
  Serial.println("Soil_sensor"); 

  // Start the Modbus serial Port, for Soil_sensor
  RS485Serial.begin(4800);   
  delay(1000);
}

void loop() {

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
                   
  delay(100);

}
