#include <avr/io.h>
#include <avr/interrupt.h>

int main(void){
  //led no pino PB1
  DDRB |= (1 << PB1);

  //configurando o temporizador (modo CTC)
  TCCR1B = 0b00001101; //config o modo do temporizador (CTC) e o prescaler - 1024 ao mesmo tempo
  OCR1A = 0b0011110100001000;//definindo o valor do OCR1A que será o TOP
  TIMSK1 = 0x1; // habilitando a interrupção de overflow do temporizador 1

  sei(); //habilitando as interrupções globais
  
  while(1){
	
  }
}

ISR(TIMER1_OVF_vect){
  PORTB ^= (1 << PB1);
}