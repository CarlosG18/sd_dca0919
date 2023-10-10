#define F_CPU 16000000UL //define a frequência do microcontrolador - 16MHz

#include <avr/io.h> //definições do componente especificado
#include <util/delay.h> //biblioteca para o uso das rotinas de _delay_ms e _delay_us()
#include <avr/pgmspace.h>//para a gravação de dados na memória flash
#include <avr/interrupt.h>
#include <math.h>
#include <DHT.h>

//Definições de macros para o trabalho com bits
#define set_bit(y,bit) (y|=(1<<bit)) //coloca em 1 o bit x da variável Y
#define clr_bit(y,bit) (y&=~(1<<bit)) //coloca em 0 o bit x da variável Y
#define cpl_bit(y,bit) (y^=(1<<bit)) //troca o estado lógico do bit x da variável Y
#define tst_bit(y,bit) (y&(1<<bit)) //retorna 0 ou 1 conforme leitura do bit
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

#define DADOS_LCD PORTH//4 bits de dados do LCD no PORTH
#define CONTR_LCD PORTB//PORT com os pinos de controle do LCD (pino R/W em 0).
#define E PB4 //pino de habilitação do LCD (enable)
#define RS PB5 //pino para informar se o dado é uma instrução ou caractere

//sinal de habilitação para o LCD
#define pulso_enable() _delay_us(1); set_bit(CONTR_LCD,E); _delay_us(1); clr_bit(CONTR_LCD,E); _delay_us(45)

const int numbers[] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
int temp[4] = {0,0,0,0};
int umid[4] = {0,0,0,0};
int hora[4] = {0,0,0,0};

int mode = 0;

bool troca_config = false;
bool add_position = false;
bool add_value = false;

int colunas_edit[4] = {5,6,8,9};
int col_atual = 0;

int temperatura = 0;
int vetor_temp[5] = {0,0,0,0,0};

DHT dht(DHTPIN, DHTTYPE);

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

void linha_inicial(){
  limpar_LCD();
  col_atual = 0;
  cmd_LCD(0x80,0,false);
  select_mode(mode);
}

void go_lin_col(int lin, int col){
  const int matriz_lcd[2][16] = {{0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F},{0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF}};
  cmd_LCD(matriz_lcd[lin][col],0,false);
}

void decoder_number(int number, int col, bool jump){
  cmd_LCD(numbers[number],1,false);
  if(jump)
    go_lin_col(1,col);
}

void select_mode(int mode){
  if(mode == 0){
    escreve_LCD("temp:00 umid:00%");
    pular_linha();
    escreve_LCD("--hora: ");
    go_lin_col(1,8);
    decoder_number(hora[0],0,false);
    decoder_number(hora[1],0,false);
    go_lin_col(1,10);
    escreve_LCD(":");
    go_lin_col(1,11);
    decoder_number(hora[2],0,false);
    decoder_number(hora[3],0,false);
    go_lin_col(1,13);
    escreve_LCD("---");
  }else if(mode == 1){
    escreve_LCD(" faixa de temp:");
    go_lin_col(1,0);
    escreve_LCD("     ");
    go_lin_col(1,5);
    decoder_number(temp[0],0,false);
    go_lin_col(1,6);
    decoder_number(temp[1],0,false);
    go_lin_col(1,7);
    escreve_LCD("-");
    go_lin_col(1,8);
    decoder_number(temp[2],0,false);
    go_lin_col(1,9);
    decoder_number(temp[3],0,false);
    go_lin_col(1,10);
    escreve_LCD("     ");
    go_lin_col(1,5);
  }else if(mode == 2){
    escreve_LCD("faixa de umidade");
    go_lin_col(1,0);
    escreve_LCD("     ");
    go_lin_col(1,5);
    decoder_number(umid[0],0,false);
    go_lin_col(1,6);
    decoder_number(umid[1],0,false);
    go_lin_col(1,7);
    escreve_LCD("-");
    go_lin_col(1,8);
    decoder_number(umid[2],0,false);
    go_lin_col(1,9);
    decoder_number(umid[3],0,false);
    go_lin_col(1,10);
    escreve_LCD("     ");
    go_lin_col(1,5);
  }else{
    escreve_LCD("  ajustar hora  ");
    go_lin_col(1,0);
    escreve_LCD("     ");
    go_lin_col(1,5);
    decoder_number(hora[0],0,false);
    go_lin_col(1,6);
    decoder_number(hora[1],0,false);
    go_lin_col(1,7);
    escreve_LCD(":");
    go_lin_col(1,8);
    decoder_number(hora[2],0,false);
    go_lin_col(1,9);
    decoder_number(hora[3],0,false);
    go_lin_col(1,10);
    escreve_LCD("     ");
    go_lin_col(1,5);
  }
}

bool checkset_hora(int hora[4]){
  return true;
}

void get_sensor(){
  
	set_bit(ADCSRA, ADSC); //inicia a conversão
	while(tst_bit(ADCSRA,ADSC));//espera a conversão ser finalizada
  temperatura = ADC;

  //return (ADC - Offset_temp);//fator k de divisão = 1
}

void vect_temp(){
  for(int i = 0; i < 5; i++){
    vetor_temp[i] = temperatura;
    //Serial.println(vetor_temp[i]);
  }
}

int main()
{
Serial.begin(9600);
dht.begin();

DDRH = 0xFF; //PORTH como saída - dados do lcd
DDRB = 0xFF; //PORTB como saída - controle do lcd
DDRD = 0x0;
DDRF = 0x0;

/*
//configurações do contador do relogio
TCCR1A = 0b11000000;
TCCR1B = 0b00001001;
//OCR1A = 0b0011110100001000;
//TIMSK1 = 0b00000001;
*/
//TCNT1H and TCNT1L

//configuração das interrupções
EICRA = 0b000111111;
EIMSK = 0b000000111;
sei();

inic_LCD_4bits();

select_mode(mode);
//get_sensor();

_delay_ms(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
//float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  //float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dht.readTemperature(true);

  //Serial.println(h);
  // Check if any reads failed and exit early (to try again).
  //if (isnan(h) || isnan(t) || isnan(f)) {
    //Serial.println(F("Failed to read from DHT sensor!"));
    //return;
  //}

  // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);

  //Serial.print(F("Humidity: "));
  //Serial.print(h);
  //Serial.print(F("%  Temperature: "));
  //Serial.print(t);
  //Serial.print(F("°C "));
  //Serial.print(f);
  //Serial.print(F("°F  Heat index: "));
  //Serial.print(hic);
  //Serial.print(F("°C "));
  //Serial.print(hif);
  //Serial.println(F("°F"));

for(;;){
  

  if(troca_config){
    if((mode+1) > 3){
      mode = 0;
    }else{
      mode = mode + 1;
      col_atual = 0;
    }
    limpar_LCD();
    select_mode(mode);
    troca_config = false;
  }

  if(add_position){
    if(col_atual+1 > 3){
      col_atual = 0;
    }else{
      col_atual = col_atual + 1;
    }
    go_lin_col(1,colunas_edit[col_atual]);
    add_position = false;
  }
  
  if(add_value){
    if(mode == 1){
      if((temp[col_atual] + 1) > 9){
        temp[col_atual] = 0;
      }else{
        temp[col_atual] = temp[col_atual] + 1;
      }
      decoder_number(temp[col_atual],colunas_edit[col_atual],true);
    }else if(mode == 2){
      if((umid[col_atual] + 1) > 9){
        umid[col_atual] = 0;
      }else{
        umid[col_atual] = umid[col_atual] + 1;
      }
      decoder_number(umid[col_atual],colunas_edit[col_atual],true);
    }else if(mode == 3){
      if(col_atual == 0 & (hora[col_atual] + 1) == 2 & hora[1] > 3){
        hora[1] = 0;
        go_lin_col(1,colunas_edit[1]);
        decoder_number(0,colunas_edit[1],true);
        go_lin_col(1,colunas_edit[0]);
      }
      if(col_atual == 0 & (hora[col_atual] + 1) > 2 || col_atual == 1 & hora[col_atual-1] == 0 & (hora[col_atual] + 1) > 9 ||col_atual == 1 & hora[col_atual-1] == 2 & (hora[col_atual] + 1) > 3 || col_atual == 1 & hora[col_atual-1] == 1 & (hora[col_atual] + 1) > 9 || col_atual == 2 & (hora[col_atual] + 1) > 5 || col_atual == 3 & (hora[col_atual] + 1) > 9){
        hora[col_atual] = 0;
      }else{
        hora[col_atual] = hora[col_atual] + 1;
      }
      decoder_number(hora[col_atual],colunas_edit[col_atual],true);
    }
    add_value = false;
  }
}

}

ISR(INT0_vect){// editar o numero
  if(mode != 0){
    add_value = true;
  }
}

ISR(INT1_vect){// mudar o modo de configuração
  troca_config = true;
}

ISR(INT2_vect){ //selecionar o numero para editar
  if(mode != 0){
    add_position = true;
  }
}

/*
ISR(TIMER1_OVF_vect){
  Serial.println("ok");
}*/
