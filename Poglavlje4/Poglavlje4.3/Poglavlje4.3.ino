void setup() {                
  Serial.begin(9600);
  Serial.println("Enter a number larger than or equal to 100.");
}

void loop() {
  if (Serial.available() > 0) {
    String line = Serial.readString();
    line.trim();
    int number = line.toInt();
    Serial.println("Input: " + String(number));
    if (number < 100)
      Serial.println("The number you input is smaller than 100. Try again.");
    else
      Serial.println("Congratulations!");
  }
}
