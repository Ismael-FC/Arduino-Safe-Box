#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

#define COL 16
#define LIN 2
#define ende 0x27

//PINOS
int pin_buzzer = 13;
int trigPin = 10;
int echoPin = 12;
int portaPin = 26;

const int motorPin = 34;
const int pin_vib = A5;

//Variaveis
int i = 0;  // contador
int session = 0;
short int tentativas;
char leitura_teclas;
char guarda_cod[10], print_LCD[10];
String guarda_codString, print_LCDString;  // codigo de 8 digitos
int pos; // posicao do servo
int vibracao = 1000;

//Keypad
const byte matriz_col = 4;                              //numero de colunas da matriz de butoes
const byte matriz_lin = 4;                              //numero de linhas da matriz de butoes
const byte Pinos_LINHAS[matriz_lin] = { 9, 8, 7, 6 };   //pins onde ligam as linhas da matriz
const byte Pinos_COLUNAS[matriz_col] = { 5, 4, 3, 2 };  //pins onde ligam as colunas da matriz
const char Teclas_matriz[matriz_lin][matriz_col] = { { '1', '2', '3', 'A' },  //definicao da matriz de chars para associar a matriz de butoes
                                                     { '4', '5', '6', 'B' },
                                                     { '7', '8', '9', 'C' },
                                                     { '*', '0', '#', 'S' }};

Keypad teclado = Keypad(makeKeymap(Teclas_matriz), Pinos_LINHAS, Pinos_COLUNAS, matriz_lin, matriz_col);  //definicao da matriz de butoes

//LCD
LiquidCrystal_I2C lcd(ende, COL, LIN);

//Motor
Servo motor; 

//Funcoes
void guarda_codigo(String save, char leitura);
void abre_porta(String save, String compare, Servo motor, char leitura);
void lock();
double dist();

void setup() {
  Serial.begin(9600);
  motor.attach(motorPin); //ASSOCIAÇÃO DO PINO DIGITAL AO OBJETO DO TIPO SERVO
  motor.write(0);         //INICIA O MOTOR NA POSIÇÃO 0º
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(portaPin, INPUT);
  lcd.init();
  lcd.setBacklight(LOW);
}

void loop() {

  while(dist()>10){
    delay(100);
     vibracao = analogRead(pin_vib);
     Serial.println(vibracao);
    if (vibracao < 400){
      lock();
    }
    loop();
  }

  session += 1;
  lcd.backlight();

  if(session == 1){
    lcd.print("PRIMA A PARA");
    lcd.setCursor(0, 1);
    lcd.print("DEFINIR CODIGO");

    leitura_teclas = NULL;
    while (leitura_teclas == NULL || leitura_teclas != 'A' ){
      leitura_teclas = teclado.getKey(); 
    }
      
    guarda_codigo(guarda_codString, leitura_teclas); 
    loop();
  }
  
  else{
    lcd.print("SEJA BEM-VINDO");
    delay(3000);
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("B: LOG-IN");
    lcd.setCursor(0, 1);
    lcd.print("S: RESET");

    leitura_teclas = NULL;
    while (leitura_teclas == NULL || (leitura_teclas != 'B' && leitura_teclas != 'S')){
      leitura_teclas = teclado.getKey();
    }

    switch(leitura_teclas){
      case 'B':
        tentativas = 1;
        abre_porta(guarda_codString, print_LCDString, motor, leitura_teclas);
        loop();
      case 'S':
        session = 0;
        
        loop();
      default:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ERRO 401");
        loop();
    }
  }

}

void guarda_codigo(String save, char leitura) {

  String save_temp;
  int j=0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("#: FIM DE CODIGO");
  lcd.setCursor(0, 1);
  lcd.print("C: LIMPAR TELA");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GUARDANDO CODIGO");

  leitura = NULL;
  for(j=0; guarda_cod[j - 1] != '#' && j < 9; j++){
    while (leitura == NULL) {
      leitura = teclado.getKey();  // Atribui a variavel a leitura do teclado
    }
    guarda_cod[j] = leitura;
    lcd.setCursor(j, 1);
    lcd.print(guarda_cod[j]);
    if (leitura == 'C') {
      guarda_codigo(save, leitura);
    }
    leitura = NULL;
  }

  save_temp = String(guarda_cod);
  if (save_temp[j - 1] == '#')
    save = save_temp.substring(0, j - 1);
  else
    save = save_temp;

  guarda_codString = save;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CODIGO GUARDADO");
  delay(1500);
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("PRIMA O BOTAO");
  lcd.setCursor(0, 1);
  lcd.print("PARA FECHAR");

  while(digitalRead(portaPin) == LOW){
    delay(100);
   }
  
  for(pos = 0; pos < 180; pos++){ //PARA "pos" IGUAL A 0, ENQUANTO "pos" MENOR QUE 180, INCREMENTA "pos"
    motor.write(pos);             //ESCREVE O VALOR DA POSIÇÃO QUE O SERVO DEVE GIRAR
    delay(15);

  lcd.clear();
  lcd.noBacklight();
  lcd.setCursor(0,0);
  }
  return;
}

void abre_porta(String save, String compare, Servo motor, char leitura) {
  int j = 0;
  String compare_tmp;


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INTRODUZA CODIGO");

  leitura = NULL;

  for(j=0; print_LCD[j - 1] != '#' && j < 9; j++){
    while (leitura == NULL) {
      leitura = teclado.getKey();  // Atribui a variavel a leitura do teclado
    }
    print_LCD[j] = leitura;
    lcd.setCursor(j, 1);
    lcd.print(print_LCD[j]);
    if (leitura == 'C') {
      abre_porta(save, compare, motor, leitura);
    }
    leitura = NULL;
  }

  compare_tmp = String(print_LCD);
  if (compare_tmp[j - 1] == '#')
    compare = compare_tmp.substring(0, j - 1);
  else
    compare = compare_tmp;

  
  print_LCDString = compare;

  while (tentativas != 3) {
    if (compare == save) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CODIGO CORRETO");
      lcd.setCursor(0, 1);
      lcd.print("ABRINDO PORTA");
      delay(500);
      
      for(pos = 180; pos > 0; pos--){ //PARA "pos" IGUAL A 0, ENQUANTO "pos" MENOR QUE 180, INCREMENTA "pos"
          motor.write(pos); //ESCREVE O VALOR DA POSIÇÃO QUE O SERVO DEVE GIRAR
          delay(15);
      }
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PRIMA O BOTAO");
      lcd.setCursor(0, 1);
      lcd.print("PARA FECHAR");

      while(digitalRead(portaPin) == LOW){
        delay(100);
      }
  
      for(pos = 0; pos < 180; pos++){ //PARA "pos" IGUAL A 0, ENQUANTO "pos" MENOR QUE 180, INCREMENTA "pos"
        motor.write(pos);             //ESCREVE O VALOR DA POSIÇÃO QUE O SERVO DEVE GIRAR
        delay(15);
      }
      
      lcd.clear();
      lcd.noBacklight();
      lcd.setCursor(0,0);
      
      loop();
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CODIGO INCORRETO");
      lcd.setCursor(0, 1);
      lcd.print("TEM");
      lcd.setCursor(4, 1);
      Serial.println(3 - tentativas);
      lcd.print(3 - tentativas);
      lcd.setCursor(6, 1);
      lcd.print("TENTATIVAS");

      delay(500);
      tentativas++;
      abre_porta(save, compare, motor, leitura);
      if (tentativas == 3)
        lock();
    }
  }
}

void lock() {
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CHAMEM A POLICIA");


  for(int i=0; i<2; i++){
  for (int m = 600; m < 640; m++) {
    tone(pin_buzzer, m);
    delay(15);
  }
  
  for(int m=640; m >600; m--){
    tone(pin_buzzer, m);
    delay(15);
  }
  }
  
  noTone(pin_buzzer);

  delay(5000);
  lcd.clear();
  lcd.noBacklight();
  lcd.setCursor(0,0);
  loop();
}

double dist() {
  
  double duracao = 0;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duracao = pulseIn(echoPin, HIGH)/58;

  //Serial.println(duracao); //Em cm

  return duracao;
}
