// UTN FRA - Sistema de Procesamiento de Datos
// Juan Pablo Ledezma - Iván Laurito

#define A 13
#define B 12
#define C 11
#define D 10
#define E 9
#define F 8
#define G 7
#define LED_1 2
#define LED_2 3
#define LED_3 4
#define SENSOR_1 A3
#define SENSOR_2 A4
#define SENSOR_3 A5
#define RESET 5
#define UNIDADES A0
#define DECENAS A1
#define DELAY 10


typedef struct {
  bool ejecucion = false;
  bool estadoAnterior = false;
}estructura;

void mostrarNumero(int num);
void encenderDisplays(int posicion);
void encenderNumero(int numero);
int normalizarContador(int contador, int limiteInferior, int limiteSuperior);

estructura detectarMovimiento(bool estadoActual, bool estadoAnterior);


// Establecer los pines como entradas/salidas de acuerdo a su objetivo.
void setup()
{
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(SENSOR_1, INPUT_PULLUP);
  pinMode(SENSOR_2, INPUT_PULLUP);
  pinMode(SENSOR_3, INPUT_PULLUP);
  pinMode(RESET, INPUT_PULLUP);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(UNIDADES, OUTPUT);
  pinMode(DECENAS, OUTPUT);
  digitalWrite(UNIDADES, 0);
  digitalWrite(DECENAS, 0);
  encenderNumero(0);
  //Serial.begin(9600);
}

// Declaración de variables.

// Número que será mostrado en el display.
int contadorDetecciones = 0;

int lecturaSensor1;
int lecturaSensor2;
int lecturaSensor3;

bool estadoActualSensor1;
bool estadoActualSensor2;
bool estadoActualSensor3;

bool deteccionReset;

estructura deteccionSensor1;
estructura deteccionSensor2;
estructura deteccionSensor3;

// flag para poner el contador en 0 al principio,
// ya que los sensores marcan un true al inicio.
bool flagInicioContador = true;


// Loop principal.
void loop()
{
  contadorDetecciones = normalizarContador(contadorDetecciones, 0, 99);
  
  deteccionReset = !(digitalRead(RESET));
 
  lecturaSensor1 = analogRead(SENSOR_1);
  lecturaSensor2 = analogRead(SENSOR_2);
  lecturaSensor3 = analogRead(SENSOR_3);
  
  lecturaSensor1 = map(lecturaSensor1,524,1023,0,100);
  lecturaSensor2 = map(lecturaSensor2,524,1023,0,100);
  lecturaSensor3 = map(lecturaSensor3,524,1023,0,100);
  
  if (lecturaSensor1 <= 20)
    estadoActualSensor1 = true;
  else
    estadoActualSensor1 = false;
  
  if (lecturaSensor2 <= 20)
    estadoActualSensor2 = true;
  else
    estadoActualSensor2 = false;
  
  if (lecturaSensor3 <= 20)
    estadoActualSensor3 = true;
  else
    estadoActualSensor3 = false;
  
  deteccionSensor1 = detectarMovimiento(estadoActualSensor1, deteccionSensor1.estadoAnterior);
  deteccionSensor2 = detectarMovimiento(estadoActualSensor2, deteccionSensor2.estadoAnterior);
  deteccionSensor3 = detectarMovimiento(estadoActualSensor3, deteccionSensor3.estadoAnterior);
    
  if (estadoActualSensor1)
    digitalWrite(LED_1, HIGH);
  
  if (estadoActualSensor2)
    digitalWrite(LED_2, HIGH);
  
  if (estadoActualSensor3)
    digitalWrite(LED_3, HIGH);
  
  if (deteccionReset){
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
  }
  
  // Si se detecta actividad en cualquier sensor, se aumenta el contador
  if (deteccionSensor1.ejecucion || deteccionSensor2.ejecucion || deteccionSensor3.ejecucion){
    contadorDetecciones++;
  }
  
  if (flagInicioContador){
    contadorDetecciones = 0;
    flagInicioContador = false;
  }
  
  //Serial.println(contadorDetecciones);
  mostrarNumero(contadorDetecciones);
  delay(25);
}

void mostrarNumero(int num) {
  digitalWrite(UNIDADES, HIGH);
  digitalWrite(DECENAS, HIGH);
  encenderNumero(num % 10);
  encenderDisplays(UNIDADES);
  digitalWrite(UNIDADES, HIGH);
  digitalWrite(DECENAS, HIGH);
  encenderNumero(num / 10);
  encenderDisplays(DECENAS);
}

void encenderDisplays(int posicion)
{
  if (posicion == UNIDADES)
  {
    digitalWrite(UNIDADES, LOW);
    digitalWrite(DECENAS, HIGH);
    delay(DELAY);
  }
  else
  {
    digitalWrite(DECENAS, LOW);
    digitalWrite(UNIDADES, HIGH);
    delay(DELAY);
  }
}

void encenderNumero(int numero){
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  switch (numero){
    case 1:
  	  digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      break;
    case 2:
  	  digitalWrite(A, HIGH);
  	  digitalWrite(B, HIGH);
      digitalWrite(E, HIGH);
  	  digitalWrite(D, HIGH);
      digitalWrite(G, HIGH);
      break;
    case 3:
      digitalWrite(A, HIGH);
  	  digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
  	  digitalWrite(D, HIGH);
      digitalWrite(G, HIGH);
      break;
    case 4:
  	  digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
  	  digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      break;
    case 5:
  	  digitalWrite(A, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
  	  digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      break;
    case 6:
  	  digitalWrite(A, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
  	  digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      break;
    case 7:
  	  digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      break;
    case 8:
  	  digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
  	  digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      break;
    case 9:
  	  digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
  	  digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      break;
    default:
  	  digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
  	  digitalWrite(F, HIGH);
      break;
  }
}

int normalizarContador(int contador, int limiteInferior, int limiteSuperior){

  int diferencia;
  
  if (contador > limiteSuperior){
    diferencia = contador - limiteSuperior;
    contador = limiteInferior + diferencia - 1;
  }
  if (contador < limiteInferior){
    diferencia = limiteInferior - contador;
    contador = limiteSuperior - diferencia + 1;
  }

  return contador;
}

estructura detectarMovimiento(bool estadoActual, bool estadoAnterior){
  estructura devolucion;

  devolucion.estadoAnterior = estadoAnterior;

  if (estadoActual != estadoAnterior){
    devolucion.estadoAnterior = estadoActual;
    devolucion.ejecucion = estadoActual;
  }
  else{
    devolucion.ejecucion = false;
  }
      
  return devolucion;
}