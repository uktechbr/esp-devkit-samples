#include "Adafruit_SSD1306.h"
#include "Adafruit_TSL2591.h"

// Define o tamanho do display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32


// Inidicaliza o sensor e o display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)


void setup() {
  //Velocidade do monitor serial
  Serial.begin(115200);
  // Pinos do barramento I2C
  Wire.begin(3, 2);

   // Verifica estado de inicialização do display, acusa falha em caso de erro
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0, false, true)) {
    Serial.println(F("Falha ao inicializar o SSD1306"));
    while (1);
  }
   // Verifica estado de inicialização do sensor de uminosidade, acusa falha em caso de erro
   if (!tsl.begin()) 
  {
    Serial.println(F("Falha ao inicializar o tsl2591"));
    while (1);
  }

  //Configura o ganho do sensor
  tsl.setGain(TSL2591_GAIN_LOW);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);

  // Limpe o buffer do display
  display.clearDisplay();

  // Configuração do texto
  display.setTextSize(1.5);             // Tamanho do texto (1 = 6x8 pixels)
  display.setTextColor(SSD1306_WHITE);  // Cor do texto
}

void loop() {
  // Tempo entre cada medida
  delay(2000);


  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;

  ir = lum >> 16; // Calcula o infravermelho
  full = lum & 0xFFFF; // Calcula a luminosidade total

  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print(F("Infravermelho: ")); Serial.print(ir);  Serial.print(F("  "));
  Serial.print(F("Total: ")); Serial.print(full); Serial.print(F("  "));
  Serial.print(F("Visivel: ")); Serial.print(full - ir); Serial.print(F("  "));
  Serial.print(F("Lux: ")); Serial.println(tsl.calculateLux(full, ir), 6);

  // Formata o dados para exibir no display
  char irString[25];
  sprintf(irString, "Infravermelho: %i", ir);
  char visString[25];
  sprintf(visString, "Visivel: %i", full -ir);
   char luxString[25];
  sprintf(luxString, "Lux: %.2f", tsl.calculateLux(full, ir));

  // Limpa o buffer do display
  display.clearDisplay();
  // Posição inicial do cursor no display
  display.setCursor(0, 0);

  //Envia novos dados ao buffer do display
  display.println(irString);
  display.println(visString);
  display.println(luxString);

  // Atualiza o display para mostrar os dados no buffer
  display.display();
}