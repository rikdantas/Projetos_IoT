
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#include "sensor_rfid.h"
#include "sensor_ultrassonico.h"
#include "gerenciamentoArquivo.h"

#define SS_PIN 5
#define RST_PIN 2
#define DHTPIN 27
#define TIME_MQTT_REFERENCE 10000
#define DISTANCE_REFERENCE 10
#define DHTTYPE DHT22
#define MAX_CADASTROS 10
#define IO_USERNAME  "ADAFRUIT_USERNAME" //<----------Mudar para o seu
#define IO_KEY       "ADAFRUIT_KEY" //<----------Mudar para o seu

const char* mqttserver = "io.adafruit.com";
const int mqttport = 1883;
const char* mqttUser = IO_USERNAME;
const char* mqttPassword = IO_KEY;

DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);
 
MFRC522 rfid; 
String id_cartao = "";

int trigPin_1 = 32;

int echoPin_1 = 33;


int trigPin_2 = 26;

int echoPin_2 = 25;


float distancia_1;

float distancia_2;

int qtd_pessoas = 0;
long tempo_inicial  = 0;

String path_cadastros = "/cadastros.txt";
String path_config_wifi = "/config_wifi.txt";

JsonDocument cadastros, config_wifi;
int qtd_cadastros = 0;

bool acesso_liberado = false;


void setup() { 
  Serial.begin(115200);

  dht.begin();

  openFS();

  rfid = inicializar_sensor_rfid(SS_PIN, RST_PIN);
  pinMode(trigPin_1, OUTPUT); //Sensor ultrassonico 01
  pinMode(echoPin_1, INPUT); //Sensor ultrassonico 01

  pinMode(trigPin_2, OUTPUT); //Sensor ultrassonico 02
  pinMode(echoPin_2, INPUT); //Sensor ultrassonico 02

  cadastros = readFile(path_cadastros);

  // ------------- Configuração e Conecção Wifi (START) ---------------- 
  config_wifi = readFile(path_config_wifi);
  const char* ssid = config_wifi["ssid"] ;
  const char* password =  config_wifi["password"]; 

  WiFi.begin(ssid, password);
  /*---- Enquanto estiver conectando ---- */
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);                                 /* Aguarda meio segundo */
    Serial.println("Conectando ao WiFi..");     /* Conectando */
  }
  Serial.println("Conectado"); /* Conectado */
  // ------------- Configuração e Conecção Wifi (END) ---------------- 

  client.setServer(mqttserver, 1883); // Publicar


}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    // Create a random client ID
    String clientId = "ESP32 - Sensores";
    clientId += String(random(0xffff), HEX);
    // Se conectado
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("conectado");
      // Depois de conectado, publique um anúncio ...
      client.publish("viniciuspaiva/feeds/temperatura", "Iniciando Comunicação");
      client.publish("viniciuspaiva/feeds/qtd_pessoas", "Iniciando Comunicação");
      //... e subscribe.
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5s");
      delay(5000);
    }
  }
}
 
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // ------------------------- Liberacao de Entrada (START) -------------------------
  while (!acesso_liberado)
  {
    id_cartao = ler_cartao(rfid);
    if(id_cartao != ""){
      for(int i = 0; i < cadastros.size(); i++){
        JsonDocument item = cadastros[i]; 
        String id_atual = item["id_cartao"];
        String nome = item["nome"];
        if (id_atual == id_cartao){
          Serial.print("Ajustando a sala para o professor: ");
          Serial.println(nome);
          acesso_liberado = true;
          tempo_inicial = millis();
          break;
        }
      }
      if(!acesso_liberado)
        Serial.println("Esse cartão não está cadastrado \n Por Favor utilize um cartão cadastrado");
    }
  }
  // ------------------------- Liberacao de Entrada (END) -------------------------

  
  // ------------------------- Sensor Ultrassonico (START) -------------------------
    // ---- Sensor 1
  enable_trigger(trigPin_1);

  distancia_1 = get_distancia(echoPin_1);

  if(distancia_1 <= DISTANCE_REFERENCE){

    person_in(qtd_pessoas, trigPin_2, echoPin_2); // O sensor 1 olha para o 2

  }
    // ---- Sensor 2
  enable_trigger(trigPin_2);

  distancia_2 = get_distancia(echoPin_2);

  if(distancia_2 <= DISTANCE_REFERENCE){

    person_out(qtd_pessoas, trigPin_1, echoPin_1); // O sensor 2 olha para o 1

  }

  // ------------------------- Sensor Ultrassonico (END) -------------------------

  delay(300);
  Serial.print("Quantidade de Pessoas na Sala: ");

  Serial.println(qtd_pessoas);

  Serial.print("Distancia_1: ");

  Serial.println(distancia_1);

  Serial.print("Distancia_2: ");

  Serial.println(distancia_2);

  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperatura = dht.readTemperature();
  if (isnan(humidity) || isnan(temperatura)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(F("Humidity: "));
  Serial.println(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.println(temperatura);


  if (millis() > tempo_inicial + TIME_MQTT_REFERENCE){
    Serial.println("Enviando dados ao AdaFruit");
    char s_temp[8];
    dtostrf(temperatura,1,2,s_temp);
    client.publish("viniciuspaiva/feeds/temperatura", s_temp);

    char s_humidity[8];
    dtostrf(humidity,1,2,s_humidity);
    client.publish("viniciuspaiva/feeds/humidity", s_humidity);

    char s_qtd_pessoas[5];
    dtostrf(qtd_pessoas,1,2,s_qtd_pessoas);
    client.publish("viniciuspaiva/feeds/qtd_pessoas", s_qtd_pessoas);

    tempo_inicial = millis();
  }
  
}

