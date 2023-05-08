// https://howtomechatronics.com/projects/scara-robot-how-to-build-your-own-arduino-based-robot/

int theta1 = 0; // Base angle
int theta2 = 0; // Second arm angle
int newIntTheta1 = 0; // Base angle
int newIntTheta2 = 0; // Second arm angle

void inverseKinematics(float x, float y) {
    int L1 = 90; // Length of the first arm
    int L2 = 90; // Length of the second arm
  double newTheta2 = acos((sq(x) + sq(y) - sq(L1) - sq(L2)) / (2 * L1 * L2));
  if (x < 0 & y < 0) {
    newTheta2 = (-1) * newTheta2;
  }
  
  double newTheta1 = atan(x / y) - atan((L2 * sin(newTheta2)) / (L1 + L2 * cos(newTheta2)));
  
  newTheta2 = (-1) * newTheta2 * 180 / PI;
  newTheta1 = newTheta1 * 180 / PI;

 // Angles adjustment depending in which quadrant the final tool coordinate x,y is
  if (x >= 0 & y >= 0) {       // 1st quadrant
    newTheta1 = 90 - newTheta1;
  }
  if (x < 0 & y > 0) {       // 2nd quadrant
    newTheta1 = 90 - newTheta1;
  }
  if (x < 0 & y < 0) {       // 3d quadrant
    newTheta1 = 270 - newTheta1;
  }
  if (x > 0 & y < 0) {       // 4th quadrant
    newTheta1 = -90 - newTheta1;
  }
  if (x < 0 & y == 0) {
    newTheta1 = 270 + newTheta1;
  }

  newIntTheta1=round(newTheta1);
  newIntTheta2=round(newTheta2);
  
    
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  // mostrar valor de theta1 y theta2
  // solicitar novas coordenadas x,y
  // calcular theta1 e theta2
  // mostrar
  // repetir

  int x = 0;
  int y = 0;

  Serial.print("Digite a coordenada x: ");
  while (Serial.available() == 0) {}
  x = Serial.parseInt();
  Serial.println(x);
  Serial.print("Digite a coordenada y: ");
  while (Serial.available() == 0) {}
  y = Serial.parseInt();
  Serial.println(y);

  inverseKinematics(x, y);

  Serial.print("Theta1: ");
  Serial.println(newIntTheta1);
  Serial.print("Theta2: ");
  Serial.println(newIntTheta2);
  delay(1000);

  // mostrar diferença entre thetas
  Serial.print("Diferença theta1: ");
  Serial.println(newIntTheta1 - theta1);
  Serial.print("Diferença theta2: ");
  Serial.println(newIntTheta2 - theta2);

  theta1 = newIntTheta1;
  theta2 = newIntTheta2;
}