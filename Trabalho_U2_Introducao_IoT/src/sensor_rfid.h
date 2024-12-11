#include <MFRC522.h>


MFRC522 inicializar_sensor_rfid(int SS_PIN, int RST_PIN);
String ler_cartao(MFRC522 rfid);
void printHex(byte *buffer, byte bufferSize);
void printDec(byte *buffer, byte bufferSize);
String hex_2_str(byte *buffer, byte bufferSize);