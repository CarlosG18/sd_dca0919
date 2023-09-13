//ligar um led com um botão enquanto ele estiver precionado atraves de sinais no reg
void setup(){
	DDRD = 0x4; 
}

void loop(){
  while(PIND & (1 << 3)){
  	PORTD = 0x4;	
  }
  PORTD = 0x0;
}