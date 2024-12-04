#include <FS.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include "SPIFFS.h"

const char* ssid = "NPITI-IoT";  /* Substitua pelo nome da rede */
const char* password =  "NPITI-IoT"; 

NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000); 
String hora;   

int btPin = 27, ledPin = 14;
int btState;
int lastBtState = 0;
int cont = 0;
String path = "/logPratica5";

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
  String s;
  Serial.println("Read file");
  File rFile = SPIFFS.open(path, "r");//r+ leitura e escrita
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  else {
    Serial.print("----------Lendo arquivo ");
    Serial.print(path);
    Serial.println("  ---------");
    while (rFile.position() < rFile.size())
    {
      s = rFile.readStringUntil('\n');
      s.trim();
      Serial.println(s);
    }
    rFile.close();

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

  pinMode(btPin, INPUT);
  pinMode(ledPin, OUTPUT);


  WiFi.begin(ssid, password);
  /*---- Enquanto estiver conectando ---- */
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);                                 /* Aguarda meio segundo */
    Serial.println("Conectando ao WiFi..");     /* Conectando */
  }
  Serial.println("Conectado"); /* Conectado */

  ntp.begin();               /* Inicia o protocolo */
  ntp.forceUpdate();    /* Atualização */
  
  delay(200);
  //formatFile();
  openFS(); 
  
}

void loop() {
  JsonDocument doc = readFile("/logPratica5");
  String led_on = doc["led_on"]; 


  if(led_on == "1"){
    digitalWrite(ledPin, HIGH);
  }else{
    digitalWrite(ledPin, LOW);
  }

  btState = digitalRead(btPin);

  if(btState == 1 && lastBtState == 0){
    lastBtState = 1;
    cont++;
    if (led_on == "0")
      writeFile(path, "led_on", "1");
    else
      writeFile(path, "led_on", "0");
  }
  if(btState == 0 && lastBtState == 1){
    lastBtState = 0;
  }

  Serial.print("Contador: ");
  Serial.println(cont);
  Serial.print("Led: ");
  Serial.println(led_on);
  delay(300);
}







