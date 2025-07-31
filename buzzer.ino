//Codigo criado por André Gaito, Sensor CO2 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME280.h"
#include <MQ135.h>
#include "BluetoothSerial.h"
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

#define PIN_MQ135 14

MQ135 mq135_sensor(PIN_MQ135);
Adafruit_BME280 bme; // I2C 
LiquidCrystal_I2C lcd1(0x27, 16, 2); // LCD 16x2


void setup() {
  Serial.begin(115200);
  Wire.begin();
  lcd1.begin(16,2,0x27);
  lcd1.clear();
  while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("BME280 test"));
  lcd1.print("Gaito Labs");
  delay(1500);
  lcd1.clear();

unsigned status;

  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bme.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring or try a different address!"));
    while (1)
    delay(10);
  }
}

void loop() {
// Leitura do MQ135
float u = bme.readHumidity(); // Le a Umidade em %.
float t = bme.readTemperature(); // Le a temperatura em °C
float rzero = mq135_sensor.getRZero();
float correctedRZero = mq135_sensor.getCorrectedRZero(t, u);
float resistance = mq135_sensor.getResistance();
float PPM = mq135_sensor.getPPM();
float correctedPPM = mq135_sensor.getCorrectedPPM(t, u);


//Exibir no Monitor Serial:
    Serial.print(F("Temperatura: "));
    Serial.print(bme.readTemperature());
    Serial.println("°C");

    Serial.print(F("Pressão: "));
    Serial.print(bme.readPressure() / 1000);
    Serial.println(" KPa");

    Serial.print(F("Altitude Aproximada: "));
    Serial.print(bme.readAltitude(1013.25)); /* Adjusted to local forecast! */
    Serial.println("m");

    Serial.print("Umidade: ");
    Serial.print(bme.readHumidity());
    Serial.println("%");

    Serial.print("CO2: ");
    Serial.print(correctedPPM);
    Serial.println("PPM");
    Serial.print(" ");
    Serial.println("");
    
  delay(1500);

//Exibir no Display 16x2
  lcd1.clear();
  lcd1.print(F("CO2: "));
  lcd1.setCursor(6,0);
  lcd1.print(correctedPPM);
  lcd1.print(" PPM");

  lcd1.setCursor(0,1);
  lcd1.print(F("Tp:"));
  lcd1.print(bme.readTemperature());
  lcd1.print("°C");

  lcd1.setCursor(6,1);
  lcd1.print("Um:");
  lcd1.print(bme.readHumidity());
  lcd1.println("%");
  lcd1.println();
  delay(1000);
}