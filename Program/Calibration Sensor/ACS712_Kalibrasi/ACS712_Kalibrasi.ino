
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int adc = analogRead(A1);
  float voltage = adc * 5 / 1023.0;
  float current = (voltage - 3.6) / 0.185;
  if (current < 0.16) {
    current = 0;
  }
  Serial.print("current: ");
  Serial.println(current);
  delay(300);
}
