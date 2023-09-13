# sd_dca0919
implementações realizada na disciplina de sistemas digitais - DCA0919 - Engenharia da Computação - UFRN

# programando com o microcontrolador ATMEGA2560 na plataforma arduino

detalhes sobre o `datasheet` do ATMEGA2560 (pinout, arquitetura interna, perifericos, etc)

- programação a nivel de registradores:

```txt
- DDRx: Registrador de 'configuração'. usado para configurar se os pinos do registrador "x" como saida ou entrada. 1 = saida, 0 = entrada;
- PORTx: Registrador de saida. usado para escrever nos pinos do registrador "x";
- PINx: registrador de entrada. Usado para ler o conteúdo dos
pinos do registrador "x".
```
## exemplos práticos: 
- [Acender 3 leds](https://github.com/CarlosG18/sd_dca0919/blob/main/examples/example1/example1.md)
- [Semáfaro](https://github.com/CarlosG18/sd_dca0919/blob/main/examples/example2/example2.md)
- [Acender led atráves de um botão](https://github.com/CarlosG18/sd_dca0919/blob/main/examples/example2/example3.md)