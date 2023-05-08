#include <EEPROM.h>
#include <Servo.h>

// DECLARAÇÕES
// Pinos da ponte H motor de passo (TODO: substituir por driver)
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

// Pinos de conexao do modulo TCS230 de cores 
const int s0 = 22;
const int s1 = 23;
const int s2 = 24;
const int s3 = 25;

// Pino do interruptor
const int IbarraO = 7;

// Matriz de movimento x e y
int atualPosX = 180;
int atualPosY = 0;
int novaPosX = 0;
int novaPosY = 0;
int atualAngX = 90;
int atualAngY = 90;
int novaAngX = 0;
int novaAngY = 0;

int velocidade_xy = 1;

// Controle eixo Z
int atualPosZ = 0;
int novaPosZ = 0;

bool sentidoZ = 0;
int velocidade_z = 1;

const int pinoStep 32;
const int pinoDir 35;

// Definição dos servo motores
Servo servomotorelo1;
Servo servomotorelo2;
const int pinoServoMotorElo1 = 5;
const int pinoServoMotorElo2 = 6;

// Definição do servo motor da garra
Servo servomotorgarra;
const int pinoServoMotorGarra = 13;
int posicaoServoMotorGarra = 0;

// Define o tempo de cada passo do motor
int tempo = 5;

// Define o objeto do temporizador
unsigned long contador;

// FUNÇÕES
// salva dados na EEPROM (remover)
void inicializa_leitura_memoria() {
    Serial.println("Fazendo leitura...");

    // faz leitura da eeprom
    // contador = EEPROM.read(0);

    contador = 50;

    Serial.print("Número do contador: ");
    Serial.println(contador);
}

void atualiza_leitura_memoria() {
    // salvar o valor do contador na posição zero (remover)
    int endereco_EEPROM = 0;
    int valor_EEPROM1 = contador;
    EEPROM.update(endereco_EEPROM, valor_EEPROM1);
}

// Função para movimentar o motor de passo do eixo Z
// em um determinado número de pulsos
void move_motorpasso(int pulsos) {
    // motor de passo usando driver

    // sentido horario
    if sentidoZ == 0 {
        digitalWrite(pinoDir, LOW);
    }
    // sentido anti-horario
    else {
        digitalWrite(pinoDir, HIGH);
    }

    for (int x = 0; x < (pulsos * 4); x++) {
        digitalWrite(pinoStep, HIGH);
        delayMicroseconds(700 / velocidade_z);
        digitalWrite(pinoStep, LOW);
        delayMicroseconds(700 / velocidade_z);
    }

}

// Função que lê o sensor de cores, modulo TCS230
// Ainda não implementada
void sensor_cor() {
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
}

// Função que calcula o ângulo de cada motor
// com base na posição x e y
void sistema_coordenadas(float x, float y) {
    int L1 = 90; // Largura do primeiro braço
    int L2 = 90; // Largura do segundo braço

    double tempAngY = acos((sq(x) + sq(y) - sq(L1) - sq(L2)) / (2 * L1 * L2));
    if (x < 0 & y < 0) {
        tempAngY = (-1) * tempAngY;
    }

    double tempAngX = atan(x / y) - atan((L2 * sin(tempAngY)) / (L1 + L2 * cos(tempAngY)));

    tempAngY = (-1) * tempAngY * 180 / PI;
    tempAngX = tempAngX * 180 / PI;

    // Ajuste dos angulos dependendo em qual quadrante a ponta do x,y está
    if (x >= 0 & y >= 0) { // 1o quadrante
        tempAngX = 90 - tempAngX;
    }
    if (x < 0 & y > 0) { // 2o quadrante
        tempAngX = 90 - tempAngX;
    }
    if (x < 0 & y < 0) { // 3o quadrante
        tempAngX = 270 - tempAngX;
    }
    if (x > 0 & y < 0) { // 4o quadrante
        tempAngX = -90 - tempAngX;
    }
    if (x < 0 & y == 0) {
        tempAngX = 270 + tempAngX;
    }

    // manipulando os valores para o range do motor usando map
    tempAngX = map(tempAngX, -180, 180, /*inicio servo1:*/ 0, /*limite servo1:*/ 180);
    tempAngY = map(tempAngY, -180, 180, /*inicio servo2:*/ 0, /*limite servo2:*/ 180);

    novaAngX = round(tempAngX);
    novaAngY = round(tempAngY);
}

// Função que calcula a quantidade de passos
// que o motor de passo do eixo Z deve dar
int sistema_z(int diff_z) {
    float passos_mm = 12; // passos que o servo tem que dar por milimetro

    // determinar quantidade de passos
    return diff_z * passos_mm;
}

// Função para movimentar todos os eixos para uma
// determinada posição x, y e z tendo como base a posição atual (antiga)
void lidando_com_x_y(int x, int y, int z) {
    sistema_coordenadas(x, y);

    // nova posição do eixo Z
    novaPosZ = z;
    int quantPassosZ = sistema_z(ABS(z - atualPosZ));

    if (novaPosZ < atualPosZ) {
        sentidoZ = 1;
    } else if (novaPosZ > atualPosZ) {
        sentidoZ = 0;
    }

    // se a nova posição do eixo Z for maior
    if (novaPosZ > atualPosZ) {
        move_motorpasso(ABS(quantPassosZ));
    }

    // movimentação eixo x e y
    if (novaAngX > antigaAngX) {
        for (int i = antigaAngX; i < novaAngX; i++) {
            servomotorelo
            1. write(i);
            delay(1000 / velocidade_xy);
        }
    } else if (novaAngX < antigaAngX) {
        for (int i = antigaAngX; i > novaAngX; i--) {
            servomotorelo
            1. write(i);
            delay(1000 / velocidade_xy);
        }
    }
    if (novaAngY > antigaAngY) {
        for (int i = antigaAngY; i < novaAngY; i++) {
            servomotorelo2.write(i);
            delay(1000 / velocidade_xy);
        }
    } else if (novaAngY < antigaAngY) {
        for (int i = antigaAngY; i > novaAngY; i--) {
            servomotorelo2.write(i);
            delay(1000 / velocidade_xy);
        }
    }

    // se a nova posição do eixo Z for menor
    if (novaPosZ < atualPosZ) {
        move_motorpasso(ABS(quantPassosZ));
    }

    // Informa na serial em uma linha só
    Serial.print("X: de " + String(x) + " para " + String(novaAngX) + " | Y: de " + String(y) + " para " + String(novaAngY));
    Serial.println();

    // Atualiza os valores antigos
    antigaAngX = novaAngX;
    antigaAngY = novaAngY;
}

void setup() {
    // Define os pinos como saída
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(IbarraO, INPUT);

    inicializa_leitura_memoria(); // ideia: não usar motor de passo, definir o ponto zero manualmente antes de ligar a máquina

    // Inicia a comunicação serial
    Serial.begin(9600);

    // Inicia os servos motores maiores
    servomotorelo1.attach(pinoServoMotorElo1);
    servomotorelo2.attach(pinoServoMotorElo2);
    servomotorgarra.attach(pinoServoMotorGarra);
}

void loop() {
    if (digitalRead(IbarraO) == HIGH) {
        move_motorpasso();
        sentido = 1;
    } else {
        sentido = 0;
    }

    informa_serial(digitalRead(IbarraO), contador, sentido, velocidade);
    delay(300);
}

// Função para cuspir informações na serial
void informa_serial(int IbarraO, int contador, int sentido, int velocidade) {
    Serial.print("IbarraO: ");
    Serial.print(IbarraO);
    Serial.print(" | contador: ");
    Serial.print(contador);
    Serial.print(" | sentido: ");
    Serial.print(sentido);
    Serial.print(" | velocidade: ");
    Serial.println(velocidade);
    Serial.print(digitalRead(IN1));
    Serial.print(" | ");
    Serial.print(digitalRead(IN2));
    Serial.print(" | ");
    Serial.print(digitalRead(IN3));
    Serial.print(" | ");
    Serial.print(digitalRead(IN4));
    Serial.println();
}