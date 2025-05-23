#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <EEPROM.h>  // <- EEPROM incluída

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int botaoTinto = 12;
const int botaoBranco = 11;
const int LED_VERDE = 4;
const int LED_AMARELO = 5;
const int LED_VERMELHO = 6;
const int BUZZER = 7;
const int LDR = A0;

float tempmedia = 0;
float lumimedia = 0;
float umimedia = 0;
int settime = 0;

bool buzzerTocando = false;
bool jaSoouNaZonaVermelha = false;

enum TipoVinho { NENHUM, TINTO, BRANCO };
TipoVinho tipoSelecionado = NENHUM;

int tempMin = 0, tempMax = 0, umidMin = 0, umidMax = 0;
int lumMax = 30;

byte bloco1[8] = {B00111,B00011,B00000,B00111,B00100,B01110,B01010,B01110};
byte bloco2[8] = {B11001,B11011,B00100,B11111,B00100,B01110,B01010,B01110};
byte bloco3[8] = {B00000,B00000,B00000,B11100,B00100,B01110,B01010,B01110};
byte bloco4[8] = {B00111,B00101,B00111,B00000,B00000,B00000,B00000,B00000};
byte bloco5[8] = {B00000,B11111,B00000,B01110,B01010,B01110,B00000,B00000};
byte bloco6[8] = {B11100,B10100,B11100,B00000,B00000,B00000,B00000,B00000};
byte folha[8]  = {B00000,B00010,B00101,B01010,B01100,B00100,B00010,B00000};

void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, bloco1); lcd.createChar(1, bloco2); lcd.createChar(2, bloco3);
  lcd.createChar(3, bloco4); lcd.createChar(4, bloco5); lcd.createChar(5, bloco6);
  lcd.createChar(6, folha);

  dht.begin();
  rtc.begin();
  Serial.begin(9600);

  pinMode(botaoTinto, INPUT_PULLUP);
  pinMode(botaoBranco, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  mostrarLogo();

  // Ler valores salvos da EEPROM
  float tempSalvo, umidSalvo, lumiSalvo;
  EEPROM.get(0, tempSalvo);
  EEPROM.get(sizeof(float), umidSalvo);
  EEPROM.get(2 * sizeof(float), lumiSalvo);

  Serial.println("Médias salvas na EEPROM:");
  Serial.print("Temp: "); Serial.print(tempSalvo, 1); Serial.println(" C");
  Serial.print("Umidade: "); Serial.print(umidSalvo, 1); Serial.println(" %");
  Serial.print("Luminosidade: "); Serial.print(lumiSalvo, 1); Serial.println(" %");
}
void mostrarLogo() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.write(byte(0));
  lcd.setCursor(1, 0); lcd.write(byte(1));
  lcd.setCursor(2, 0); lcd.write(byte(2));
  lcd.setCursor(0, 1); lcd.write(byte(3));
  lcd.setCursor(1, 1); lcd.write(byte(4));
  lcd.setCursor(2, 1); lcd.write(byte(5));
  delay(2000);
  lcd.clear();
}
void mostrarLogoCanto() {
  lcd.setCursor(0, 0); lcd.write(byte(0));
  lcd.setCursor(1, 0); lcd.write(byte(1));
  lcd.setCursor(2, 0); lcd.write(byte(2));
  lcd.setCursor(0, 1); lcd.write(byte(3));
  lcd.setCursor(1, 1); lcd.write(byte(4));
  lcd.setCursor(2, 1); lcd.write(byte(5));
}

void loop() {
  checarTrocaDeVinho();

  if (tipoSelecionado == NENHUM) {
    lcd.clear();
    mostrarLogoCanto();
    lcd.setCursor(4, 0);
    lcd.print("Selecione");
    lcd.setCursor(4, 1);
    lcd.print("Tinto/Branco");
    delay(300);
    return;
  }

  float temp = dht.readTemperature();
  float umid = dht.readHumidity();
  int lum = analogRead(LDR);
  int lumPercent = map(lum, 0, 1023, 0, 100);

  DateTime now = rtc.now();

  Serial.print("[");
  Serial.print(now.day()); Serial.print("/");
  Serial.print(now.month()); Serial.print("/");
  Serial.print(now.year()); Serial.print(" ");
  Serial.print(now.hour()); Serial.print(":");
  Serial.print(now.minute()); Serial.print(":");
  Serial.print(now.second()); Serial.print("] ");
  Serial.print("Tipo: ");
  Serial.print(tipoSelecionado == TINTO ? "Tinto" : "Branco");
  Serial.print(" | Temp: "); Serial.print(temp); Serial.print(" C");
  Serial.print(" | Umid: "); Serial.print(umid); Serial.print(" %");
  Serial.print(" | Lum: "); Serial.print(lumPercent); Serial.println(" %");

  bool tempOK = (temp >= tempMin && temp <= tempMax);
  bool umidOK = (umid >= umidMin && umid <= umidMax);
  bool lumOK = lumPercent <= lumMax;

  lcd.clear();
  mostrarLogoCanto();
  lcd.setCursor(4, 0);
  lcd.print(tipoSelecionado == TINTO ? "Tinto" : "Branco");

  if (tempOK && umidOK && lumOK) {
    lcd.setCursor(4, 1);
    lcd.print("Tudo OK");
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    noTone(BUZZER);
    jaSoouNaZonaVermelha = false;
  } else {
    lcd.setCursor(4, 1);
    if (!tempOK)
      lcd.print(temp < tempMin ? "Aumente Temp" : "Reduza Temp");
    else if (!umidOK)
      lcd.print(umid < umidMin ? "Aum. Umidade" : "Red. Umidade");
    else if (!lumOK)
      lcd.print("Reduza Lum.");

    digitalWrite(LED_VERDE, LOW);

    if (!tempOK && (temp < tempMin - 3 || temp > tempMax + 3)) {
      digitalWrite(LED_VERMELHO, HIGH);
      digitalWrite(LED_AMARELO, LOW);
      if (!jaSoouNaZonaVermelha) {
        tone(BUZZER, 1000);
        jaSoouNaZonaVermelha = true;
      }
    } else {
      digitalWrite(LED_AMARELO, HIGH);
      digitalWrite(LED_VERMELHO, LOW);
      noTone(BUZZER);
      jaSoouNaZonaVermelha = false;
    }
  }

  // Acumula para média
  tempmedia += temp;
  lumimedia += lumPercent;
  umimedia += umid;
  settime += 1;
  delay(1000);

  if (settime == 10) {
    tempmedia /= 10;
    lumimedia /= 10;
    umimedia /= 10;

    Serial.println("Médias (últimos 10 seg):");
    Serial.print("Temp: "); Serial.print(tempmedia, 1); Serial.println(" C");
    Serial.print("Umidade: "); Serial.print(umimedia, 1); Serial.println(" %");
    Serial.print("Luminosidade: "); Serial.print(lumimedia, 1); Serial.println(" %");

    // Salvar na EEPROM
    EEPROM.put(0, tempmedia);
    EEPROM.put(sizeof(float), umimedia);
    EEPROM.put(2 * sizeof(float), lumimedia);

    // Resetar contadores
    tempmedia = 0;
    lumimedia = 0;
    umimedia = 0;
    settime = 0;
  }
}

void checarTrocaDeVinho() {
  if (digitalRead(botaoTinto) == LOW) {
    tipoSelecionado = TINTO;
    tempMin = 12;
    tempMax = 18;
    umidMin = 60;
    umidMax = 70;
    delay(300);
  }
  if (digitalRead(botaoBranco) == LOW) {
    tipoSelecionado = BRANCO;
    tempMin = 8;
    tempMax = 12;
    umidMin = 70;
    umidMax = 80;
    delay(300);
  }
}
