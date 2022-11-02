int btn = 2;

void setup() {
  Serial.begin(9600);
  pinMode(btn, INPUT_PULLUP);
}

void loop() {
  digitalWrite(btn, HIGH);
  if (digitalRead(btn) == LOW) {
    Serial.println("Bruh");
  }

}
