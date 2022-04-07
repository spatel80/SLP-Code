//library imports
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <sps30.h>
#include <RTClib.h>
#include <DFRobot_DHT20.h>
#include "SparkFunCCS811.h"
#define CCS811_ADDR 0x5B //Default I2C Address

const int chipSelect = 10;    //override system default
RTC_DS1307 rtc;               //real-time-clock object
DFRobot_DHT20 dht20;          //temperature/humidity sensor
CCS811 mySensor(CCS811_ADDR); //tvoc sensor


void setup () {
  Serial.begin(9600);
  Serial.println("Initializing System...");
  
  rtc.begin();                                     //activate real-time-clock
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//set RTC to date & time of sketch compilation, this must be commented out after sketch is ready to run
  SD.begin(chipSelect);                            //activate SD card reader

  //Wire.begin();                                  //initialize i2c hardware (for tvoc sensor)
  mySensor.begin();                              //initialize tvoc sensor
  //dht20.begin();                               //activate temperature sensor
  sensirion_i2c_init();                          //activate particulate matter sensor
  sps30_start_measurement();                     //activate particulate matter measurements
  Serial.println("System Active!");
  }

void loop () {

    //string that will store all data for saving
    String dataString;
  
    //Timestamp generation
    DateTime now = rtc.now();
    String timestamp = String(now.year(),DEC)+"/"+
                       String(now.month(),DEC)+"/"+
                       String(now.day(),DEC)+" "+
                       String(now.hour(),DEC)+":"+
                       String(now.minute(),DEC)+":"+
                       String(now.second(),DEC)+", ";
    dataString += timestamp;

    //DHT20 data collection
    String tempHumid = "temperature: "+String(dht20.getTemperature())+"C, humidity: "+String(dht20.getHumidity()*100)+" %RH, ";
    dataString += tempHumid;

    //CCS811 data collection
    String tvoc;
    //if (mySensor.dataAvailable()){ 
    mySensor.readAlgorithmResults();
    tvoc = " CO2: "+String(mySensor.getCO2())+", TVOC: "+String(mySensor.getTVOC());
    //}
    dataString += tvoc;
    
    //SPS30 data collection
    struct sps30_measurement m;  //particulate matter measurement object
    uint16_t data_ready;         //to check if data is ready for collection
    int16_t ret;
    String partMatter;
    while(1) {
      ret = sps30_read_data_ready(&data_ready);
      if (ret < 0){
        Serial.print("error reading data-ready flag: ");
      }
      else if (!data_ready){
        Serial.print("data not ready, no new measurement available\n");
      }
      else{
        break;
      }
    delay(100);
    }
    sps30_read_measurement(&m);
    partMatter = "PM 1.0: "+String(m.mc_1p0)+", PM 2.5: "+String(m.mc_2p5)+", PM 4.0: "+String(m.mc_4p0)+", PM 10.0: "+String(m.mc_10p0)+",";
    dataString += partMatter;

    //Datalogging
    Serial.println(dataString);
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }
    delay(5000);
}
