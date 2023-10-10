# Sensor de Temperatura e umidade DHT11

verificando o [datasheet do sensor]() de umidade e temperatura **DHT11** podemos obter algumas características de seu funcionamento:

## esquema de pinout do DHT11
![pinout do sensor DHT11]()

## processo de comunicação do sensor DHT11

o sensor DHT11 possui uma saída digital, e para fazer a comunicação com o MCU, devemos seguir alguns passos definidos pelo datasheet:

1. 
2. 
3. 
4. 
5. 

## como "traduzir" os dados obtidos?

segundo o datasheet, o sensor mandará 5bytes de informação a cada leitura:
1. **1 byte**: parte inteira da umidade;
2. **2 byte**: parte decimal da umidade;
3. **3 byte**: parte inteira da temperatura;
4. **4 byte**: parte decimal da temperatura;
5. **5 byte**: checksum.

o ultimo byte `checksum`, é o byte para verificar a integridade do dado enviado pelo sensor. se somarmos os 4 bytes referentes aos dados da umidade e temperatura e o valor obtido for igual ao 5 byte, então temos que os dados enviados pelo sensor são válidos.

## montagem do circuito

![montagen do circuito para o sensor DHT11]()