void setup()
{
  DDRD = 0x1c; //setando as saidas(1) e entradas(0) do registrador D - 00011100
}

void loop()
{
  PORTD = 0x4;  //escrevendo 'high' no pino 2 (led vermelho) do registrador D
  delay(5000);
  PORTD = 0x0;  //escrevendo 'low' em todos os pinos do geristrador D
  PORTD = 0x10; //escrevendo 'high' no pino 4 (led verde) do registrador D
  delay(5000);
  PORTD = 0x0;  //escrevendo 'low' em todos os pinos do geristrador D
  PORTD = 0x8;  //escrevendo 'high' no pino 3 (led amarelo) do registrador D
  delay(2000);
  PORTD = 0x0;  //escrevendo 'low' em todos os pinos do geristrador D
}