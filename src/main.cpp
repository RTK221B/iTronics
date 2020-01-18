#include <Arduino.h>
#include <buzzer.h>
#include <led.h>
#include <accelerometer.h>
#include <battreyStetus.h>


ADC_MODE(ADC_VCC);

void setup()
{
  Serial.begin(115200);
}

rom_phy_get_vdd33()

void loop()
{
  int vcc = ESP.getVcc();
  Serial.println(vcc);
  delay(1000);
}
