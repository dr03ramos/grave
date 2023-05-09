#include <EEPROM.h>

#include <Servo.h>

#include <AFMotor.h>

// DECLARAÇÕES
// Número de passos por revolução do motor de passo
const int stepsPerRevolution = 200;

// Instância da biblioteca Stepper
AF_Stepper motor(stepsPerRevolution, 2);

// Pinos de conexao do modulo TCS230 de cores 
const int s0 = 22;
const int s1 = 24;
const int s2 = 26;
const int s3 = 28;
const int out = 30;

// Variaveis que armazenam o valor das cores  
int red = 0;  
int green = 0;  
int blue = 0;  

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
int velocidade_z_rpm = 350;

// Definição dos servo motores
Servo servomotorelo1;
Servo servomotorelo2;
const int pinoServoMotorElo1 = 10;
const int pinoServoMotorElo2 = 11;

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
    if (sentidoZ == 0) {
        motor.step(stepsPerRevolution * pulsos, FORWARD, MICROSTEP);
    }
    // sentido anti-horario
    else {
        motor.step(stepsPerRevolution * pulsos, BACKWARD, MICROSTEP);
    }
    delayMicroseconds(500);
}

  //Rotina que le o valor das cores  
void color()  
{  
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);  
  //count OUT, pRed, RED  
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
    Serial.println(String(digitalRead(s2)) + " | " + String(digitalRead(s3)) + " | " + String(red));

  digitalWrite(s3, HIGH);  
  //count OUT, pBLUE, BLUE  
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
    Serial.println(String(digitalRead(s2)) + " | " + String(digitalRead(s3)) + " | " + String(blue));
  digitalWrite(s2, HIGH);  
  //count OUT, pGreen, GREEN  
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
    Serial.println(String(digitalRead(s2)) + " | " + String(digitalRead(s3)) + " | " + String(green));
}

// Função que lê o sensor de cores, modulo TCS230
int detecta_cores() {
    int retorno = 0;
    Serial.println("Detectando cor...");
    while (retorno == 0 && /*digitalRead(IbarraO)*/ HIGH == HIGH) {
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
  if (red < blue && red < green && red < 100)  
  {  
   Serial.println("Vermelho");  
   retorno = 1;
  }  
  //Verifica se a cor azul foi detectada  
  else if (blue < red && blue < green)   
  {  
   Serial.println("Azul");  
   retorno = 2;
  }  
  //Verifica se a cor verde foi detectada  
  else if (green < red && green < blue)  
  {  
   Serial.println("Verde");  
   retorno = 0;
  }  
  Serial.println();  
  //Aguarda 2 segundos, apaga os leds e reinicia o processo  
  delay(2000);   
    }
    return retorno;
    Serial.println("Saindo da deteccao de cor...");
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
    Serial.println("Movendo garra para a posição: " + String(x) + ", " + String(y) + ", " + String(z));

    sistema_coordenadas(x, y);

    Serial.println("Ângulos definidos.");

    // nova posição do eixo Z
    novaPosZ = z;
    int quantPassosZ = sistema_z(abs(z - atualPosZ));

    if (novaPosZ < atualPosZ) {
        sentidoZ = 1;
        Serial.println("Desce.");
    } else if (novaPosZ > atualPosZ) {
        sentidoZ = 0;
        Serial.println("Sobe.");
    }

    // se a nova posição do eixo Z for maior
    if (novaPosZ > atualPosZ) {
        Serial.println("Movendo motor de passo.");
        move_motorpasso(abs(quantPassosZ));
    }
    Serial.println("Movendo motores servo grandes.");
    // movimentação eixo x e y
    if (novaAngX > atualAngX) {
        for (int i = atualAngX; i < novaAngX; i++) {
            servomotorelo1.write(i);
            delay(1000 / velocidade_xy);
        }
    } else if (novaAngX < atualAngX) {
        for (int i = atualAngX; i > novaAngX; i--) {
            servomotorelo1.write(i);
            delay(1000 / velocidade_xy);
        }
    }
    if (novaAngY > atualAngY) {
        for (int i = atualAngY; i < novaAngY; i++) {
            servomotorelo2.write(i);
            delay(1000 / velocidade_xy);
        }
    } else if (novaAngY < atualAngY) {
        for (int i = atualAngY; i > novaAngY; i--) {
            servomotorelo2.write(i);
            delay(1000 / velocidade_xy);
        }
    }

    // se a nova posição do eixo Z for menor
    if (novaPosZ < atualPosZ) {
        Serial.println("Movendo motor de passo.");
        move_motorpasso(abs(quantPassosZ));
    }
    Serial.println("Movimento concluído.");

    // Informa na serial em uma linha só
    Serial.print("X: de " + String(x) + " para " + String(novaAngX) + " | Y: de " + String(y) + " para " + String(novaAngY));
    Serial.println();

    // Atualiza os valores antigos
    atualAngX = novaAngX;
    atualAngY = novaAngY;
}

void garra(int abreoufecha) {
    if (abreoufecha == 1) {
        servomotorgarra.write(0);
        Serial.println("Garra aberta.");
    } else if (abreoufecha == 0) {
        servomotorgarra.write(180);
        Serial.println("Garra fechada.");
    }
}

void setup() {
    // Define os pinos como saída
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    pinMode(out, INPUT);

      digitalWrite(s0, HIGH);  
  digitalWrite(s1, LOW);  

    // Inicia a comunicação serial
    Serial.begin(9600);

    motor.setSpeed(velocidade_z_rpm); // RPM

    // Inicia os servos motores maiores
    servomotorelo1.attach(pinoServoMotorElo1);
    servomotorelo2.attach(pinoServoMotorElo2);
    servomotorgarra.attach(pinoServoMotorGarra);

    Serial.println("Setup concluido.");
}

void loop() {
    // funciona se o interruptor está ligado
    // se detecta azul ou vermelho, ele executa o motor
    // manda o motor pegar a peça na área do sensor e levar para um destino
    // os destinos são diferentes para as duas cores
    // sobe o motor e volta para o ponto inicial
    Serial.println("Loop iniciado.");
    // se o interruptor estiver ligado
    if ( /*digitalRead(IbarraO)*/ 1 == 1) {
        // se detectar azul ou vermelho
        int cor = detecta_cores();
        Serial.println("Detecção de cor: " + String(cor));
        if (cor != 0) {
            Serial.println("Uma cor foi detectada.");
            // abre a garra
            garra(0);
            // leva o braço até o ponto do sensor
            lidando_com_x_y(200, 0, 0);
            // fecha a garra
            garra(1);
            // leva o braço até o ponto de destino
            if (cor == 2) {
                lidando_com_x_y(0, 0, 0);
                Serial.println("Peça azul jogada na caixa.");
            } else if (cor == 1) {
                lidando_com_x_y(0, 200, 0);
                Serial.println("Peça vermelha jogada na caixa.");
            }
            // abre a garra
            garra(0);
            Serial.println("Ciclo concluído.");
            // sobe o braço
            lidando_com_x_y(100, 100, 200);
            Serial.println("Braço em um ponto neutro.");
        }
    }
}