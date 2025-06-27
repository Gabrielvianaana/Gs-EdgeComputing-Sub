#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <EEPROM.h>

// LCD I2C e RTC
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

// Pinos
const int sensorSolo = A0;
const int buzzer = 9;
const int ledVerde = 6;
const int ledAmarelo = 7;
const int ledVermelho = 8;

// Variáveis
int menorUmidade = 100;
float umidadeSuavizada = -1;
const float alpha = 0.1;  // Suavização exponencial
int umidadeAnterior = -1; // Para histerese
const int tolerancia = 3; // Histerese para estabilidade

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(sensorSolo, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  if (!rtc.begin()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RTC NAO ENCONTRADO");
    while (1);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Bem-vindo ao");
  lcd.setCursor(0, 1);
  lcd.print("   FloodSense!");
  delay(3000);

  menorUmidade = EEPROM.read(0);
  if (menorUmidade < 0 || menorUmidade > 100) {
    menorUmidade = 100;
  }
}

void loop() {
  // Leitura com média de 50 amostras
  long soma = 0;
  for (int i = 0; i < 50; i++) {
    soma += analogRead(sensorSolo);
    delay(2);
  }
  int leitura = soma / 50;

  // Ajuste do mapeamento do sensor (ajustável conforme necessidade real)
  int umidade = map(leitura, 800, 500, 0, 100);
  umidade = constrain(umidade, 0, 100);

  // Suavização exponencial
  if (umidadeSuavizada < 0) {
    umidadeSuavizada = umidade;
  } else {
    umidadeSuavizada = alpha * umidade + (1 - alpha) * umidadeSuavizada;
  }

  int umidadeFiltrada = round(umidadeSuavizada);
  DateTime agora = rtc.now();

  // Atualiza EEPROM
  if (umidadeFiltrada < menorUmidade) {
    menorUmidade = umidadeFiltrada;
    EEPROM.write(0, menorUmidade);
  }

  // Só atualiza display e LEDs se houver variação significativa
  if (abs(umidadeFiltrada - umidadeAnterior) >= tolerancia || umidadeAnterior == -1) {
    umidadeAnterior = umidadeFiltrada;

    // LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Solo: ");
    lcd.print(umidadeFiltrada);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("Hora: ");
    if (agora.hour() < 10) lcd.print("0");
    lcd.print(agora.hour());
    lcd.print(":");
    if (agora.minute() < 10) lcd.print("0");
    lcd.print(agora.minute());
    lcd.print(":");
    if (agora.second() < 10) lcd.print("0");
    lcd.print(agora.second());

    // Serial
    Serial.print("Umidade: ");
    Serial.print(umidadeFiltrada);
    Serial.print("% | Menor: ");
    Serial.print(menorUmidade);
    Serial.print("% | Hora: ");
    Serial.print(agora.hour());
    Serial.print(":");
    Serial.print(agora.minute());
    Serial.print(":");
    Serial.println(agora.second());

    // Reset LEDs e buzzer
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    noTone(buzzer);

    // Alerta por faixa ideal (ideal: 40–69%)
    if (umidadeFiltrada >= 70) {
      digitalWrite(ledVerde, HIGH);  // Encharcado
      tone(buzzer, 1000);            // ALERTA
    } else if (umidadeFiltrada >= 40) {
      digitalWrite(ledAmarelo, HIGH); // Ideal
    } else {
      digitalWrite(ledVermelho, HIGH); // Seco
      tone(buzzer, 1000);              // ALERTA
    }
  }

  delay(500);
}
