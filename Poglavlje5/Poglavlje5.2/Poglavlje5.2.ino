const int button_S1 = PUSH1, button_S2 = PUSH2;
const int LED1_red =  YELLOW_LED;
const int LED2_red = RED_LED, LED2_green = GREEN_LED, LED2_blue = BLUE_LED;

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
  int S1_value = digitalRead(button_S1);
  int S2_value = digitalRead(button_S2);
  
  if (S1_value == HIGH && S2_value == HIGH)
    digitalWrite(LED2_red, HIGH);

  digitalWrite(LED1_red, HIGH);
  delay(1000);
  digitalWrite(LED1_red, LOW);
  delay(1000);
}
