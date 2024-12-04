int led = 18;
int led_j1 = 27;
int led_j2 = 14;
int placar_j1 = 0;
int placar_j2 = 0;

#define jog1 15
#define jog2 13
#define starter 4

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP - Touch");
  pinMode(led, OUTPUT);
  pinMode(led_j1, OUTPUT);
  pinMode(led_j2, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (touchRead(starter) < 30){
    Serial.println("####### Iniciando novo round #######");

    for (int i = 0; i<3; i++){
      digitalWrite(led, HIGH);
      delay(1000);
      digitalWrite(led, LOW);
      delay(1000);
    }
    
    unsigned long start_time = millis();
    unsigned long reaction_time = 0;
    digitalWrite(led, 1);

    while (true){
      if(touchRead(jog1) < 30){
        reaction_time = millis() - start_time;
        digitalWrite(led_j1, 1);
        Serial.println("Jogador 1 ganhou");
        placar_j1++;
        delay(2000);
        break;
      }
      else if(touchRead(jog2) < 30){
        reaction_time = millis() - start_time;
        digitalWrite(led_j2, 1);
        Serial.println("Jogador 2 ganhou!");
        placar_j2++;
        delay(2000);
        break;
      }

    }

    digitalWrite(led_j1, 0);
    digitalWrite(led_j2, 0);
    digitalWrite(led, 0);

    Serial.print("\t Tempo de reação (ms): ");
    Serial.println(reaction_time);
    Serial.println("#### Placar ####");
    Serial.print("Jogador 01: ");
    Serial.println(placar_j1);
    Serial.print("Jogador 02: ");
    Serial.println(placar_j2);

    delay(1000);
  }
}
