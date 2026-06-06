#include <Arduino.h>
#include <Wire.h>
#include <SensirionI2cSps30.h>

#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

SensirionI2cSps30 sensor;

static char errorMessage[64];
static int16_t error;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("Iniciando SPS30 por I2C...");

  // ESP32: SDA=GPIO21, SCL=GPIO22
  Wire.begin(21, 22);

  sensor.begin(Wire, SPS30_I2C_ADDR_69);

  sensor.stopMeasurement();
  delay(500);

  int8_t productType[8] = {0};

  error = sensor.readProductType(productType, 8);
  if (error != NO_ERROR) {
    Serial.print("ERROR readProductType(): ");
    errorToString(error, errorMessage, sizeof(errorMessage));
    Serial.println(errorMessage);
    while (1);
  }

  Serial.print("Producto: ");
  Serial.println((const char*)productType);

  error = sensor.startMeasurement(SPS30_OUTPUT_FORMAT_OUTPUT_FORMAT_UINT16);
  if (error != NO_ERROR) {
    Serial.print("ERROR startMeasurement(): ");
    errorToString(error, errorMessage, sizeof(errorMessage));
    Serial.println(errorMessage);
    while (1);
  }

  Serial.println("SPS30 detectado. Midiendo...");
}

void loop() {
  uint16_t dataReadyFlag = 0;

  uint16_t mc1p0 = 0;
  uint16_t mc2p5 = 0;
  uint16_t mc4p0 = 0;
  uint16_t mc10p0 = 0;

  uint16_t nc0p5 = 0;
  uint16_t nc1p0 = 0;
  uint16_t nc2p5 = 0;
  uint16_t nc4p0 = 0;
  uint16_t nc10p0 = 0;

  uint16_t typicalParticleSize = 0;

  delay(1000);

  error = sensor.readDataReadyFlag(dataReadyFlag);
  if (error != NO_ERROR) {
    Serial.print("ERROR readDataReadyFlag(): ");
    errorToString(error, errorMessage, sizeof(errorMessage));
    Serial.println(errorMessage);
    return;
  }

  if (!dataReadyFlag) {
    Serial.println("Datos no listos...");
    return;
  }

  error = sensor.readMeasurementValuesUint16(
    mc1p0, mc2p5, mc4p0, mc10p0,
    nc0p5, nc1p0, nc2p5, nc4p0, nc10p0,
    typicalParticleSize
  );

  if (error != NO_ERROR) {
    Serial.print("ERROR readMeasurementValuesUint16(): ");
    errorToString(error, errorMessage, sizeof(errorMessage));
    Serial.println(errorMessage);
    return;
  }

  Serial.println("------------------");
  Serial.print("PM1.0 : "); Serial.println(mc1p0);
  Serial.print("PM2.5 : "); Serial.println(mc2p5);
  Serial.print("PM4.0 : "); Serial.println(mc4p0);
  Serial.print("PM10  : "); Serial.println(mc10p0);
}
