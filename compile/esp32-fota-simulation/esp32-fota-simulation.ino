#define LED_PIN 2
#define CURRENT_VERSION 2.0

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  Serial.printf("[v%.1f] Firmware atualizado!\n", CURRENT_VERSION);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(300);

  digitalWrite(LED_PIN, LOW);
  delay(300);

  Serial.println("[v2.0] Executando loop...");
}
