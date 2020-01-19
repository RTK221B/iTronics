#include <server.h>


#define BattreyVoltagePin  32
#define ElectricSensorPin 2
#define RainSensorPin 33
int pinDHT11 = 25;

int BattreyADC;
float BattreyVoltage;
int RainStatus;
int Rain = 0;
int ElectricStatus;
int temp, humi;

#include <SimpleDHT.h>

SimpleDHT11 dht11(pinDHT11);

#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

long timer = 0;

int AxisData = 0;

int appSwich;

float x1, x2, x3;
float xn1, xn2, xn3;




#include <Adafruit_NeoPixel.h>

#define PIN 26
int Pixels = 8;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(Pixels, PIN, NEO_GRB + NEO_KHZ800);




/*IoT M2M Server Details */

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>

// WiFi network info.
char ssid[] = "speechbee";
char wifiPassword[] = "speechbee";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "4f9e03c0-3a28-11ea-b73d-1be39589c6b2";
char password[] = "532fd65309de6b9d89deffb16ba08d774d4f25d0";
char clientID[] = "5917b450-3a28-11ea-b73d-1be39589c6b2";


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {

  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);

  pinMode(BattreyVoltagePin, INPUT_PULLDOWN);

  pinMode(RainSensorPin, INPUT);

  pinMode(ElectricSensorPin, INPUT_PULLUP);

  pinMode(12, OUTPUT);

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  strip.begin();
  strip.show();  //Initilize strip to all off

}


void loop() {
  Cayenne.loop();

  digitalWrite(5, HIGH);
  delay(50);
  redFlash();  //call redFlash funtion
  digitalWrite(5, LOW);
  delay(50);
  red2Flash();  //Call blueFlash function


  ////////////////////////////////////////////////////--Battrey--//////////////////////////////////////////////////////

  // read the input on analog pin 0:
  BattreyADC = analogRead(BattreyVoltagePin);
  // Convert the analog reading (which goes from 0 - 4096) to a voltage (0 - 3.3V):
  BattreyVoltage = BattreyADC * (3.3 / 4096.0);
  // print out the value you read:
  Serial.println(BattreyVoltage);

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



  ////////////////////////////////////////////////////--Rain--//////////////////////////////////////////////////////

  Rain = analogRead(RainSensorPin);
  //Serial.println(Rain);
  if (Rain > 1000 ) {
    RainStatus = 1;
    Serial.println("1");
  }
  else {
    RainStatus = 0;
    Serial.println("0");
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



  ///////////////////////////////////////////////--ElectricStatus--//////////////////////////////////////////////////////

  ElectricStatus = digitalRead(ElectricSensorPin);
  //Serial.println(ElectricStatus);



  /////////////////////////////////////////////////////////--DTH--/////////////////////////////////////////////////////////

  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    return;
  }

  temp = (int)temperature;
  Serial.println(temp);

  humi = (int)humidity;
  Serial.println(humi);

  //Serial.println(appSwich);


  /////////////////////////////////////////////////////////--AxisSensor--///////////////////////////////////////////////////

  mpu6050.update();

  if (millis() - timer > 1000) {

    x1 = mpu6050.getAngleX();
    x2 = mpu6050.getAngleY();
    x3 = mpu6050.getAngleZ();

    //    Serial.println("angleX : ");Serial.print(x1);
    //  Serial.println("angleY : ");Serial.print(x2);
    // Serial.println("angleZ : ");Serial.println(x3);

    if (x1 > xn1 + 20 || x1 < xn1 - 20 || x2 > xn2 + 20 || x2 < xn2 - 20) {
      Serial.println("misallignment");
      AxisData = 1;
    }
    else {
      AxisData = 1;
    }

    xn1 = x1;
    xn2 = x2;
    xn3 = x3;

    timer = millis();
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}



/*BattreyVoltage to Server*/
CAYENNE_OUT(V2) {
  Cayenne.virtualWrite(V2, BattreyVoltage);
}

/*RainStatus to Server*/
CAYENNE_OUT(V3) {
  Cayenne.virtualWrite(V3, RainStatus);
}

/*ElectricStatus to Server*/
CAYENNE_OUT(V4) {
  Cayenne.virtualWrite(V4, ElectricStatus);
}

/*temperature to Server*/
CAYENNE_OUT(V5) {
  Cayenne.virtualWrite(V5, temp);
}

/*humidity to Server*/
CAYENNE_OUT(V6) {
  Cayenne.virtualWrite(V6, humi);
}

/*AxisData to Server*/
CAYENNE_OUT(V7) {
  Cayenne.virtualWrite(V7, AxisData);
}

/*GET data from server*/
CAYENNE_IN(V8) {
  appSwich = getValue.asInt();
}




void redFlash()
{
  for (int x = 0; x <= 3; x++) {
    red(strip.Color(255, 0, 0), 13);  //call Red funtion - pass it color and dleay values
    red(strip.Color(0, 0, 0), 0);  //Turn off all pixels
    delay(75);
  }
}  //end redFlash

void red2Flash()
{
  for (int x = 0; x <= 3; x++) {
    red2(strip.Color(225, 0, 0 ), 13); //call blue funtion - pass it color and dleay values
    red2(strip.Color(0, 0, 0), 0);  //Turn off all pixels
    delay(75);
  }
} //end blueFlash

void red(uint32_t c, uint8_t wait)  //passed color (c) and delay (wait) values
{
  int i = 0;  //start at pixel 0
  while (i < Pixels / 2) //loop until you reach half the pixels
  {
    strip.setPixelColor(i, c);  //sets the pixel and color
    i++;
  }
  strip.show();  //turns on the pixels
  delay(wait);  //wait designated time (wait)
}  //end red

void red2(uint32_t c, uint8_t wait)  //passed color (c) and delay (wait) values
{
  int i = Pixels + 1; //start at the last pixel
  while (i > Pixels / 2 - 1) //loop until you reach half the pixels
  {
    strip.setPixelColor(i, c);  //sets pixel and color
    i--;
  }
  strip.show();  //turns on pixels
  delay(wait);  //wait designated time (wait)
}  //end blue