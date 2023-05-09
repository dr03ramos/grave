#include <EEPROM.h>
#include <Servo.h>
#include <Stepper.h>

// DECLARAÇÕES
// Número de passos por revolução do motor de passo
const int stepsPerRevolution = 200;

// Instância da biblioteca Stepper
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

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
int velocidade_z_rpm = 60;

// Definição dos servo motores
Servo servomotorelo1;
Servo servomotorelo2;
const int pinoServoMotorElo1 = 5;
const int pinoServoMotorElo2 = 6;

// Definição do servo motor da garra
Servo servomotorgarra;
const int pinoServoMotorGarra = 13;
int posicaoServoMotorGarra = 0;


// FUNÇÕES
// Função para movimentar o motor de passo do eixo Z
// em um determinado número de pulsos
void move_motorpasso(int pulsos) {
    // motor de passo usando driver

    // sentido horario
    if sentidoZ == 0 {
        myStepper.step(stepsPerRevolution*pulsos);
    }
    // sentido anti-horario
    else {
        myStepper.step(stepsPerRevolution*pulsos*-1);
    }
    delayMicroseconds(500);
}

// Função que lê o sensor de cores, modulo TCS230
// Ainda não implementada
void detectacores() {
    //Rotina que le o valor das cores  
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    //count OUT, pRed, RED  
    red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
    digitalWrite(s2, LOW);
    digitalWrite(s3, HIGH);
    //count OUT, pBLUE, BLUE  
    blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

    char retorno = "x";
    while (retorno != "y" && digitalRead(IbarraO) == HIGH) {
        //Verifica se a cor vermelha foi detectada  
        if (red < blue && red < 100) {
            return "a";
        }
        //Verifica se a cor azul foi detectada  
        else if (blue < red && blue < green) {
            return "v";
        }
    }
}

// Função que calcula o ângulo de cada motor
// com base na posição x e y
void sistema_coordenadas(float x, float y) {
    int L1 = 120; // Largura do primeiro braço
    int L2 = 150; // Largura do segundo braço

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

void garra(int abreoufecha) {
    if (abreoufecha == 1) {
        servomotorgarra.write(0);
    } else if (abreoufecha == 0) {
        servomotorgarra.write(180);
    }
}

void setup() {
    // Define os pinos como saída
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(IbarraO, INPUT);

    // Inicia a comunicação serial
    Serial.begin(9600);

    myStepper.setSpeed(velocidade_z_rpm); // RPM

    // Inicia os servos motores maiores
    servomotorelo1.attach(pinoServoMotorElo1);
    servomotorelo2.attach(pinoServoMotorElo2);
    servomotorgarra.attach(pinoServoMotorGarra);
}

void loop() {
    // funciona se o interruptor está ligado
    // se detecta azul ou vermelho, ele executa o motor
    // manda o motor pegar a peça na área do sensor e levar para um destino
    // os destinos são diferentes para as duas cores
    // sobe o motor e volta para o ponto inicial

    // se o interruptor estiver ligado
    if (digitalRead(IbarraO) == 1) {
        // se detectar azul ou vermelho
        if detectacores() == 'a' || detectacores() == 'v' {
            // abre a garra
            garra(0);
            // leva o braço até o ponto do sensor
            lidando_com_x_y(200, 0, 0);
            // fecha a garra
            garra(1);
            // leva o braço até o ponto de destino
            if (detectacores() == 'a') {
                lidando_com_x_y(0, 0, 0);
            } else if (detectacores() == 'v') {
                lidando_com_x_y(0, 200, 0);
            }
            // abre a garra
            garra(0);
            // sobe o braço
            lidando_com_x_y(100, 100, 200);

        }
    }        
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