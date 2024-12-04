#include <SoftwareSerial.h>

SoftwareSerial ArduinoSlave(10, 11);
#define ledPin 2

String receivedData;
int brilho = 0;

void setup() {
    Serial.begin(9600);
    ArduinoSlave.begin(9600);
    pinMode(ledPin, OUTPUT);
}

int parseArray(String data, int arr[]) {
    int index = 0;
    int startIndex = 0;
    
    // Percorre a string e separa os valores pela vírgula
    while (true) {
        int commaIndex = data.indexOf(',', startIndex);
        if (commaIndex == -1) { // Último elemento (sem vírgula)
            arr[index++] = data.substring(startIndex).toInt();
            break;
        }
        arr[index++] = data.substring(startIndex, commaIndex).toInt();
        startIndex = commaIndex + 1;
    }
    
    return index; // Retorna o número de elementos no array
}


void loop() {
    //     ledVal = map(intVal, 0, 1023, 0, 255);
    //     Serial.print("led value: ");
    //     Serial.println(ledVal);

     if (ArduinoSlave.available() > 0) {
        receivedData = ArduinoSlave.readStringUntil('\n'); // Lê a string até o final da linha
        int intArray[4]; // Array para armazenar os valores recebidos
        int arraySize = parseArray(receivedData, intArray);

        brilho = map(intArray[0], 0, 1023, 0, 255);;
        if (intArray[1] == 1){
          analogWrite(ledPin, 255);
        }
        else{
          if(brilho < 20){
            Serial.println("Baixa Luminosidade!");
          }
          analogWrite(ledPin, brilho);
        }
        
        // Exibe o array no Serial Monitor
        Serial.print("Slave received array: ");
        for (int i = 0; i < arraySize; i++) {
            Serial.print(intArray[i]);
            Serial.print(" ");
        }
        Serial.println();
    }
}

// void readMasterPort() {
//     while (ArduinoMaster.available()) {
//         delay(10);
//         if (ArduinoMaster.available() > 0) {
//             char c = ArduinoMaster.read(); // gets one byte from serial buffer
//             msg += c; // appends the character to msg string
//         }
//     }
//     ArduinoMaster.flush();
// }

// void convertMsgToCmd() {
//     if (msg.length() > 0) {
//         Serial.print("message length: ");
//         Serial.println(msg.length());

//         char carray1[6]; // array to hold the message for conversion
//         msg.toCharArray(carray1, sizeof(carray1));
//         intVal = atoi(carray1);

//         msg = ""; // clear the message for the next command
//     }
// }
