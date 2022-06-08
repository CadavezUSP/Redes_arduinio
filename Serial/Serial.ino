
#define PINO_RX 13
#define PINO_CTS 12
#define PINO_RTS 11
#define BAUD_RATE 1
#define HALF_BAUD 1000/(2*BAUD_RATE)

#include "Temporizador.h"
 byte dataB;
 byte Bit_counter=10;
//int Ascii_to_Bin(int asciiInput, int* res)
//{
//  int rem, counter=0; 
//  res = (int*) malloc(1*sizeof(int));
//   
//  while (asciiInput > 0)
//  {
//    rem = asciiInput % 2;
//    res[counter] = rem;
//    asciiInput = asciiInput / 2;
//    res = (int*) realloc(res, (counter++)*sizeof(int));
//  }
//  return counter;
//}
//
//void remove_char(char* msg, int* sz){
//  for (int i =0;i<(*sz)-1;i++){
//    msg[i] = msg[i+1];
//  }
//  *sz--;
//}
//
//void invert_vector(int* v, int sz){
//  for (int i =0; i<sz/2;i++, sz--){
//    int aux = v[i];
//    v[i] = v[sz];
//    v[sz] = aux;
//  }
//}

// Calcula bit de paridade - Par ou impar
// int* bitParidade(char dado, int* siz){
//   int *dado_bin, counter=0;
//   int sz = Ascii_to_Bin(dado, dado_bin);
//   for (int i = sz; i>0;i--){
//     if (dado_bin[i] == 1){
//       counter++;
//     }
//   }
//   dado_bin = (int*)realloc(dado_bin, (sz++)*sizeof(int));
//   *siz = sz;
//   if (counter %2){
//     dado_bin[sz] = 1;
//     invert_vector(dado_bin, sz);
//     return dado_bin;
//   }
//   else{
//     dado_bin[sz] = 0;
//     invert_vector(dado_bin, sz);
//     return dado_bin;
//   }
// }
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
// O que fazer toda vez que 1s passou?
ISR(TIMER1_COMPA_vect){
  Serial.println("Entrei na ISR");
//  char dado = Serial.read();
//  if (dado <= '\n')
//    Bit_counter = -2;
//  else{
//    dataB = dado;
//  }
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
  Serial.println("Digite a msg");
  // Configura timer
  configuraTemporizador(BAUD_RATE);
  // habilita interrupcoes
  interrupts();
}

// O loop() eh executado continuamente (como um while(true))
void loop ( ) {
  //Three way handshake
 if (Bit_counter == 10){
//  digitalWrite(PINO_RTS, HIGH);
//  int resp = digitalRead(PINO_CTS);
//  while(resp == LOW){
//    digitalWrite(PINO_RTS, HIGH);
//    resp = digitalRead(PINO_CTS);
//  }
  while(!Serial.available());
  char dado = Serial.read();
  dataB = dado;
  Bit_counter = 7;
  iniciaTemporizador();
  }

  if (Bit_counter == 7){
    Serial.println(dataB, BIN);
//    Serial.println(dataB, BIN);
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
  if (Bit_counter >0){
  //deve ser um loop
    byte sign = bitRead(dataB, Bit_counter);
    if (sign == 1){
      Serial.print("1");
      digitalWrite(PINO_RX, HIGH);
    }
    else{
      Serial.print("0");
      digitalWrite(PINO_RX,LOW);
    }
    Bit_counter--;
  }
  else if (Bit_counter == 0){
      Bit_counter =7;
      Serial.println();
      paraTemporizador();
      char dado = Serial.read();
      Serial.println(dado, DEC);
      if (dado <= '\n'){
        Bit_counter = 10;
      }
      else{
        dataB = dado;
      }
    }
  else{
    digitalWrite(PINO_RTS, LOW);
    return;
   }
    

  //Transmissão do byte bit a bit
  //Fim da transmissão
  // int bin_size=0;
  // int* msg_bin = bitParidade(mensagem[sz], &bin_size);
  // for (int i = 0;i<bin_size;i++){
  //   if (msg_bin[i] == 1){
  //     digitalWrite(12, HIGH);
  //   }
  //   else{
  //     digitalWrite(12,LOW);
  //   }
  // }
  // remove_char(mensagem, &sz);
}
