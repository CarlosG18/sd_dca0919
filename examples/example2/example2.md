# Semáfaro - Lista 01 – Portas de saída

**Problema**: Construção de um semáforo.
Projete um programa para a construção de um semáforo na qual o sinal fechado fica `5s`, o
sinal de atenção `2s` e o sinal de aberto fica `5s`.

## objetivos
1. Faça o programa utilizado os comandos nativos do arduino.
2. Refaça o programa utilizando C, aplicando os sinais diretamente nos registradores.
3. Faça a simulação para verificação dos códigos.
4. Faça uma comparação entre os dois programas explicando quais comandos são
equivalentes.

## equivalências nos comandos

|comando nativo| comando registrador| o que o comando faz (alto nivel)|o que o comando faz (baixo nivel)|
|:--:|:--:|:--:|:--:|
|pinMode(LedRed, OUTPUT),pinMode(LedYellow, OUTPUT),pinMode(LedGreen, OUTPUT)|DDRD = 0x1c| configura os pinos do arduino em entrada ou saida|configura os pinos do registrador D do microcontrolador em entrada(bit 0) ou saida(bit 1)|
|digitalWrite(LedRed, HIGH)|PORTD = 0x4| atribui ao led vermelho o valor HIGH| escreve o bit 1 no pino 2 do registrador D|
|digitalWrite(LedRed, LOW)|PORTD = 0x0| atribui ao led vermelho o valor LOW| escreve o bit 0 em todos os pinos do registrador D|
|digitalWrite(LedGreen, HIGH)|PORTD = 0x10|atribui ao led verde o valor HIGH| escreve o bit 1 no pino 4 do registrador D|
|digitalWrite(LedYellow, HIGH)|PORTD = 0x8|atribui ao led amarelo o valor HIGH| escreve o bit 1 no pino 3 do registrador D|

- os comandos `pinMode(LedRed, OUTPUT),pinMode(LedYellow, OUTPUT),pinMode(LedGreen, OUTPUT)` é equivalente ao `DDRD = 0x1c`
- o comando  `digitalWrite(LedRed, HIGH)` é equivalente ao `PORTD = 0x4`
- o comando `digitalWrite(LedRed, LOW)` é equivalente ao `PORTD = 0x0`
- o comando `digitalWrite(LedGreen, HIGH)` é equivalente ao `PORTD = 0x10`
- o comando `digitalWrite(LedYellow, HIGH)` é equivalente ao `PORTD = 0x8`