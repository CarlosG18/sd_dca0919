#define F_CPU 16000000UL //define a frequência do microcontrolador - 16MHz

#include <avr/io.h> //definições do componente especificado
#include <util/delay.h> //biblioteca para o uso das rotinas de _delay_ms e _delay_us()
#include <avr/pgmspace.h>//para a gravação de dados na memória flash
#include <avr/interrupt.h>
#include "DHT.h"

//Definições de macros para o trabalho com bits
#define set_bit(y,bit) (y|=(1<<bit)) //coloca em 1 o bit x da variável Y
#define clr_bit(y,bit) (y&=~(1<<bit)) //coloca em 0 o bit x da variável Y
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22  // DHT 22
#define LED PE5
#define BUZZER 4

#define DADOS_LCD PORTH//4 bits de dados do LCD no PORTH
#define CONTR_LCD PORTB//PORT com os pinos de controle do LCD (pino R/W em 0).
#define E PB4 //pino de habilitação do LCD (enable)
#define RS PB5 //pino para informar se o dado é uma instrução ou caractere

//sinal de habilitação para o LCD
#define pulso_enable() _delay_us(1); set_bit(CONTR_LCD,E); _delay_us(1); clr_bit(CONTR_LCD,E); _delay_us(45)

typedef struct{
  float temperatura;
  float umidade;
} Dados;

Dados data_sensor[10];
int index_current = 0;

const int numbers[] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
int temp[4] = {0,0,0,0};
int temp_sensor[3] = {0,0,0};
int umid[4] = {0,0,0,0};
int umid_sensor[2] = {0,0};
int hora[4] = {0,0,0,0};
unsigned int temp_amostra[4] = {0,0,0,0};
unsigned int temp_amostra_full = 0;

int temp_min = 0;
int temp_max = 0;
int umid_min = 0;
int umid_max = 0;

int mode = 0;

bool troca_config = false;
bool add_position = false;
bool update_display = false;
bool add_value = false;
bool alarm_active = false;
bool init_configuration = false;
bool init_capture = false;

int colunas_edit[4] = {5,6,8,9};
int colunas_edit_amostra[4] = {5,6,7,8};
int col_atual = 0;
int temperatura = 0;
int vetor_temp[5] = {0,0,0,0,0};

int segundos = 0;

float h;
float t;

uint16_t sensor_fogo;

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
    escreve_LCD("t=");
    decoder_number(temp_sensor[0],0,false);
    decoder_number(temp_sensor[1],0,false);
    escreve_LCD(".");
    decoder_number(temp_sensor[2],0,false);
    escreve_LCD(" C   u=");
    decoder_number(umid_sensor[0],0,false);
    decoder_number(umid_sensor[1],0,false);
    escreve_LCD("%");
    pular_linha();
    escreve_LCD("--hora: ");
    decoder_number(hora[0],0,false);
    decoder_number(hora[1],0,false);
    escreve_LCD(":");
    decoder_number(hora[2],0,false);
    decoder_number(hora[3],0,false);
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
  }else if(mode == 3){
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
  }else{
    escreve_LCD("tempo da amostra");
    pular_linha();
    escreve_LCD(" t = ");
    decoder_number(temp_amostra[0],0,false);
    decoder_number(temp_amostra[1],0,false);
    decoder_number(temp_amostra[2],0,false);
    decoder_number(temp_amostra[3],0,false);
    escreve_LCD(" ms");
    go_lin_col(1,5);
  }
}//modo de configuração do tempo de amostragem

void Get_num(float n, int vetor[]){
  int parteInteira = (int)n;
  float parteDecimal = n - parteInteira;
    
  vetor[0] = (parteInteira / 10) % 10; // Obtém a dezena
  vetor[1] = parteInteira % 10;       // Obtém a unidade
  vetor[2] = (int)(parteDecimal * 10); // Obtém o valor após a vírgula
}

void alarm(){
  tone(BUZZER,262,10);
  _delay_ms(50);
  set_bit(PORTE,LED);
}

int Get_data(int dezena, int unidade){
  return dezena * 10 + unidade;
}

void atualizar_hora(){
  if((hora[3] + 1) > 9){
    if(hora[2] == 5){
      if(hora[0] == 2 & (hora[1]+1) > 3){
        hora[0] = 0; 
        hora[1] = 0;
        hora[2] = 0;
        hora[3] = 0;
      }else if((hora[1]+1) > 9){
        hora[0] = hora[0] + 1;
        hora[1] = 0;
        hora[2] = 0;
        hora[3] = 0;
      }else{
        hora[1] = hora[1] + 1;
        hora[2] = 0;
        hora[3] = 0;
      }
    }else{
      hora[2] = hora[2] + 1;
      hora[3] = 0;
    }
  }else{
    hora[3] = hora[3] + 1;
  }
  limpar_LCD();
  select_mode(mode);
}

void att_Data(int vetor[4], int type){
  if(type == 1){
    temp_min = Get_data(vetor[0],vetor[1]);
    temp_max = Get_data(vetor[2],vetor[3]);
  }else{
    umid_min = Get_data(vetor[0],vetor[1]);
    umid_max = Get_data(vetor[2],vetor[3]);
  }
}

bool check_parametros(uint16_t sensor){
  if(sensor > 777){
    return true;
  }else{
    return false;
  }
}

/*
bool check_parametros(float temperatura_sensor, float umidade_sensor){
  int temp_ajust_min = Get_data(temp[0],temp[1]);
  int temp_ajust_max = Get_data(temp[2],temp[3]);
  int umid_ajust_min = Get_data(umid[0],umid[1]);
  int umid_ajust_max = Get_data(umid[2],umid[3]);

  if(int(temperatura_sensor) < temp_ajust_min | int(temperatura_sensor) > temp_ajust_max | int(umidade_sensor) < umid_ajust_min | int(umidade_sensor) > umid_ajust_max){
    return true;
  }else{
    return false;
  }
}*/

void verify_addValue(int lim_min, int lim_max, int vetor[4],int type){
  att_Data(vetor,type);
  int dez_min = int(lim_min/10);
  int uni_min = lim_min % 10;
  int dez_max = int(lim_max/10);
  int uni_max = lim_max % 10;

  if(col_atual == 0){
    if((vetor[col_atual]+1) > dez_max){
      vetor[col_atual] = dez_min;
    }else{
      vetor[col_atual] = vetor[col_atual] + 1;
      if(vetor[col_atual] == dez_max & vetor[1] > 0){
        vetor[1] = uni_min;
        go_lin_col(1,colunas_edit[1]);
        decoder_number(vetor[1],colunas_edit[col_atual],true);
      }
      att_Data(vetor,type);
      if(type == 1){
        if(temp_min > temp_max){
        vetor[2] = vetor[col_atual];
        go_lin_col(1,colunas_edit[2]);
        decoder_number(vetor[2],colunas_edit[col_atual],true);
        vetor[3] = vetor[col_atual+1];
        go_lin_col(1,colunas_edit[3]);
        decoder_number(vetor[3],colunas_edit[col_atual],true);
        }
      }else{
        if(umid_min > umid_max){
        vetor[2] = vetor[col_atual];
        go_lin_col(1,colunas_edit[2]);
        decoder_number(vetor[2],colunas_edit[col_atual],true);
        vetor[3] = vetor[col_atual+1];
        go_lin_col(1,colunas_edit[3]);
        decoder_number(vetor[3],colunas_edit[col_atual],true);
        }
      }
      
    }
  }else if(col_atual == 1){
    if(vetor[col_atual-1] == dez_min & (vetor[col_atual]+1) > 9){
      vetor[col_atual] = uni_min;
    }else if((vetor[col_atual]+1) > 9){
      vetor[col_atual] = 0;
    }else{
      vetor[col_atual] = vetor[col_atual] + 1;
      att_Data(vetor,type);
      if(type == 1){
        if(temp_min > temp_max){
          vetor[3] = vetor[col_atual];
          decoder_number(vetor[3],colunas_edit[3],true);
        }
      }else{
        if(umid_min > umid_max){
          vetor[3] = vetor[col_atual];
          decoder_number(vetor[3],colunas_edit[3],true);
        }
      }
    }
  }else if(col_atual == 2){
    if((vetor[col_atual]+1) > dez_max){
      vetor[col_atual] = dez_min;
      att_Data(vetor,type);
      if(type == 1){
        if(temp_max < temp_min){
          vetor[0] = vetor[2];
          go_lin_col(1,colunas_edit[0]);
          decoder_number(vetor[0],colunas_edit[col_atual],true);
          vetor[1] = vetor[3];
          go_lin_col(1,colunas_edit[1]);
          decoder_number(vetor[1],colunas_edit[col_atual],true);
        }
      }else{
        if(umid_max < umid_min){
          vetor[0] = vetor[2];
          go_lin_col(1,colunas_edit[0]);
          decoder_number(vetor[0],colunas_edit[col_atual],true);
          vetor[1] = vetor[3];
          go_lin_col(1,colunas_edit[1]);
          decoder_number(vetor[1],colunas_edit[col_atual],true);
        }
      }
    }else{
      vetor[col_atual] = vetor[col_atual] + 1;
      if(vetor[col_atual] == dez_max & vetor[3] > uni_max){
        vetor[3] = 0;
        go_lin_col(1,colunas_edit[3]);
        decoder_number(vetor[3],colunas_edit[col_atual],true);
      }
    }
  }else if(col_atual == 3){
    if((vetor[col_atual]+1) > 9 | vetor[col_atual-1] == dez_max){
      vetor[col_atual] = 0;
    }else{
      vetor[col_atual] = vetor[col_atual] + 1;
    }
  }
  decoder_number(vetor[col_atual],colunas_edit[col_atual],true);
}

unsigned int get_time_full(){
  uint16_t time_full = 0;
  uint16_t escala = 0;
  time_full = time_full + (static_cast<uint16_t>(temp_amostra[0]) * 1000);
  time_full = time_full + (static_cast<uint16_t>(temp_amostra[1]) * 100);
  time_full = time_full + (static_cast<uint16_t>(temp_amostra[2]) * 10);
  time_full = time_full + (static_cast<uint16_t>(temp_amostra[3]) * 1);
  escala = ((125*time_full)/8) - 1;
  
  return escala;
}

uint16_t read_poten(){
  ADCSRA |= (1 << ADSC);
  while(ADCSRA & (1 << ADSC));
  return ADC;
}

void config_temp_amostra(int value){
  //configuração do temporiador para a amostragem
  TCCR4A = 0;
  TCCR4B = (1 << CS42) | (1 << CS40);
  TCNT4 = 0;
  OCR4A = value;
  TIMSK4 |= (1 << OCIE4A);
}

void setup(){
  Serial.begin(9600);
  dht.begin();

  DDRH = 0xFF; //PORTH como saída - dados do lcd
  DDRB = 0xFF; //PORTB como saída - controle do lcd
  DDRD = 0x0;
  DDRF = 0x0;
  DDRE = 0b00100000;
  DDRG = 0b00100000;

  //configurações do ADC - potenciometro relacionado ao sensor de incendio
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //prescale 1024

  // configuração do temporizador do sensor
  TCCR1A = 0; // Configurar TCCR1A para o modo normal (nenhuma alteração necessária)
  TCCR1B = (1 << CS12) | (1 << CS10); // Configurar o prescaler para 1024
  TCNT1 = 0;   // Configurar o valor inicial de TCNT1 para 0
  OCR1A = 31249; // 16 MHz / (1024 * 1) - 1 = 15624
  TIMSK1 |= (1 << OCIE1A); // Habilitar a interrupção de comparação A
  
  // configuração do temporizador do relogio
  TCCR3A = 0;
  TCCR3B = (1 << CS32) | (1 << CS30);
  TCNT3 = 0;
  OCR3A = 62499;
  TIMSK3 |= (1 << OCIE3A);

  //configuração das interrupções
  EICRA = 0b000111111;
  EIMSK = 0b000000111;
  sei();

  inic_LCD_4bits();
  escreve_LCD("iniciando...");

  _delay_ms(2000);
  limpar_LCD();
  t = dht.readTemperature();
  Get_num(t, temp_sensor);
  select_mode(mode);
}

void loop(){
  if(troca_config){
    if((mode+1) > 4){
      mode = 0;
      init_capture = true;
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
    
    if(mode == 4){
      go_lin_col(1,colunas_edit_amostra[col_atual]);
    }else{
      go_lin_col(1,colunas_edit[col_atual]);
    }
    add_position = false;
  }
  
  if(add_value){
    if(mode == 1){
      verify_addValue(0,50,temp,1);
    }else if(mode == 2){
      verify_addValue(20,90,umid,2);
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
    }if(mode == 4){
      if(temp_amostra[col_atual] + 1 > 9){
        temp_amostra[col_atual] = 0;
      }else{
        temp_amostra[col_atual] = temp_amostra[col_atual] + 1;
      }
      decoder_number(temp_amostra[col_atual],colunas_edit_amostra[col_atual],true);
    }
    add_value = false;
  }

  if(alarm_active){
    alarm();
  }

  if(init_capture){
    temp_amostra_full = get_time_full();
    config_temp_amostra(temp_amostra_full);
    init_capture = false;
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

ISR(TIMER1_COMPA_vect) {
  h = dht.readHumidity();
  t = dht.readTemperature();
  Get_num(t, temp_sensor);
  Get_num(h, umid_sensor);
  if(mode == 0){
    limpar_LCD();
    select_mode(mode);
  }
  clr_bit(PORTE,LED);
  sensor_fogo = read_poten();
  alarm_active = check_parametros(sensor_fogo);
  TCNT1 = 0;
}

ISR(TIMER3_COMPA_vect){//timer do relogio
  if(mode != 3){
    segundos = segundos + 1;
  }
  if(segundos == 15){
    atualizar_hora();
    segundos = 0;
  }
  TCNT3 = 0;
}

ISR(TIMER4_COMPA_vect){//timer do tempo de amostragem
  Serial.println();
  h = dht.readHumidity();
  t = dht.readTemperature();
  data_sensor[index_current].temperatura = t;
  data_sensor[index_current].umidade = h;
  if(index_current + 1 > 10){
    index_current = 0;
    TCCR4B = 0;
  }else{
    index_current = index_current + 1;
    print_data();
  }
  TCNT4 = 0;
}

void print_data(){
  for(int i=0; i<index_current; i++){
    Serial.print("aferição[");
    Serial.print(i);
    Serial.print("] t = ");
    Serial.print(data_sensor[i].temperatura);
    Serial.print(" u = ");
    Serial.println(data_sensor[i].umidade);
  }
}
