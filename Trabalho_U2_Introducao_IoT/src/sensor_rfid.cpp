
#include "sensor_rfid.h"
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>



MFRC522 inicializar_sensor_rfid(int SS_PIN, int RST_PIN){
    MFRC522::MIFARE_Key key; 
    MFRC522 rfid(SS_PIN, RST_PIN);

    SPI.begin(); // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

  return rfid;
}

String ler_cartao(MFRC522 rfid){

    if ( ! rfid.PICC_IsNewCardPresent())
    return "";

    // Verify if the NUID has been readed
    if ( ! rfid.PICC_ReadCardSerial())
        return "";

    String id_cartao = "";

    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    Serial.println(rfid.PICC_GetTypeName(piccType));

    // Check is the PICC of Classic MIFARE type
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && 
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("Your tag is not of type MIFARE Classic."));
        return "";
    }

    
    Serial.println(F("A new card has been detected."));

    
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    id_cartao = hex_2_str(rfid.uid.uidByte, rfid.uid.size);
    
    

    // Halt PICC
    rfid.PICC_HaltA();

    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();

    return id_cartao;


}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
String hex_2_str(byte *buffer, byte bufferSize){
    String hexResult = "";  // String para armazenar o resultado

    for (byte i = 0; i < bufferSize; i++) {
      // Adiciona um espaço extra se o número for menor que 0x10
      if (buffer[i] < 0x10) {
        hexResult += " 0";
      } else {
        hexResult += " ";
      }

      // Converte o byte para hexadecimal e adiciona à String
      hexResult += String(buffer[i], HEX);
    }

    return hexResult;  // Retorna a string com o resultado
}


void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
