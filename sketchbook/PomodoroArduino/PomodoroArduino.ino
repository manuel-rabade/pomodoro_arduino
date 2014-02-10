#include <Servo.h> 

/* configuracion */
#define BTN_RESET 2
#define BTN_UNO   3
#define BTN_DOS   4
#define LED_RESET 5
#define LED_UNO   6
#define LED_DOS   7 
#define SERVO     9
#define ZUMBADOR 11

/* globales */
Servo aguja; // aguja en el servomotor
boolean activo; // true = modo 5 o 15 minutos, false = esperando modo
unsigned long inicio; // inicio del ciclo
unsigned long tiempo; // tiempo total del ciclo en milisegundos
int debug; // contador de ciclos para imprimir debug

/* inicializamos */
void setup() {  
  Serial.begin(9600);
  pinMode(LED_RESET, OUTPUT);
  pinMode(LED_UNO, OUTPUT);
  pinMode(LED_DOS, OUTPUT);
  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(BTN_UNO, INPUT_PULLUP);
  pinMode(BTN_DOS, INPUT_PULLUP);
  aguja.attach(SERVO);
  aguja.write(30); // el servo va de 30 a 120 grados
  pinMode(ZUMBADOR, OUTPUT);
  reset();
  beep(1);
}

/* ciclo infinito */
void loop() {
  /* maquina de estados que responde a la flag activo y si se presionan botones */
  if (digitalRead(BTN_RESET) == LOW && activo) {
    /* reset */
    reset();
    beep(1);
  }
  if (digitalRead(BTN_UNO) == LOW && !activo) {
    /* modo 25 minutos */
    activo = true;
    debug = 0;
    digitalWrite(LED_RESET, LOW);
    digitalWrite(LED_UNO, HIGH);
    digitalWrite(LED_DOS, LOW);
    aguja.write(30);
    inicio = millis();
    tiempo = 1500000; // 25 minutos en milisegundos
    beep(1);
  }
  if (digitalRead(BTN_DOS) == LOW && !activo) {
    /* modo 5 minutos */
    activo = true;
    debug = 0;
    digitalWrite(LED_RESET, LOW);
    digitalWrite(LED_UNO, LOW);
    digitalWrite(LED_DOS, HIGH);
    aguja.write(30);
    inicio = millis();
    tiempo = 300000; // 5 minutos en milisegundos
    beep(1);
  }
  /* si estamos activo */
  if (activo) {
    /* calculamos viaje de la aguja */
    unsigned long diferencia = millis() - inicio; // cuantos milisegundos llevamos corriendo
    float avance = (float) diferencia / (float) tiempo; // % de avance
    int grados = (avance * 120) + 30; // posicion de la aguaja en grados (de 30 a 120)
    aguja.write(grados);
    debug++;
    if (debug > 50) {
      /* solo imprimos el debug cada 50 ciclos de loop() */
      debug = 0;
      Serial.println("--------------------------------------");
      Serial.print("inicio: ");
      Serial.println(inicio);
      Serial.print("tiempo: ");
      Serial.println(tiempo);
      Serial.print("diferencia: ");
      Serial.println(diferencia);
      Serial.print("avance: ");
      Serial.println(avance, DEC);    
      Serial.print("grados: ");
      Serial.println(grados, DEC);
    }
    if (avance > 1) {
      /* si llegamos al 100% hacemos beep y reset */
      beep(3);
      reset();
    }
  }
  delay(100);
}

/* reinicio de globales, leds y aguja */
void reset () {
  activo = false;
  digitalWrite(LED_RESET, HIGH);
  digitalWrite(LED_UNO, LOW);
  digitalWrite(LED_DOS, LOW);
  aguja.write(30);
}

/* sonido del zumbador */
void beep(int j) {
  for (int i = 0; j > i; i++) {
    tone(ZUMBADOR, 4978, 90);
    delay(500);
  }
}

