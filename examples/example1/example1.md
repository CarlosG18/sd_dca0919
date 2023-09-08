# Acender 3 leds 

- Acender 3 leds atráves do uso dos registradores internos do microcontrolador ATMEGA 328p do arduino uno.

a simulação foi desenvlvida pelo tinkercad e foi obtido o seguinte esquema da montagem do circuito no arduino uno:
<p align="center">
  <img src="https://github.com/CarlosG18/sd_dca0919/blob/main/exemples/exemple1/esquema1.png" alt="esquema do circuito no arduino">
</p>

o código ultilizado foi o seguinte:

```c++
#define led 2
void setup()
{
  DDRD = 0x1c;
}

void loop()
{
  PORTD = 0x4;
  delay(200);
  PORTD = 0x0;
  delay(200);
  PORTD = 0x8;
  delay(200);
  PORTD = 0x0;
  delay(200);
  PORTD = 0x10;
  delay(200);
  PORTD = 0x0;
  delay(200);
}
```

## explicando o código

a ideia inicial do código é simples:

- cada led ficará acesso por 200ms e depois apagará também por 200ms;
- a sequência de acendimento será: vermelho, amarelo e verde;

para determinar no codigo os pinos dos registradores que são os correspondentes a pinagem do arduino, devemos análisar o [datasheet do arduino uno](https://docs.arduino.cc/hardware/uno-rev3) e observar seu pinout. podemos observar na imagem abaixo:

<p align="center">
  <img src="https://github.com/CarlosG18/sd_dca0919/blob/main/exemples/exemple1/pinout_uno.png" alt="pinout do arduino uno">
</p>

ao analisarmos o esquema de montagem do circuito, podemos ver que os pinos digitais (2,3,4) foram usados para conectar os leds (vermelho, amarelo e verde) respectivamente. 

assim, precisamos fazer a associação de quais são os registradores correspondentes a essas portas. ao analisarmos o pinout, podemos ver que os pinos D2, D3 e D4 (do arduino) são conectados ao `registrador D` do microcontrolador através dos pinos (PD2, PD3 e PD4) do registrador D.

como os registradores do microcontrolador trabalham com 8 bits, temos então:

> PD2 -> 00000100
> PD3 -> 00001000
> PD4 -> 00010000

onde cada bit corresponde a um pino do registrador.

precisamos fazer a configuração dos pinos do registrador D. precisamos informar quais pinos serão de entrada(0) e quais pinos serão de saida(1). como precisamos setar os pinos 2, 3 e 4 como saida precisamos carregar a seguinte configuração no registrador:

> registrador D -> 00011100 = 28 = 0x1c

para isso usamos o seguinte comando usando:

```c++
void setup(){
  DDRD = 0x1c;
}
```
este comando é o equivalente em alto nivel com o `pinMode(2, OUTPUT)`.

com os pinos corretamente setados, partimos para o código da parte do `loop`:

para fazermos o led vermelho piscar usamos o comando para escrever no registrador D:

```c++
void loop(){
  PORTD = 0x4;
  delay(200);
  PORTD = 0x0;
}
```
com o comando `PORTD = 0x4` temos que o registrador deve carregar com `00000100`, ou seja, o pino 2 será escrito com o `HIGH`. esse comando em alto nivel é o equivalente ao `digitalWrite(2, HIGH)`. para fazer o pino 'apagar', basta carregar o registrador D com zeros `PORTD = 0x0`.

A mesma lógica é aplicada para o led amarelo e verde.
