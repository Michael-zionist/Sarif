String x = "1";
int y = x.toInt();

void setup() {
  Serial.begin(115200);
  Serial.print(y);
}

void loop() {
  Serial.print(y);
}