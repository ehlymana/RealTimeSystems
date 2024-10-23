const int button_S1 = PUSH1, button_S2 = PUSH2;
const int LED1_red =  YELLOW_LED;
const int LED2_red = RED_LED, LED2_green = GREEN_LED, LED2_blue = BLUE_LED;
int state = 1;

void setup()
{
  pinMode(LED1_red, OUTPUT);   
  pinMode(LED2_red, OUTPUT);
  pinMode(LED2_green, OUTPUT);
  pinMode(LED2_blue, OUTPUT);
  
  pinMode(button_S1, INPUT_PULLUP); 
  pinMode(button_S2, INPUT_PULLUP);
}

void loop(){
  int S1 = digitalRead(button_S1);
  int S2 = digitalRead(button_S2);

  if (state == 1 && S1 == LOW && S2 == HIGH) state = 2;
  else if (state == 1 && S1 == HIGH && S2 == LOW) state = 3;
  else if (state == 2 && S1 == LOW && S2 == LOW) state = 4;
  else if (state == 3 && S1 == LOW && S2 == LOW) state = 4;
  delay(100);

  if (state == 1)
  {
    digitalWrite(LED1_red, LOW);
    digitalWrite(LED2_red, LOW);
    digitalWrite(LED2_green, LOW);
    digitalWrite(LED2_blue, LOW);
  }
  else if (state == 2)
  {
    digitalWrite(LED2_red, LOW);
    digitalWrite(LED2_green, LOW);
    digitalWrite(LED2_blue, LOW);
    digitalWrite(LED1_red, HIGH);
    delay(1000);
    digitalWrite(LED1_red, LOW);
    delay(1000);
  }
  else if (state == 3)
  {
    digitalWrite(LED1_red, LOW);
    digitalWrite(LED2_red, LOW);
    digitalWrite(LED2_blue, LOW);
    digitalWrite(LED2_green, HIGH);
    delay(1000);
    digitalWrite(LED2_green, LOW);
    delay(1000);
  }
  else if (state == 4)
  {
    digitalWrite(LED1_red, HIGH);
    digitalWrite(LED2_red, HIGH);
    digitalWrite(LED2_green, LOW);
    digitalWrite(LED2_blue, LOW);
  }
}
