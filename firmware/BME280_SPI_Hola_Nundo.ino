#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_CS   5
#define BME_MOSI 23
#define BME_MISO 19
#define BME_SCK  18

Adafruit_BME280 bme(
  BME_CS,
  BME_MOSI,
  BME_MISO,
  BME_SCK
);

void setup() {

  Serial.begin(115200);

  Serial.println("Iniciando BME280 SPI...");

  if (!bme.begin()) {

    Serial.println("No se detecta BME280 por SPI");
    while (1);
  }

  Serial.println("BME280 SPI detectado");
}

void loop() {

  Serial.println("---------------");

  Serial.print("Temperatura: ");
  Serial.print(bme.readTemperature());
  Serial.println(" ºC");

  Serial.print("Humedad: ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.print("Presion: ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  delay(2000);
}
