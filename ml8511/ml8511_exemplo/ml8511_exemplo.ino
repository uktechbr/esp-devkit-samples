#include "Adafruit_SSD1306.h"
#include <ML8511.h>

// Define o tamanho do display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define ANALOGPIN 0
ML8511 ml(ANALOGPIN);
// Inidicaliza o sensor e o display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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
  //Parametros da tensão de operação do sensor e do adc
  ml.setVoltsPerStep(3.3, 4096);   

  // Limpe o buffer do display
  display.clearDisplay();

  // Configuração do texto
  display.setTextSize(1.5);             // Tamanho do texto (1 = 6x8 pixels)
  display.setTextColor(SSD1306_WHITE);  // Cor do texto
}

void loop() {
  // Tempo entre cada medida
  delay(2000);


  Serial.print("UV: ");
  Serial.println( ml.getUV(), 4);

  // Formata o dados para exibir no display
  char uvString[25];
  sprintf(uvString, "Ultravioleta: %.5f", ml.getUV());
 
  // Limpa o buffer do display
  display.clearDisplay();
  // Posição inicial do cursor no display
  display.setCursor(0, 0);

  //Envia novos dados ao buffer do display
  display.println(uvString);

  // Atualiza o display para mostrar os dados no buffer
  display.display();
}