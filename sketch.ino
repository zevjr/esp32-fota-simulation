#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* firmwareURL =
  "https://raw.githubusercontent.com/zevjr/esp32-fota-simulation/main/firmware_v2.bin";


#define LED_PIN 2
#define CURRENT_VERSION 1.0

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("\n =====!================");
  Serial.printf("FIRM ATUAL: versao %.1f\n", CURRENT_VERSION);
  Serial.println("==========================");

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println("\n WiFi conectado");

  checkAndApplyOTA();
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
  Serial.println("[v1.0] Executando loop...");
}

void checkAndApplyOTA() {
  Serial.println("\n[FOTA] - verificando");

  HTTPClient http;
  http.begin(firmwareURL);

  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.println("Erro de conexão");
    http.end();
    return;
  }

  int contentLength = http.getSize();

  Serial.printf(
    "[FOTA] - Novo firmware encontrado, tamanho %d bytes\n",
    contentLength
  );

  bool canBegin = Update.begin(contentLength);

  if (canBegin) {
    Serial.println("[FOTA] - Gravando na memoria");

    WiFiClient* client = http.getStreamPtr();
    size_t written = Update.writeStream(*client);

    if (written == contentLength) {
      Serial.println("[FOTA] - deu boa");
    } else {
      Serial.printf(
        "[FOTA] - deu ruim %d/%d\n",
        written,
        contentLength
      );
    }

    if (Update.end()) {
      if (Update.isFinished()) {
        Serial.println("[FOTA] - concluido!");
        delay(1000);
        ESP.restart();
      }
    } else {
      Serial.printf(
        "[FOTA] Erro: %s\n",
        Update.errorString()
      );
    }
  } else {
    Serial.println("[FOTA] - nao foi possivel iniciar");
  }

  http.end();
}
