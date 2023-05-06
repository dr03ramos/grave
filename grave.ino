#include <EEPROM.h>

int IN1 = 8;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;
int IbarraO = 12;

// Define o tempo de cada passo do motor
int tempo = 5;

// Define o objeto do temporizador
unsigned long contador;
bool sentido = 0;

// salva dados na EEPROM
void inicializa_leitura_memoria() {
    Serial.println("Fazendo leitura...");

    // faz leitura da eeprom
    // contador = EEPROM.read(0);

    contador = 50;

    Serial.print("Número do contador: ");
    Serial.println(contador);
}

void atualiza_leitura_memoria() {
    // salvar o valor do contador na posição zero
    int endereco_EEPROM = 0;
    int valor_EEPROM1 = contador;
    EEPROM.update(endereco_EEPROM, valor_EEPROM1);
}

void gira_motor(int velocidade) {
    if (velocidade > 0) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    }
    // Se a velocidade for zero, o motor para
    else if (velocidade == 0) {
        para_motor();
    }
    // Se a velocidade for negativa, o motor gira em outro sentido
    else if (velocidade < 0) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    }
}

void para_motor() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void informa_serial(int IbarraO, int contador, int sentido) {
    Serial.print("IbarraO: ");
    Serial.print(IbarraO);
    Serial.print(" | contador: ");
    Serial.print(contador);
    Serial.print(" | sentido: ");
    Serial.println(sentido);
    Serial.print(IN1);
    Serial.print(" | ");
    Serial.print(IN2);
    Serial.print(" | ");
    Serial.print(IN3);
    Serial.print(" | ");
    Serial.print(IN4);
    Serial.println();
}

void setup() {
    // Define os pinos como saída
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(IbarraO, INPUT);

    inicializa_leitura_memoria();

    // Inicia a comunicação serial
    Serial.begin(9600);
}

void loop() {
    int velocidade = 1;

    if (digitalRead(IbarraO) == HIGH) {
        gira_motor(velocidade);
    }
    else {
        para_motor();
    }

    // contador
    if (sentido == 0) {
        if (contador < 100) {
            contador++;
        }
        else {
            sentido = 1;
        }
        velocidade = 1;
    }
    else {
        if (contador > 0) {
            contador--;
        }
        else {
            sentido = 0;
        }
        velocidade = -1;
    }
    
    informa_serial(digitalRead(IbarraO), contador, sentido);
}
/*
//Pinos de conexao do modulo TCS230  
const int s0 = 8;
const int s1 = 9;
const int s2 = 12;
const int s3 = 11;
const int out = 10;

//Pinos dos leds  
int pinoledverm = 2;
int pinoledverd = 3;
int pinoledazul = 4;

//Variaveis que armazenam o valor das cores  
int red = 0;
int green = 0;
int blue = 0;

void setup() {
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    pinMode(out, INPUT);
    pinMode(pinoledverm, OUTPUT);
    pinMode(pinoledverd, OUTPUT);
    pinMode(pinoledazul, OUTPUT);
    Serial.begin(9600);
    digitalWrite(s0, HIGH);
    digitalWrite(s1, LOW);
}

void loop() {
    color(); //Chama a rotina que le as cores  
    //Mostra no serial monitor os valores detectados  
    Serial.print("Vermelho :");
    Serial.print(red, DEC);
    Serial.print(" Verde : ");
    Serial.print(green, DEC);
    Serial.print(" Azul : ");
    Serial.print(blue, DEC);
    Serial.println();
    //Verifica se a cor vermelha foi detectada  
    if (red < blue && red < green && red < 100) {
        Serial.println("Vermelho");
        digitalWrite(pinoledverm, HIGH); //Acende o led vermelho  
        digitalWrite(pinoledverd, LOW);
        digitalWrite(pinoledazul, LOW);
    }
    //Verifica se a cor azul foi detectada  
    else if (blue < red && blue < green) {
        Serial.println("Azul");
        digitalWrite(pinoledverm, LOW);
        digitalWrite(pinoledverd, LOW);
        digitalWrite(pinoledazul, HIGH); //Acende o led azul  
    }
    //Verifica se a cor verde foi detectada  
    else if (green < red && green < blue) {
        Serial.println("Verde");
        digitalWrite(pinoledverm, LOW);
        digitalWrite(pinoledverd, HIGH); //Acende o led verde  
        digitalWrite(pinoledazul, LOW);
    }
    Serial.println();
    //Aguarda 2 segundos, apaga os leds e reinicia o processo  
    delay(2000);
    digitalWrite(pinoledverm, LOW);
    digitalWrite(pinoledverd, LOW);
    digitalWrite(pinoledazul, LOW);
}

void color() {
    //Rotina que le o valor das cores  
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    //count OUT, pRed, RED  
    red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
    digitalWrite(s3, HIGH);
    //count OUT, pBLUE, BLUE  
    blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
    digitalWrite(s2, HIGH);
    //count OUT, pGreen, GREEN  
    green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
}*/