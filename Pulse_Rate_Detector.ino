#include <LiquidCrystal.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin = 2, TXPin = 3;
static const uint32_t gps_baudrate = 9600;
TinyGPSPlus gps;
SoftwareSerial soft(RXPin, TXPin);
float Lat, Lon;
int const PULSE_SENSOR_PIN = A0;
int buzzpin = 4;
int Signal;                // Store incoming ADC data. Value can range from 0-1024
int Threshold = 550;       // Determine which Signal to "count as a beat" and which to ignore.
const int rs = 10, en = 11, d4 = 5, d5 = 7, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int key;

void setup()
{
  soft.begin(gps_baudrate);
  Serial.begin(19200);
  pinMode(A0, INPUT);
  pinMode(12, INPUT);
  pinMode(buzzpin, OUTPUT);
  lcd.begin(16, 2);
}
void sendsms()
{
  Serial.print("AT+CMGF=1\r");
  delay(100);
  Serial.println("AT+CMGS =\"+91XXXXXXXXXX\"");
  delay(100);
  Serial.println("I want Help !!!Location:" + String("Lat: ") + String(Lat) + " " + String("Lon: ") + String(Lon));
  delay(100);
  Serial.println((char)26);
  delay(100);
  Serial.println();
  delay(5000);
}
void loop()
{
  Signal = analogRead(PULSE_SENSOR_PIN);
  if (Signal > Threshold)
  { lcd.setCursor(0, 0);
    // print the number of seconds since reset:
    lcd.print(Signal);
    lcd.setCursor(0, 1);
    lcd.print("Abnormal");
    digitalWrite(buzzpin, HIGH);
    while (soft.available() > 0)
    {
      gps.encode(soft.read());
      if (gps.location.isUpdated())
      {
        Lat = gps.location.lat();
        Lon = gps.location.lng();
      }
    }
    key = digitalRead(12);
    if (key == 1)
    {
      sendsms();
    }
  }
  else {
    lcd.setCursor(0, 0);
    // print the number of seconds since reset:
    lcd.print(Signal);
    lcd.setCursor(0, 1);
    lcd.print("normal");
  }
  }
