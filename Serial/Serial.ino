
#define PINO_RX 13
#define PINO_TX 12
#define BAUD_RATE 1
#define HALF_BAUD 1000/(2*BAUD_RATE)

#include "Temporizador.h"

int Ascii_to_Bin(int asciiInput, int* res)
{
  int rem, counter=0; 
  res = (int*) malloc(1*sizeof(int))
   
  while (asciiInput > 0)
  {
    rem = asciiInput % 2;
    res[counter] = rem;
    asciiInput = asciiInput / 2;
    res = (int*) realloc(res, (counter++)*sizeof(int));
  }
  return counter;
}

void remove_char(char* msg, int* sz){
  for (int i =0;i<(*sz)-1;i++){
    msg[i] = msg[i+1];
  }
  *sz--;
}

void invert_vector(int* v, int sz){
  for (int i =0; i<sz/2;i++, sz--){
    int aux = v[i];
    v[i] = v[sz];
    v[sz] = aux;
  }
}

// Calcula bit de paridade - Par ou impar
int* bitParidade(char dado, int* siz){
  int *dado_bin, counter=0;
  sz = Ascii_to_Bin(dado, dado_bin);
  for (int i = sz; i>0;i--){
    if (dado_bin[i] == 1){
      counter++;
    }
  }
  dado_bin = (int*)realloc(dado_bin, (sz++)*sizeof(int));
  *siz = sz;
  if (counter %2)
    dado_bin[sz] = 1;
    invert_vector(dado_bin, sz)
    return = dado_bin;
  else{
    dado_bin[sz] = 0;
    invert_vector(dado_bin, sz)
    return = dado_bin;
  }
}

// Rotina de interrupcao do timer1
// O que fazer toda vez que 1s passou?
ISR(TIMER1_COMPA_vect){
  //>>>> Codigo Aqui <<<<
}

// Executada uma vez quando o Arduino reseta
void setup(){
  //desabilita interrupcoes
  noInterrupts();
  // Configura porta serial (Serial Monitor - Ctrl + Shift + M)
  Serial.begin(9600);
  // Inicializa TX ou RX
  pinMode(13, INPUT)
  pinMode(12, OUTPUT)
  
  // Configura timer
  configuraTemporizador(BAUD_RATE)
  // habilita interrupcoes
  interrupts();
  int sz = 15;

  char* mesagem = malloc(sz*sizeof(char);
  mensagem = "Redes eh legal";
}

// O loop() eh executado continuamente (como um while(true))
void loop ( ) {
//  if (sz <0) interrupt()
  digitalWrite(12, HIGH)
  int resp = digitalRead(13)
  while(resp == LOW){
    digitalWrite(12, HIGH)
    resp = digitalRead(13)
  }
  int bin_size=0;
  char msg_bin = bitParidade(mensagem[sz], &bin_size)
  for (int i = 0;i<bin_size;i++){
    if (msg_bin[i] == 1){
      digitalWrite(12, HIGH);
    }
    else{
      digitalWrite(12,LOW);
    }
  }
  remove_char(mensagem, &sz)
}
