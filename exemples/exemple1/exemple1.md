# Acender 3 leds 

- Acender 3 leds atráves do uso dos registradores internos do microcontrolador ATMEGA 328p do arduino uno

a simulação foi desenvlvida pelo tinkercad e foi obtido o seguinte esquema da montagem do circuito no arduino uno:
![esquema da montagem do circuito no arduino](https://github.com/CarlosG18/sd_dca0919/blob/main/exemples/exemple1/esquema1.png)

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