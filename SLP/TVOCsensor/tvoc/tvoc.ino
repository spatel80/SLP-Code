#include <Adafruit_CCS811.h>

Adafruit_CCS811 ccs;

void setup() {
  serial.begin(9600);
  ccs.begin();
  //calibrate temperature sensor
  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);
}

void loop(){
  if(ccs.available()){
    float temp = ccs.calculateTemperature();

    if(!ccs.readData()){
      int co2 = ccs.geteCO2();
      int tvoc = ccs.getTVOC();
      serial.println(tvoc);
      serial.println(co2);
      delay(3000);    
    }

    else{
      serial.println("ERROR");
      while(1);
    }
  }
}
