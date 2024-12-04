#include <SoftwareSerial.h>

SoftwareSerial ArduinoSlave(10, 11);
String answer;
String msg;
int intVal = 0, oldIntVal = 0, btState = 0, lastBtState = 0, btPin = 8;

int intArray[] = {0,0};

void setup() {
    Serial.begin(9600);
    ArduinoSlave.begin(9600);
}

void sendArray(int arr[], int size) {
    String message = "";
    for (int i = 0; i < size; i++) {
        message += String(arr[i]);
        if (i < size - 1) message += ","; // Adiciona uma vírgula entre os valores
    }
    
    ArduinoSlave.println(message); // Envia a string formatada
    Serial.print("Master sent: ");
    Serial.println(message);
}



void loop() {

    // #####  REFERENTE AO SENSOR ##### 
    intVal = analogRead(A0);
    Serial.print("VALOR ANALOG: ");
    Serial.println(intVal);

    intArray[0] = intVal;

    // #####  REFERENTE AO BOTÃO ##### 
    btState = digitalRead(btPin);
    if(btState == 1 && lastBtState == 0){ // Botão Pressionado
      lastBtState = 1;
    }
    if(btState == 0 && lastBtState == 1){ //Botão Não pressionado
      lastBtState = 0;
    }

    intArray[1] = btState;

    // #####  REFERENTE AO ENVIO DOS DADOS ##### 
    // Envia dados para o slave se o valor tiver mudado
    if (oldIntVal != intVal || lastBtState != btState) {
        sendArray(intArray, sizeof(intArray) / sizeof(intArray[0]));
    }

     delay(1000);
}
