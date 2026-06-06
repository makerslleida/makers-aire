#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <SensirionI2cSps30.h>

#ifdef NO_ERROR
#undef NO_ERROR
#endif

#define NO_ERROR 0

// ======================================================================
// BME280 SPI
// ======================================================================

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

// ======================================================================
// SPS30 I2C
// ======================================================================

SensirionI2cSps30 sps30;

static char errorMessage[64];
static int16_t error;

// ======================================================================
// SETUP
// ======================================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("=================================");
  Serial.println("MAKERS AIRE V1");
  Serial.println("=================================");

  // =========================================================
  // BME280 SPI
  // =========================================================

  Serial.println();
  Serial.println("Iniciando BME280 SPI...");

  if (!bme.begin()) {

    Serial.println("ERROR: No se detecta BME280");

    while (1);
  }

  Serial.println("BME280 OK");

  // =========================================================
  // SPS30 I2C
  // =========================================================

  Serial.println();
  Serial.println("Iniciando SPS30 I2C...");

  Wire.begin(21, 22);

  sps30.begin(Wire, SPS30_I2C_ADDR_69);

  sps30.stopMeasurement();

  delay(500);

  int8_t productType[8] = {0};

  error = sps30.readProductType(productType, 8);

  if (error != NO_ERROR) {

    Serial.print("ERROR SPS30: ");

    errorToString(error, errorMessage, sizeof(errorMessage));

    Serial.println(errorMessage);

    while (1);
  }

  Serial.print("Producto detectado: ");
  Serial.println((const char*)productType);

  error = sps30.startMeasurement(
    SPS30_OUTPUT_FORMAT_OUTPUT_FORMAT_UINT16
  );

  if (error != NO_ERROR) {

    Serial.print("ERROR startMeasurement(): ");

    errorToString(error, errorMessage, sizeof(errorMessage));

    Serial.println(errorMessage);

    while (1);
  }

  Serial.println("SPS30 OK");

  Serial.println();
  Serial.println("Sistema iniciado correctamente");
}

// ======================================================================
// LOOP
// ======================================================================

void loop() {

  // =========================================================
  // VARIABLES SPS30
  // =========================================================

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

  // =========================================================
  // LEER SPS30
  // =========================================================

  error = sps30.readDataReadyFlag(dataReadyFlag);

  if (error != NO_ERROR) {

    Serial.print("ERROR SPS30: ");

    errorToString(error, errorMessage, sizeof(errorMessage));

    Serial.println(errorMessage);

    delay(2000);

    return;
  }

  if (dataReadyFlag) {

    error = sps30.readMeasurementValuesUint16(
      mc1p0,
      mc2p5,
      mc4p0,
      mc10p0,
      nc0p5,
      nc1p0,
      nc2p5,
      nc4p0,
      nc10p0,
      typicalParticleSize
    );

    if (error != NO_ERROR) {

      Serial.print("ERROR lectura SPS30: ");

      errorToString(error, errorMessage, sizeof(errorMessage));

      Serial.println(errorMessage);

      delay(2000);

      return;
    }
  }

  // =========================================================
  // MOSTRAR DATOS
  // =========================================================

  Serial.println();
  Serial.println("=================================");
  Serial.println("DATOS MAKERS AIRE");
  Serial.println("=================================");

  // -----------------------------
  // BME280
  // -----------------------------

  Serial.println();
  Serial.println("[BME280]");

  Serial.print("Temperatura: ");
  Serial.print(bme.readTemperature());
  Serial.println(" ºC");

  Serial.print("Humedad: ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.print("Presion: ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  // -----------------------------
  // SPS30
  // -----------------------------

  Serial.println();
  Serial.println("[SPS30]");

  Serial.print("PM1.0 : ");
  Serial.println(mc1p0);

  Serial.print("PM2.5 : ");
  Serial.println(mc2p5);

  Serial.print("PM4.0 : ");
  Serial.println(mc4p0);

  Serial.print("PM10  : ");
  Serial.println(mc10p0);

  delay(5000);
}
