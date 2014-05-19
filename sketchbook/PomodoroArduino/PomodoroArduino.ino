/* configuracion */
#define BTN_RESET 4
#define BTN_UNO   7
#define BTN_DOS   8
#define LED_RESET 9
#define LED_UNO   5
#define LED_DOS   6 
#define MOTOR    10
#define ZUMBADOR 11

/* globales */
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
  pinMode(MOTOR, OUTPUT);
  pinMode(ZUMBADOR, OUTPUT);
  digitalWrite(LED_UNO, HIGH);
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
    analogWrite(LED_RESET, 0);
    analogWrite(LED_UNO, 255);
    analogWrite(LED_DOS, 0);
    inicio = millis();
    tiempo = 1500000; // milisegundos
    beep(1);
  }
  if (digitalRead(BTN_DOS) == LOW && !activo) {
    /* modo 5 minutos */
    activo = true;
    debug = 0;
    analogWrite(LED_RESET, 0);
    analogWrite(LED_UNO, 0);
    analogWrite(LED_DOS, 255);
    inicio = millis();
    tiempo = 300000; // milisegundos
    beep(1);
  }
  /* si estamos activo */
  if (activo) {
    debug++;
    if (debug > 50) {
      /* solo imprimos el debug cada 50 ciclos */
      debug = 0;
      Serial.println("--------------------------------------");
      Serial.print("inicio: ");
      Serial.println(inicio);
      Serial.print("tiempo: ");
      Serial.println(tiempo);
      Serial.print("millis: ");
      Serial.println(millis());
    }
    if (inicio + tiempo <= millis()) {
      /* beep y reset */
      beep(3);
      reset();
    }
  }
  delay(100);
}

/* reinicio de globales y leds */
void reset () {
  activo = false;
  analogWrite(LED_RESET, 63);
  analogWrite(LED_UNO, 0);
  analogWrite(LED_DOS, 0);
}

/* sonido del zumbador y vibracion del motor*/
void beep(int j) {
  for (int i = 0; j > i; i++) {
    analogWrite(MOTOR, 127);
    tone(ZUMBADOR, 4978);
    delay(100);
    noTone(ZUMBADOR);
    delay(0);    
    analogWrite(MOTOR, 0);
    delay(250);
  }
}
