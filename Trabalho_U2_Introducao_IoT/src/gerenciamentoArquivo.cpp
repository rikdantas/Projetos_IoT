#include <FS.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"

#include "gerenciamentoArquivo.h"



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
  Serial.println("Lendo arquivo...");
  
  File rFile = SPIFFS.open(path, "r");  // Abre o arquivo para leitura
  if (!rFile) {
    Serial.println("Erro ao abrir o arquivo!");
    return JsonDocument();  // Retorna um JsonArray vazio em caso de erro
  }
  
  while (rFile.available()) {
    s += rFile.readString();  // Lê o conteúdo do arquivo e armazena na string
    //Serial.println(s);
  }
  rFile.close();  // Fecha o arquivo

  // Cria o documento JSON para deserializar
  JsonDocument doc;  // Cria um documento JSON com tamanho adequado
  DeserializationError error = deserializeJson(doc, s);  // Deserializa o conteúdo JSON

  if (error) {
    Serial.print("Erro de deserialização: ");
    Serial.println(error.f_str());
    return JsonArray();  // Retorna um JsonArray vazio em caso de erro
  }

  // Converte o documento para um JsonArray e retorna
  return doc;
}

// JsonDocument readFile(String path) {
//   String s = "";
//   Serial.println("Read file");
//   File rFile = SPIFFS.open(path, "r");//r+ leitura e escrita
//   if (!rFile) {
//     Serial.println("Erro ao abrir arquivo!");
//   }
//   else {
//     Serial.print("----------Lendo arquivo ");
//     Serial.print(path);
//     Serial.println("  ---------");
//     while (rFile.available()) {
//       s += rFile.readString(); // Acrescenta o conteúdo do arquivo à string
//     }
//     rFile.close(); // Fecha o arquivo

//     JsonDocument doc;
//     deserializeJson(doc,s);

//     return doc;
//   }
// }

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



