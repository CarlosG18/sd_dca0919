# Display LCD (16x2)

observações para o LCD:

1. O pino RS é responsavel por definir se os dados que estão no LCD se tratam de uma instrução ou um caractere que deverá ser inscrito.
2. Exixtem dois tipos de transferência de dados no LCD: **8 bits** e **4 bits**;
3. Na transferência por 4 bits, você deverá inviar os dados duas vezes, a primeira para os 4 bits mais significativos e a segunda para os 4 bits menos significativos; 
4. para cada instrução/caractere devemos realizar um pulso no enable, ou seja fazendo com que o enable seja 1 e logo em seguida 0 em um curto periodo de tempo.

## lista de instruções

As instruções do LCD são encontradas no datasheet do LCD que você estiver usando. temos um exemplo da lista de instruções do LCD 1602.

![lista de comandos do LCD 1602](https://github.com/CarlosG18/sd_dca0919/blob/main/imagens/lista_comandos_lcd.png)

## código do LCD (16x2)

```c
#define F_CPU 16000000UL //define a frequência do microcontrolador - 16MHz

#include <avr/io.h> //definições do componente especificado
#include <util/delay.h> //biblioteca para o uso das rotinas de _delay_ms e _delay_us()
#include <avr/pgmspace.h>//para a gravação de dados na memória flash

//Definições de macros para o trabalho com bits
#define set_bit(y,bit) (y|=(1<<bit)) //coloca em 1 o bit x da variável Y
#define clr_bit(y,bit) (y&=~(1<<bit)) //coloca em 0 o bit x da variável Y
#define cpl_bit(y,bit) (y^=(1<<bit)) //troca o estado lógico do bit x da variável Y
#define tst_bit(y,bit) (y&(1<<bit)) //retorna 0 ou 1 conforme leitura do bit

#define DADOS_LCD PORTH //4 bits de dados do LCD no PORTH
#define CONTR_LCD PORTB//PORT com os pinos de controle do LCD (pino R/W em 0).
#define E PB4 //pino de habilitação do LCD (enable)
#define RS PB5 //pino para informar se o dado é uma instrução ou caractere

//sinal de habilitação para o LCD
#define pulso_enable() _delay_us(1); set_bit(CONTR_LCD,E); _delay_us(1); clr_bit(CONTR_LCD,E); _delay_us(45)

const unsigned char mensagem[] = "DADOS"; //mensagem armazenada na memória flash

void cmd_LCD(unsigned char c, char cd, bool flag){
  if(cd==0){
    clr_bit(CONTR_LCD,RS); // RS = 0 --> instrução
  }else{
    set_bit(CONTR_LCD,RS); //RS = 1 --> caractere
  }

  if(flag){
    DADOS_LCD = (DADOS_LCD & 0x87)|(c >> 1); //carregando dados do mais significativo
    pulso_enable();
  }else{
    DADOS_LCD = (DADOS_LCD & 0x87)|(c >> 1); //carregando dados do mais significativo
    pulso_enable();
    DADOS_LCD = (DADOS_LCD & 0x87) | (c<<3); //carregando dados do menos significativo
    pulso_enable();
  }

  if((cd==0) && (c<4)) //se for instrução de retorno ou limpeza espera LCD estar pronto
    _delay_ms(2);
}

void inic_LCD_4bits()//sequência ditada pelo fabricando do circuito integrado HD44780
{
clr_bit(CONTR_LCD,RS);//RS em zero indicando que o dado para o LCD será uma instrução
clr_bit(CONTR_LCD,E);//pino de habilitação em zero
_delay_ms(20);

cmd_LCD(0x28,0,false); //modo de configuração do LCD
cmd_LCD(0x08,0,true); //desligando o LCD
cmd_LCD(0x01,0,false); //limpando o LCD
cmd_LCD(0xD,0,false); //ligando o LCD
}

void escreve_LCD(char *c)
{
for (; *c!=0;c++) cmd_LCD(*c,1,false);
}

void pular_linha(){
  cmd_LCD(0xC0,0,false);
}

void limpar_LCD(){
  cmd_LCD(0x01,0,false);
}

int main()
{
DDRH = 0xFF; //PORTH como saída - dados do lcd
DDRB = 0xFF; //PORTB como saída - controle do lcd
inic_LCD_4bits();

for(;;){}
//laço infinito, aqui vai o código principal
}

```

## montagem do circuito 

![circuito](https://github.com/CarlosG18/sd_dca0919/blob/main/imagens/circuito_lcd.png)