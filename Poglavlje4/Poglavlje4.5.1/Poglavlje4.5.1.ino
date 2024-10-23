int numbers[5];
int i = 0, n = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter five numbers (in separate lines):");
}

void loop() {
  if (Serial.available() > 0 && i < 6) {
    String line = Serial.readString();
    line.trim();
    int number = line.toInt();
    if (i < 5)
      numbers[i] = number;
    else
      n = number;
    i++;
    if (i == 5)
      Serial.println("Enter n:");
    
    else if (i == 6)
    {
      Serial.print("Numbers divisible by n: ");
      for (int j = 0; j < 5; j++)
      {
        if (numbers[j] % n == 0)
          Serial.print(String(numbers[j]) + " ");
      }
    }
  }
}
