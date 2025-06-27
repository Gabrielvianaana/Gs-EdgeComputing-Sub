#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <EEPROM.h>

// LCD I2C e RTC
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

// Pinos
const int potenciometro = A0; // Slide pot
const int buzzer = 9;
const int ledVerde = 6;
const int ledAmarelo = 7;
const int ledVermelho = 8;

// Variáveis
int menorUmidade = 100;
int maiorUmidade = 0;
float umidadeSuavizada = -1;
const float alpha = 0.1;
int umidadeAnterior = -1;
const int tolerancia = 3;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(potenciometro, INPUT);
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
  maiorUmidade = EEPROM.read(1);

  if (menorUmidade < 0 || menorUmidade > 100) menorUmidade = 100;
  if (maiorUmidade < 0 || maiorUmidade > 100) maiorUmidade = 0;
}

void loop() {
  long soma = 0;
  for (int i = 0; i < 50; i++) {
    soma += analogRead(potenciometro);
    delay(2);
  }
  int leitura = soma / 50;
  int umidade = map(leitura, 1023, 0, 0, 100);
  umidade = constrain(umidade, 0, 100);

  if (umidadeSuavizada < 0) {
    umidadeSuavizada = umidade;
  } else {
    umidadeSuavizada = alpha * umidade + (1 - alpha) * umidadeSuavizada;
  }

  int umidadeFiltrada = round(umidadeSuavizada);
  DateTime agora = rtc.now();

  // Atualiza menor e maior umidade
  if (umidadeFiltrada < menorUmidade) {
    menorUmidade = umidadeFiltrada;
    EEPROM.write(0, menorUmidade);
  }
  if (umidadeFiltrada > maiorUmidade) {
    maiorUmidade = umidadeFiltrada;
    EEPROM.write(1, maiorUmidade);
  }

  if (abs(umidadeFiltrada - umidadeAnterior) >= tolerancia || umidadeAnterior == -1) {
    umidadeAnterior = umidadeFiltrada;

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

    // Serial Monitor
    Serial.print("Umidade: ");
    Serial.print(umidadeFiltrada);
    Serial.print("% | Menor: ");
    Serial.print(menorUmidade);
    Serial.print("% | Maior: ");
    Serial.print(maiorUmidade);
    Serial.print("% | Hora: ");
    Serial.print(agora.hour());
    Serial.print(":");
    Serial.print(agora.minute());
    Serial.print(":");
    Serial.println(agora.second());

    // Alerta visual e sonoro
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    noTone(buzzer);

    if (umidadeFiltrada >= 70) {
      digitalWrite(ledVerde, HIGH);
      tone(buzzer, 1000);
      delay(3000);
      noTone(buzzer);
    } else if (umidadeFiltrada >= 40) {
      digitalWrite(ledAmarelo, HIGH);
    } else {
      digitalWrite(ledVermelho, HIGH);
      tone(buzzer, 1000);
      delay(3000);
      noTone(buzzer);
    }
  }

  delay(500);
}
