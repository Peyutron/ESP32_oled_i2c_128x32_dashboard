/* ESP32_Engine_Dashboard_U8g2
 * Usa la librería U8g2Lib para mostrar los relojes de velocímetro, RPM, Combustible y ADblue. 
 * También indicadores y luces de posición, largas y antinieblas  
 * Información sobre el proyecto: https://www.infotronikblog.com/2024/10/esp32-oled-128x32-velocimetro-rc.html
 * Descartado, muy lento
 * Autor: Carlos Muñoz
 * Pagina web: https://www.infotronikblog.com
 */


#include "dashboardi2c.h" 

DashboardI2c dashboard;

void setup(){
  Serial.begin(115200);

  // Inicia el 
  dashboard.init(0);

}

void loop(){
  dashboard.setRightIndicator(false);
  dashboard.setLeftIndicator(false);
  dashboard.setGear(1);
  dashboard.setLowBeamIndicator(false);
  dashboard.setHighBeamIndicator(false);
  dashboard.setFogLightIndicator(false);
  dashboard.setVolt(8.4, 8.4);


  delay(1500);

  dashboard.setRightIndicator(true);
  dashboard.setLeftIndicator(true);
  dashboard.setGear(0);
  dashboard.setLowBeamIndicator(true);
  dashboard.setHighBeamIndicator(true);
  dashboard.setFogLightIndicator(true);
  dashboard.setVolt(7.2, 8.4);

  for (int speed = 0; speed <= 10; speed++)
  {
    dashboard.setSpeed(speed * 12);
    dashboard.setRPM(speed * 50);
    dashboard.setFuelLevel(speed * 10);
    dashboard.setAdBlueLevel(speed * 10);
  }

  for (int dspeed = 10; dspeed >= 0; dspeed--){
    dashboard.setSpeed(dspeed * 12);
    dashboard.setRPM(dspeed * 50);
    dashboard.setFuelLevel(dspeed * 10);
    dashboard.setAdBlueLevel(dspeed * 10);
  }
delay(1500);
}
