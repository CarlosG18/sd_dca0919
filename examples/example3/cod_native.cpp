//ligar um led com um botão enquanto ele estiver precionado em código nativo do arduino
void setup()
{
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
}

void loop()
{
  while(digitalRead(3)){
	  digitalWrite(2, HIGH);
  }
  digitalWrite(2, LOW);
}