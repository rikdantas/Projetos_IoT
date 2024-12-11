
#include <Arduino.h>
#include "sensor_ultrassonico.h"



// Velocidade do som

#define SOUND_SPEED 0.034

#define TIME_LIMIT 5000

#define DISTANCE_REFERENCE 10


float get_distancia(int porta_echo){

  // Calcular o tempo que o pino fica alto em ms

  long duracaoPulso = pulseIn(porta_echo, HIGH);


  // Calcular a distancia (cm)

  float distanciaCm = duracaoPulso * SOUND_SPEED / 2;


  return distanciaCm;

}


void enable_trigger(int porta_trigger){

  // Envio do sinal de trig

  digitalWrite(porta_trigger, LOW);

  delayMicroseconds(2);


  //Seta o pino Trig alto por 10ms

  digitalWrite(porta_trigger, HIGH);

  delayMicroseconds(10);

  digitalWrite(porta_trigger, LOW);

}


void person_in(int &qtd_pessoas, int trigPin, int echoPin){

  int tempo_inicial = millis();

  int tempo_atual = 0;

  float distancia = 0;

  while(tempo_atual < tempo_inicial + TIME_LIMIT){

    enable_trigger(trigPin);

    distancia = get_distancia(echoPin);

    if(distancia <= DISTANCE_REFERENCE){

      qtd_pessoas++;

      delay(500);

      break;

    }

    tempo_atual = millis();

  }

}


void person_out(int &qtd_pessoas, int trigPin, int echoPin){

  int tempo_inicial = millis();

  int tempo_atual = 0;

  float distancia = 0;

  while(tempo_atual < tempo_inicial + TIME_LIMIT){

    enable_trigger(trigPin);

    distancia = get_distancia(echoPin);

    if(distancia <= DISTANCE_REFERENCE){

      qtd_pessoas--;

      delay(500);

      break;

    }

    tempo_atual = millis();

  }

}
