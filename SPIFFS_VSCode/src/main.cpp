#include <FS.h>
#include <WiFi.h>
#include <ArduinoJson.h>
//#include <NTPClient.h>
#include "SPIFFS.h"

const char* ssid = "NPITI-IoT";  /* Substitua pelo nome da rede */
const char* password =  "NPITI-IoT"; 


// NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000); 
// String hora;   

int btPin = 27, ledPin = 14;
int btState;
int lastBtState = 0;
int cont = 0;
String path_teste = "/teste.txt";

JsonDocument readFile(String path);
void writeFile(String path, String field, String value);
void formatFile();
void openFS(void);

void writeFile(String path, String field, String value) { //escreve conteúdo em um arquivo
  File rFile = SPIFFS.open(path, "w");//a para anexar
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  else {
    
    JsonDocument doc = readFile(path);
    doc[field] = value;

    String save; 
    serializeJson(doc, save);

    Serial.print("tamanho");
    Serial.println(rFile.size());
    rFile.println(save);
    Serial.print("Gravou: ");
    Serial.println(save);
  }
  rFile.close();
}

JsonDocument readFile(String path) {
  String s = "";
  Serial.println("Read file");
  File rFile = SPIFFS.open(path, "r");//r+ leitura e escrita
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  else {
    Serial.print("----------Lendo arquivo ");
    Serial.print(path);
    Serial.println("  ---------");
    while (rFile.available()) {
      s += rFile.readString(); // Acrescenta o conteúdo do arquivo à string
    }
    rFile.close(); // Fecha o arquivo

    JsonDocument doc;
    deserializeJson(doc,s);

    return doc;
  }
}

void formatFile() {
  Serial.println("Formantando SPIFFS");
  SPIFFS.format();
  Serial.println("Formatou SPIFFS");
}

void openFS(void) {
  if (!SPIFFS.begin()) {
    Serial.println("\nErro ao abrir o sistema de arquivos");
  }
  else {
    Serial.println("\nSistema de arquivos aberto com sucesso!");
  }
}

void setup() {
  Serial.begin(115200);
  openFS();

  pinMode(btPin, INPUT);
  pinMode(ledPin, OUTPUT);

  JsonDocument config = readFile(path_teste);

   const char* ssid = config["ssid"] ;
   const char* password =  config["password"]; 

  WiFi.begin(ssid, password);
  /*---- Enquanto estiver conectando ---- */
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);                                 /* Aguarda meio segundo */
    Serial.println("Conectando ao WiFi..");     /* Conectando */
  }
  Serial.println("Conectado"); /* Conectado */

  // ntp.begin();               /* Inicia o protocolo */
  // ntp.forceUpdate();    /* Atualização */
  
  delay(200);
  //formatFile();
   
  
}
int c = 0;


void loop() {

  // JsonDocument config = readFile(path_teste);

  // const char* ssid = config["ssid"] ;
  // const char* password =  config["password"]; 
  
  // Serial.println(ssid);
  // Serial.println(password);
  
  delay(10000);
}







