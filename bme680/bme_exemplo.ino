#include "Adafruit_SSD1306.h"
#include "Adafruit_BME680.h"

// Define o tamanho do display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define SEALEVELPRESSURE_HPA (1013.25)

// Inidicaliza o sensor e o display
Adafruit_BME680 bme;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  //Velocidade do monitor serial
  Serial.begin(115200);
  // Pinos do barramento I2C
  Wire.begin(3, 2);

   // Verifica estado de inicialização do display, acusa falha em caso de erro
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0, false, true)) {
    Serial.println(F("Falha ao inicializar o SSD1306"));
    for (;;)
      ;
  }

  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }

  // Ajusta os filtros da biblioteca do sensor
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  // Limpe o buffer do display
  display.clearDisplay();

  // Configuração do texto
  display.setTextSize(1.5);             // Tamanho do texto (1 = 6x8 pixels)
  display.setTextColor(SSD1306_WHITE);  // Cor do texto
}

void loop() {
  // Tempo entre cada medida
  delay(2000);


  // BME680 inicia o processo de amostragem
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Falha ao obter medicao"));
    return;
  }
  Serial.print(F("Medicao comecou em: "));
  Serial.print(millis());
  Serial.print(F(" e terminou em "));
  Serial.println(endTime);
  
  if (!bme.endReading()) {
    Serial.println(F("Falha com completar medicao"));
    return;
  }

  Serial.print(F("Temperatura = "));
  Serial.print(bme.temperature);
  Serial.println(F(" *C"));

  Serial.print(F("Pressão = "));
  Serial.print(bme.pressure / 100.0);
  Serial.println(F(" hPa"));

  Serial.print(F("Umidade = "));
  Serial.print(bme.humidity);
  Serial.println(F(" %"));

  Serial.print(F("Gas = "));
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(F(" KOhms"));

  Serial.print(F("Altitude aproximada = "));
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(F(" m"));

  Serial.println();

  // Formata o dados para exibir no display
  char tempString[25];
  sprintf(tempString, "Temperatura: %.2f%cC", bme.temperature, (char)247);
  char humString[20];
  sprintf(humString, "Umidade: %.2f%%", bme.humidity);
  char gasString[20];
  sprintf(gasString, "VOC: %.2lf%KOhms", bme.gas_resistance / 1000.0);
  char alt[20];
  sprintf(alt, "Altitude: %.2f% m", bme.readAltitude(SEALEVELPRESSURE_HPA));

  // Limpa o buffer do display
  display.clearDisplay();
  // Posição inicial do cursor no display
  display.setCursor(0, 0);

  //Envia novos dados ao buffer do display
  display.println(tempString);
  display.println(humString);
  display.println(gasString);
  display.println(alt);


  // Atualiza o display para mostrar os dados no buffer
  display.display();
}
