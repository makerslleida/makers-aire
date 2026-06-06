#include <Wire.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Escaner I2C");

  Wire.begin(21, 22);
  Wire.setClock(100000);
}

void loop() {
  byte error;
  int encontrados = 0;

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Encontrado I2C en 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      encontrados++;
    }
  }

  if (encontrados == 0) {
    Serial.println("No se ha encontrado ningun dispositivo I2C");
  }

  Serial.println("----");
  delay(3000);
}
