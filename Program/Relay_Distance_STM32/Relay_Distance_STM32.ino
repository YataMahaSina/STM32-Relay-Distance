#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2);
int adc_max1 = 920;
int adc_min1 = 609;
float volt_multi = 220;
float volt_multi_p;
float volt_multi_n;
float volt_rms1 = 0;
int Voltage_Sensor_1 = A0;
int Current_Sensor = A1;
int relay = 7;
#define I_SET 1.20
float impedansi;

void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  pinMode(Voltage_Sensor_1, INPUT);
  volt_multi_p = volt_multi * 1.4142;
  volt_multi_n = -volt_multi_p;
  lcd.begin();
  lcd.backlight();
  digitalWrite(relay, LOW);
}

void loop() {
  //Voltage Sensor
  volt_rms1 = get_voltage(Voltage_Sensor_1, adc_min1, adc_max1);
  String VOLT1 = String(volt_rms1);
  //Current Sensor
  int adc = analogRead(Current_Sensor);
  float voltage = adc * 5 / 1023.0;
  float current = (voltage - 3.6) / 0.185;
  impedansi = volt_rms1 / current;
  float Z_set = 0.7 * impedansi;
  if (current < 0.16) {
    current = 0;
  }
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(VOLT1);
  lcd.print(",");
  lcd.print("I:");
  lcd.print(current, 2);
  lcd.print(" A");
  lcd.setCursor(0, 1);
  lcd.print("Z:");
  lcd.print(impedansi);
  lcd.setCursor(7, 1);
  lcd.print(" ohm");
  Serial.print("Vrms: ");
  Serial.print(VOLT1);
  Serial.println(" VAC");
  Serial.print("current: ");
  Serial.println(current, 2);

  if (current > I_SET) {
    impedansi = volt_rms1 / current;
    digitalWrite(relay, HIGH); delay(7000);
    digitalWrite(relay, LOW);
  }
  else
  {
    digitalWrite(relay, LOW);
  }

  //  if (current > I_SET){
  //    float impedansi = VOLT1/current;
  //    if (impedansi <= Z_set){
  //      digitalWrite(relay,HIGH);
  //    }
  //    else
  //    {
  //      digitalWrite(relay,LOW);
  //    }
  //  }
  //  else
  //  {
  //    digitalWrite(relay,LOW);
  //  }
}

float get_voltage(int aa, int bb, int cc) {
  float adc_sample;
  float volt_inst = 0;
  float sum = 0;
  float volt;
  long init_time = millis();
  int N = 0;

  while ((millis() - init_time) < 100) {
    adc_sample = analogRead(aa);
    volt_inst = map(adc_sample, bb, cc, volt_multi_n, volt_multi_p);
    sum += sq(volt_inst);
    N++;
    delay(1);
  }
  volt = sqrt(sum / N);
  return volt;
}
