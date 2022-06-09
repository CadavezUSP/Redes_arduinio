###Integrantes
-Felipe cadavez Oliveira nUSP: 11208558
-Kevin Naoto Ueda - nUSP: 10337022
-Paulo Cesar de Moraes Filho nUSP: 10734303

##Objetivos

Esse projeto tem como objetivo programar um arduino para enviar uma mensagem usando a codificação Ascii de caractere para binário.

##Especificações

- O arduino é programado como um emissor com bit de paridade par, portanto para ler os dados deve ser codificado um arduino tambem com bit de paridade par.

- cada dado tem 8 bits (1 bytes), 1 bit sendo o bit de paridade e os outros 7 representando o caractere
- a mensagem é enviada bit a bit

- para iniciar a leirtura deve ser colocado no pino CTS do receptor o valor HIGH (1), enquanto o RTS estiver em HIGH 1

- o fim da transmissão ocorre quando o pino RTS mudar seu valor para Low (0)

##Conexões
- os dados serão enviados pelo pino RX - pino 13
- o CTS do receptor deve ser conectado no pino 12 do leitor
- o RTS do emissor estará associado ao pino 11