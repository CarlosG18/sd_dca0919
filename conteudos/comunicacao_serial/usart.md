# Protocolo de comunicação Serial USART

USART, que significa "Universal Synchronous Asynchronous Receiver Transmitter", é um protocolo de comunicação serial amplamente utilizado para a transferência de dados entre dispositivos. Ele pode operar de forma síncrona ou assíncrona, o que oferece flexibilidade em diferentes aplicações.

Aqui estão os principais aspectos do protocolo USART:

- Modos de Operação:

    - Assíncrono (UART): Neste modo, os dados são transferidos de forma assíncrona, ou seja, sem um sinal de clock compartilhado entre os dispositivos. Cada byte é enviado com bits de start e stop para sincronização.
    - Síncrono: Neste modo, um sinal de clock é compartilhado entre os dispositivos para sincronizar a transferência de dados. Isso é útil em aplicações onde a precisão do tempo é crítica.

- Configuração de Baud Rate:

    O Baud Rate representa a taxa de transferência de dados e deve ser configurado corretamente nos dispositivos conectados para garantir uma comunicação eficaz.

- Estrutura do Pacote de Dados:

    Cada pacote de dados normalmente consiste em bits de start, dados, bits de paridade (opcional), bits de stop e, possivelmente, bits de controle adicionais. O número de bits de dados, paridade e bits de stop pode variar de acordo com a configuração.

**TEXTO GERADO PELO CHATGPT**

## como usar?

Para usarmos o USART precisamos configurar alguns registradores:

- **UBRRn - Usart Baud Rate Register**: para configurar o baud rate use o registrador 

- **UDRn - Usart I/O Data Register**: registrador onde os dados serão armazenados 

- **UCSRnA - Usart Control and Status Register A**: 

- **UCSRnB - Usart Control and Status Register B**: 

- **UCSRnC - Usart Control and Status Register C**: 

