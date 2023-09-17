int alto = 0;
int cont = 0;

void setup()
{
  //fazendo a configuração dos registradores de I/O
  DDRD = 0xF8;
  DDRB = 0x3;
  //definindo o button como pull-up
  PORTD = 0x4;
}

void zero(){
  PORTD = 0xF8; 
  PORTB = 0x1;
  delay(1000);
}

void um(){
  PORTD = 0x30; 
  PORTB = 0x0;
  delay(1000);
}

void dois(){
  PORTD = 0xD8; 
  PORTB = 0x2;
  delay(1000);
}

void tres(){
  PORTD = 0x78; 
  PORTB = 0x2;
  delay(1000);
}

void quatro(){
  PORTD = 0x30;
  PORTB = 0x3;
  delay(1000);
}

void cinco(){
  PORTD = 0x68;
  PORTB = 0x3;
  delay(1000);
}

void seis(){
  PORTD = 0xE8;
  PORTB = 0x3;
  delay(1000);
}

void sete(){
  PORTD = 0x38;
  PORTB = 0x0;
  delay(1000);
}

void oito(){
  PORTD = 0xFF; 
  PORTB = 0x3;
  delay(1000);
}

void nove(){
  PORTD = 0x78;
  PORTB = 0x3;
  delay(1000);
}

void reset(){
  PORTD = 0x0;
  PORTB = 0x0;
}

void setNumber(int num){
  if(num == 0){
     zero();  
  }else if(num == 1){
  	 um();
  }else if(num == 2){
  	 dois();
  }else if(num == 3){
  	 tres();
  }else if(num == 4){
  	 quatro();
  }else if(num == 5){
  	 cinco();
  }else if(num == 6){
  	 seis();
  }else if(num == 7){
  	 sete();
  }else if(num == 8){
  	 oito();
  }else if(num == 9){
  	 nove();
  }
  reset();
}

void loop()
{
  alto = PIND;
  Serial.println(alto);
  if(alto == 3){
    Serial.println("clicou");
    setNumber(cont);
    cont = cont + 1;
  }
}

//butao => PD2

//led de 7 segmentos
//A => PD3
//B => PD4
//C => PD5
//D => PD6
//E => PD7
//F => PB0
//G => PB1

/*
PD7 PD6 PD5 PD4 PD3 PD2 PD1 PD0
 1   1   1   1   1   0   0   0
 DDRD = 0xF8;
 
PB7 PB6 PB5 PB4 PB3 PB2 PB1 PB0
 0   0   0   0   0   0   1   1
 DDRB = 0x3;
 
     PB1 PB0 PD7 PD6 PD5 PD4 PD3 PD2 PD1 PD0
      g   f   e   d   c   b   a   -   -   -
         
0 =>  0   1   1   1   1   1   1   0   0   0  => PORTD = 0xF8; PORTB = 0x1;
1 =>  0   0   0   0   1   1   0   0   0   0  => PORTD = 0x30; PORTB = 0x0;
2 =>  1   0   1   1   0   1   1   0   0   0  => PORTD = 0xD8; PORTB = 0x2;
3 =>  1   0   0   1   1   1   1   0   0   0  => PORTD = 0x78; PORTB = 0x2;
4 =>  1   1   0   0   1   1   0   0   0   0  => PORTD = 0x30; PORTB = 0x3;
5 =>  1   1   0   1   1   0   1   0   0   0  => PORTD = 0x68; PORTB = 0x3;
6 =>  1   1   1   1   1   0   1   0   0   0  => PORTD = 0xE8; PORTB = 0x3;
7 =>  0   0   0   0   1   1   1   0   0   0  => PORTD = 0x38; PORTB = 0x0;
8 =>  1   1   1   1   1   1   1   0   0   0  => PORTD = 0xFF; PORTB = 0x3;
9 =>  1   1   0   1   1   1   1   0   0   0  => PORTD = 0x78; PORTB = 0x3;

*/
