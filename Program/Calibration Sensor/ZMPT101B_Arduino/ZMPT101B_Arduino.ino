#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2);
int adc_max1 = 920;
int adc_min1 = 609;
float volt_multi = 231;
float volt_multi_p;
float volt_multi_n;
float volt_rms1 = 0;
int Voltage_Sensor_1 = A0;
void setup() {
  Serial.begin(115200);
  pinMode(Voltage_Sensor_1, INPUT);
  volt_multi_p = volt_multi * 1.4142;
  volt_multi_n = -volt_multi_p;
  lcd.begin();
  lcd.backlight();
}

void loop() {
  volt_rms1 = get_voltage(Voltage_Sensor_1, adc_min1, adc_max1);
  String VOLT1 = String(volt_rms1);
  lcd.setCursor(0,0);
  lcd.print("Vrms: ");
  lcd.print(VOLT1);
  lcd.print(" VAC"); 
  Serial.print("Vrms: ");
  Serial.print(VOLT1);
  Serial.println(" VAC");
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
