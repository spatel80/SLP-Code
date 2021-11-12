#include <sps30.h>
#include <DFRobot_DHT20.h>

DFRobot_DHT20 dht20;
void setup(){
  Serial.begin(9600);
  delay(2000);

  //Sensor Initializations
  sensirion_i2c_init(); //Note: SDA & SCL are pins A4 & A5 on Arduino
  sps30_start_measurement();
  //dht20.begin();
  }

void loop(){

  //SPS30 (Particulate Matter Sensor)
  if (true){
  struct sps30_measurement m;
  uint16_t data_ready;
  int16_t ret;
  do{
    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0){
      continue;}
    else if (!data_ready)
      Serial.print("no new measurement available\n");
    else
      break;
    delay(100);
  }
  while(1);
  ret = sps30_read_measurement(&m);
  if (ret < 0) {
    Serial.print("error reading measurement\n");
  }
  //explore SPS30 library to discover more metrics here
  Serial.print("PM 10.0: ");
  Serial.println(m.mc_10p0);
  }
  
  //DHT20 (Temperature/Humidity Sensor): possible challenge: different baud rates for different sensors
  if (false){
  Serial.print("temperature:"); Serial.print(dht20.getTemperature());Serial.print("C");
  Serial.print("  humidity:"); Serial.print(dht20.getHumidity()*100);Serial.println(" %RH");
  }
}
