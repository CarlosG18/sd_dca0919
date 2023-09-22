#include <avr/io.h>
#include <avr/interrupt.h>

int cont = 1;
int btn_cont = 0;
int btn_reset = 0;

void um(){
  PORTD = 0x60;
  PORTB = 0x0;
}

void dois(){
  PORTD = 0xB0; 
  PORTB = 0x5;
}

void tres(){
  PORTD = 0xF0; 
  PORTB = 0x4;
}

void quatro(){
  PORTD = 0x60;
  PORTB = 0x6;
}

void cinco(){
  PORTD = 0xD0; 
  PORTB = 0x6;
}

void seis(){
  PORTD = 0xD0; 
  PORTB = 0x7;
}

void sete(){
  PORTD = 0x70; 
  PORTB = 0x0;
}

void oito(){
  PORTD = 0xF0; 
  PORTB = 0x7;
}

void nove(){
  PORTD = 0xF0; 
  PORTB = 0x6;
}

void setNumber(int num){
  if(num == 1){
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
  delay(200);
}

void reset(){
  PORTD = 0xF0; 
  PORTB = 0x1B;
}

void setup(){
  //definindo as I/O
  DDRD = 0xF0;
  //setando os butões com pull-up
  //PORTB = 0x18;
  DDRB = 0x7;
  PORTD = 0xF0; 
  PORTB = 0x1B;
  
  //configurando as interrupções
  EICRA = 0xF; //colocando 11 - borda de subida
  EIMSK = 0x3; //ativando o INT0 e o INT1
  sei();
  
  Serial.begin(9600);
}

ISR(INT0_vect){
  Serial.println(PINB);
  setNumber(cont);
  if(PINB & (1 << 3)){
    cont++;
  }
  
  if(PINB & (1 << 4)){
    cont = 0;
  }
}

void loop(){
  
}

//rascunhos
/*

butao_cont => PB3
butao_reset => PB4

PB7 PB6 PB5 PB4 PB3 PB2 PB1 PB0
 0   0   0   1   1   0   0   0
PORTB = 

led de 7 segmentos
A => PD4
B => PD5
C => PD6
D => PD7
E => PB0
F => PB1
G => PB2

PD7 PD6 PD5 PD4 PD3 PD2 PD1 PD0
 1   1   1   1   0   0   0   0
 DDRD = 0xF0;
 
PB7 PB6 PB5 PB4 PB3 PB2 PB1 PB0
 0   0   0   0   0   1   1   1
 DDRB = 0x7;
 
     PB2 PB3 PB2 PB1 PB0 | PD7 PD6 PD5 PD4 PD3 PD2 PD1 PD0
              g   f   e  | d   c   b   a   -   -   -   -
         
0 =>  1   1   0   1   1  | 1   1   1   1   0   0   0   0  => PORTD = 0xF0; PORTB = 0x1B;
1 =>  1   1   0   0   0  | 0   1   1   0   0   0   0   0  => PORTD = 0x60; PORTB = 0x18;
2 =>  1   1   1   0   1  | 1   0   1   1   0   0   0   0  => PORTD = 0xB0; PORTB = 0x1D;
3 =>  1   1   1   0   0  | 1   1   1   1   0   0   0   0  => PORTD = 0xF0; PORTB = 0x1C;
4 =>  1   1   1   1   0  | 0   1   1   0   0   0   0   0  => PORTD = 0x60; PORTB = 0x1E;
5 =>  1   1   1   1   0  | 1   1   0   1   0   0   0   0  => PORTD = 0xD0; PORTB = 0x1E;
6 =>  1   1   1   1   1  | 1   1   0   1   0   0   0   0  => PORTD = 0xD0; PORTB = 0x1F;
7 =>  1   1   0   0   0  | 0   1   1   1   0   0   0   0  => PORTD = 0x70; PORTB = 0x18;
8 =>  1   1   1   1   1  | 1   1   1   1   0   0   0   0  => PORTD = 0xF0; PORTB = 0x1F;
9 =>  1   1   1   1   0  | 1   1   1   1   0   0   0   0  => PORTD = 0xF0; PORTB = 0x1E;

*/