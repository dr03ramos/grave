//Programa: Ponte H L298N com motor de passo
//Autor: Arduino e Cia
//Conexoes modulo - Arduino
int IN1 = 8;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;
int I/O = 12;

// Define o tempo de cada passo do motor
int tempo = 5;

// Define o objeto do temporizador
unsigned long contador;

// salva dados na EEPROM
void inicializa_leitura_memoria(){
    Serial.println("Fazendo leitura...");

    // faz leitura da eeprom
    contador = EEPROM.read(0);

    Serial.print("Número do contador: ");
    Serial.println(contador);
}
void atualiza_leitura_memoria(){
    // salvar o valor do contador na posição zero
    endereco_EEPROM = 0;
    int valor_EEPROM1 = contador;
    EEPROM.update(endereco_EEPROM, valor_EEPROM1);
}
void gira_descer(){
  // Faz o motor girar em passos
  // Passo 1
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 1);
  delay(tempo);  
  // Passo 2
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 1);
  delay(tempo);
  // Passo 3
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
  delay(tempo);
  // Passo 4
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
  delay(tempo);
  contador--;
}
void gira_descer(){
  // Faz o motor girar em passos
    // Passo 4
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
  delay(tempo);
  // Passo 3
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
  delay(tempo);
  // Passo 2
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 1);
  delay(tempo);
  // Passo 1
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 1);
  delay(tempo);  
  contador++;
}
void setup()
{
  // Define os pinos como saída
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(I/O, INPUT);
}

void loop(){
  if (digitalRead(I/O, HIGH)){
    contador = 1000;
    if (contador == 0) {
      // Para o motor
      digitalWrite(IN1, 0);
      digitalWrite(IN2, 0);
      digitalWrite(IN3, 0);
      digitalWrite(IN4, 0);
    } 
    else {
      gira_descer();
    }
  }
  else{
    contador = 0;
    if (contador >= 1000) {
    // Para o motor
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);
    } 
    else {
    gira_subir();
    }
  }
}