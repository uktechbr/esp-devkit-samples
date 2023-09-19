#include "DHT.h"
#include "Adafruit_SSD1306.h"

// Define o tamanho do display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define DHTPIN 1  //Pino onde esta conectado o sensor DHT

// Define o modelo de sensor DHT que estamos usando
#define DHTTYPE DHT11  // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Inidicaliza o sensor e o display
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  //Velocidade do monitor serial
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  // Pinos do barramento I2C
  Wire.begin(3, 2);

  // Verifica estado de inicialização do display, acusa falha em caso de erro
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0, false, true)) {
    Serial.println(F("Falha ao inicializar o SSD1306"));
    for (;;)
      ;
  }

  // Limpe o buffer do display
  display.clearDisplay();

  // Configuração do texto
  display.setTextSize(1.5);             // Tamanho do texto (1 = 6x8 pixels)
  display.setTextColor(SSD1306_WHITE);  // Cor do texto
}

void loop() {
  // Tempo entre cada medida
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // Verifica se as leituras são validas
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Falha ao ler do sensor"));
    return;
  }

  // Calcula o índice de calor em Fahrenheit
  float hif = dht.computeHeatIndex(f, h);
  // Calcula o índice de calor em Celsius
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Umidade: "));
  Serial.print(h);
  Serial.print(F("%  Temperatura: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Indice de calor: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  // Formata o dados para exibir no display
  char tempString[25];
  sprintf(tempString, "Temperatura: %.2f%cC", t, (char)247);
  char humString[20];
  sprintf(humString, "Umidade: %.2f%%", h);

  // Limpa o buffer do display
  display.clearDisplay();
  // Posição inicial do cursor no display
  display.setCursor(0, 0);

  //Envia novos dados ao buffer do display
  display.println(tempString);
  display.println(humString);

  // Atualiza o display para mostrar os dados no buffer
  display.display();
}
