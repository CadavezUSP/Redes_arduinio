
#define PINO_RX 13
#define PINO_CTS 12
#define PINO_RTS 11
#define BAUD_RATE 1
#define HALF_BAUD 1000/(2*BAUD_RATE)

#include "Temporizador.h"
 byte dataB; // dado em bytes/binario
 byte Bit_counter; //ponteiro de bits
 size_t dataSize; // tamanho do dado
 int code =0; // identificador de estados:
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
  if (Bit_counter == dataSize){
    byte bitP = bitParidade(dataB);
    if (bitP == 1){
     Serial.print("1");
     digitalWrite(PINO_RX, HIGH);
    }
    else{
     Serial.print("0");
     digitalWrite(PINO_RX,LOW);
    }
  }
  // envia os demais bits
  else if (Bit_counter >= 0){
    byte sign = bitRead(dataB, Bit_counter);
    if (sign == 1){
      Serial.print("1");
      digitalWrite(PINO_RX, HIGH);
    }
    else{
      Serial.print("0");
      digitalWrite(PINO_RX,LOW);
    }
  }
  //anda com o ponteiro para os bits
  Bit_counter--;
  // como é byte 0 -1 = 255, pois da a volta
  if (Bit_counter == 255){
    code = 2;
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
  // Configura timer
  configuraTemporizador(BAUD_RATE);
  // habilita interrupcoes
  interrupts();
}

// O loop() eh executado continuamente (como um while(true))
void loop ( ) {
  //Three way handshake
  if (code == 0){
//  digitalWrite(PINO_RTS, HIGH);
//  int resp = digitalRead(PINO_CTS);
    //fica preso aqui até receber o sinal que pode enviar dados
//  while(resp == LOW){
//    digitalWrite(PINO_RTS, HIGH);
//    resp = digitalRead(PINO_CTS);
//  }
  code = 1;
  }

  // inicia o temporizador pela primeira vez
 if (code == 1){

    if (!Serial.available()){
      Serial.println("Digite uma Mensagem");
    }
    while(!Serial.available());
    char dado = Serial.read();
    dataB = dado;
    dataSize = findHighestBit(dado);
    Bit_counter = dataSize;
//    Serial.println(Bit_counter);
    Serial.print(dado);
    Serial.print(" - ");
    iniciaTemporizador();
    code = 4;
   }

// para o temporizador e verifica se deve continuar enviando ou parar
  else if (code == 2){
      Serial.println();
      paraTemporizador();
      while(!Serial.available());
      char dado = Serial.read();
      if (dado == '\n'){
        code = 3;
      }
      else{
        Serial.print(dado);
        Serial.print(" - ");
        dataSize = findHighestBit(dado);
        Bit_counter = dataSize;
        dataB = dado;
        code = 4;
        iniciaTemporizador();
      }
    }
   // para o envio
  if (code == 3){
    digitalWrite(PINO_RTS, LOW);
//    return;
    code = 0;
    Serial.println("mensagem enviada!");
   }
    
}
