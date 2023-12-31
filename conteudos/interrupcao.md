# Interrupções

É um mecanismo que permite que um microcontrolador pare temporariamente a execução do programa principal (conhecido como código de usuário) para lidar com eventos ou condições específicas que requerem atenção imediata. Esses eventos podem incluir:

    - Periféricos: Interrupções podem ser geradas por periféricos como temporizadores, portas de entrada/saída (GPIO), conversores analógico-digitais (ADC), e UARTs quando ocorrem eventos relevantes, como estouro de temporizador, detecção de borda em uma entrada, ou recebimento de dados em uma porta serial.

    - Exceções: Exceções, como erros de divisão por zero ou estouro de pilha, podem ser tratadas por meio de interrupções para garantir que o sistema seja mantido em um estado controlado.

    - Comunicação: Interrupções podem ser usadas para lidar com eventos de comunicação, como a chegada de dados em uma porta serial, evitando que o programa principal fique preso esperando por esses dados.

**TEXTO ACIMA GERADO PELO CHATGPT**

Mas como usar essa poderosa ferramenta na prática com os registradores do ATmega 2560? consultando o datasheet, para operar com interrupções no microcontrolador ATmega 2560 usamos os seguintes registradores:

- EICRA/EICRB
- EIMSK

### EICRA/EICRB

![Registrador EICRA](https://github.com/CarlosG18/sd_dca0919/blob/main/imagens/eicra_reg.png)

nesse registrador podemos configurar os modos de operação da interrupção, ou seja os dois primeiros bits do registrador EICRA (ISC00 e ISC01) corresponde a interrupção 0 (INT0). resumindo, podemos configurar as interrupções (INT0, INT1, INT2 e INT3) com esse registrador (EICRA). os modos de operações estão abaixo:

![modos de configurações das interrupções](https://github.com/CarlosG18/sd_dca0919/blob/main/imagens/tabela1.png)

O registrador EICRB faz a mesma coisa que o EICRA porém varre as interrupções (INT4, INT5, INT6 e INT7).

### EIMSK

![Registrador EIMSK](https://github.com/CarlosG18/sd_dca0919/blob/main/imagens/eimsk.png)

Basicamente esse registrador configura qual interrupção será abilitada. temos uma prioridade de interrupções ao qual pode ser visualizado na pagina `101` do [datasheet do Atmega 2560](https://ww1.microchip.com/downloads/en/devicedoc/atmel-2549-8-bit-avr-microcontroller-atmega640-1280-1281-2560-2561_datasheet.pdf)

após esses dois registradores serem configurados, precisamos acionar um registrador global que ativa todas as interrupções do microcontrolador. para fazer isso basta chamar a função `sei()`. isso poderia ser feito no inicio (a ordem não importa):

```cpp
    ...
    EICRA = 0x2;
    EIMSK = 0x1;
    sei();
    ...
```

com os registradores dividamentes configurados, precisamos definir a rotina de interrupção que será realizada quando a interrupção for "ativada". para isso usamos:

```cpp

ISR(<nome_da_interrupção>_vect){
    //código aqui
}

```

Neste caso, devemos substituir o <nome_da_interrupção> pelos nome do devido registrador de interrupção como por exemplo:

```cpp

ISR(INT0_vect){
    //código aqui
}

```