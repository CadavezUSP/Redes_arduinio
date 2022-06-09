
#define PINO_RX 13
#define PINO_CTS 11
#define PINO_RTS 12
#define CLOCK 10
#define BAUD_RATE 1
#define HALF_BAUD 1000/(2*BAUD_RATE)

#include "Temporizador.h"
 byte dataB; // dado em bytes/binario
 byte Bit_counter; //ponteiro de bits
 size_t dataSize; // tamanho do dado
 int code =-1; // identificador de estados:
 // 0 - tentando three way handshake
 // 1 - primeiro bit enviado e primeira ativação do temporizador
 // 2 - interrupção do temporizador e verificação de continuidade
 // 3 - fim da transmissão
 // 4 - transmitindo

// acha o tamanho do bit
byte findHighestBit(char x)
{
  for (int bit = 15; bit >= 0; bit--)
  {
    if (bitRead(x, bit))
    {
      return bit + 1;
    }
  }
  return 0;
}

// calcula o bit de paridade para par
byte bitParidade(byte dado){
  int counter = 0;
  byte compare = 1;
  byte bitP;
  for (byte i=0; i<8; i++){
    byte aux = bitRead(dado, i);
    if (aux == compare){
      counter ++;
    }
  }
  if (counter %2 ==0){
    bitP = 0;
  }
  else{
    bitP = 1;
  }
  return bitP;
}

// Rotina de interrupcao do timer1
// Anda bit a bit do dado binario enviando e imprimindo no Serial 
ISR(TIMER1_COMPA_vect){
  //envia o bit de paridade
  if (code == 4){
  // envia os demais bits
    if (Bit_counter >= 0){
      byte sign = bitRead(dataB, Bit_counter);
      Serial.print("bit enviado: ");
      Serial.print(sign);
      if (sign == 1){
//        Serial.print(" e 1");/
        digitalWrite(13, HIGH);
      }
      else{
//        Serial.print(" e 0");/
        digitalWrite(13,LOW);
      }
      Serial.println();
    }

    if (Bit_counter == 255){
      byte bitP = bitParidade(dataB);
      Serial.print("bitP: ");
      Serial.print(bitP);
      if (bitP == 1){
//      Serial.print(" e 1");/
      digitalWrite(13, HIGH);
    }
    else{
//        Serial.print(" e 0");/
        digitalWrite(13,LOW);
      }
    Serial.println();
    }
  Bit_counter--;
  }
  //anda com o ponteiro para os bits
  // como é byte 0 -1 = 255, pois da a volta
  if (Bit_counter == 254){
    Serial.print("Code ");
    code = 2;
    Serial.print(code);
  }
}

// Executada uma vez quando o Arduino reseta
void setup(){
  //desabilita interrupcoes
  noInterrupts();
  // Configura porta serial (Serial Monitor - Ctrl + Shift + M)
  Serial.begin(9600);
  // Inicializa TX ou RX
  pinMode(PINO_RX, OUTPUT);
  pinMode(PINO_CTS, INPUT);
  pinMode(PINO_RTS, OUTPUT);
//  pinMode(CLOCK, INPUT);/
  // Configura timer
  configuraTemporizador(BAUD_RATE);
  // habilita interrupcoes
  interrupts();
}

// O loop() eh executado continuamente (como um while(true))
void loop ( ) {
  if (code == -1){
    if (!Serial.available()){
      Serial.println("Digite uma Mensagem");
    }
    while(!Serial.available());
    char dado = Serial.read();
    dataB = (byte)dado;
    Serial.println((char)dataB);
    code = 0;
    }
  //Three way handshake
  if (code == 0){
     
    digitalWrite(PINO_RTS, HIGH);
    byte resp = digitalRead(PINO_CTS);
    //fica preso aqui até receber o sinal que pode enviar dados
    while(resp == LOW){
//      digitalWrite(PINO_RTS, HIGH);/
      resp = digitalRead(PINO_CTS);
    }
    //delay(10);
    iniciaTemporizador();
    Serial.println("three way");
    code = 1;
  }

  // inicia o temporizador pela primeira vez
 if (code == 1){


    dataSize = 7;
    Bit_counter = dataSize;
//    Serial.println(Bit_counter);
    Serial.print((char)dataB);
    Serial.print(" - ");
//    while (!digitalRead(CLOCK));/
    code = 4;
   }

// para o temporizador e verifica se deve continuar enviando ou parar
  else if (code == 2){
      Serial.println();
      paraTemporizador();
      while(!Serial.available());
      char dado = Serial.read();
      if (dado == '\n'){
        digitalWrite(PINO_RTS, LOW);
        Serial.println("mensagem enviada!");
        code = -1;
      }
      else{
        Serial.print(dado);
        dataB = dado;
        code = 3;
//        while (!digitalRead(CLOCK));/
      }
    }
   // para o envio
  if (code == 3){
    digitalWrite(PINO_RTS, LOW);
    delay(2000);
//    return;
    code = 0;
   }
    
}
