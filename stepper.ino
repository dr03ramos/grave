//Programa: Ponte H L298N com motor de passo
//Autor: Arduino e Cia
//Conexoes modulo - Arduino
int IN1 = 8;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;

// Define o tempo de cada passo do motor
int tempo = 50;

// Define o objeto do temporizador
unsigned long contador;

void setup()
{
  // Define os pinos como saída
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Define o tempo de início do temporizador
  contador = 0;
}

void loop()
{
  // Verifica se o tempo de 5 segundos já passou
  if (contador >= 100000) {
    // Para o motor
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);
  } else {
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
}