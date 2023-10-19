// UTN FRA - Sistema de Procesamiento de Datos
// Juan Pablo Ledezma - Iván Laurito


/** Agregamos las variables:
*
* SWITCH_NUMEROS: correspondiente al switch usado para elegir
* entre el contador normal y el contador de primos. Este switch
* solo funciona para las ejecuciones de los botones, siempre que
* SWITCH_INGRESO esté LOW.
*
* SWITCH_INGRESO: correspondiente al switch utilizado para
* intercalar entre el conteo de números por medio de los botones
* como se realizó en la Parte 1 y el conteo por medio del sensor
* de temperatura. El valor mínimo del sensor devuelve el 0 y 
* su valor máximo devuelve el 99. El número devuelto se mostrará
* en el display con la multiplexación de la misma manera que en
* la Parte 1.
*
* SENSOR: referente a la entrada analógica del sensor de temperatura.
*
* MOTOR: referente a la salida digital del motor
*/


#define A 13
#define B 12
#define C 11
#define D 10
#define E 9
#define F 8
#define G 7
#define RESET 5
#define AUMENTAR 4
#define DISMINUIR 3
#define SWITCH_NUMEROS 2
#define SWITCH_INGRESO 1
#define UNIDADES A0
#define DECENAS A1
#define SENSOR A3
#define MOTOR 6
#define DELAY 10

typedef struct {
  bool ejecucion;
  bool estadoAnterior;
}estructura;



estructura detectarPulsacion(bool estadoActual, bool estadoAnterior);



void mostrarNumero(int num);



void encenderDisplays(int posicion);



void encenderNumero(int numero);



int normalizarContador(int contador, int limiteInferior, int limiteSuperior);



/**
*  Contador de primos:
*
*  Cuando el switch está en modo 'Números primos', el contador de 
*  primos aumenta o disminuye según qué botón se presionó.
*  Si ese nuevo número no es primo, se repite la acción hasta
*  que el número cumpla con esa condición. Entonces, la función 
*  muestra el siguiente o el anterior número primo según corresponda.
*  
*  Si el número primo mostrado es 97, al presionar el botón de 
*  Aumentar' el contador pasa a ser 2.
*  Si el número primo mostrado es 2, al presionar el botón de 
*  'Disminuir' el contador pasa a ser 97.
*
*/
bool esPrimo(int numero);



void setup()
{
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(RESET, INPUT_PULLUP);
  pinMode(AUMENTAR, INPUT_PULLUP);
  pinMode(DISMINUIR, INPUT_PULLUP);
  pinMode(SWITCH_NUMEROS, INPUT_PULLUP);
  pinMode(SWITCH_INGRESO, INPUT_PULLUP);
  pinMode(UNIDADES, OUTPUT);
  pinMode(DECENAS, OUTPUT);
  pinMode(SENSOR, INPUT);
  digitalWrite(UNIDADES, 0);
  digitalWrite(DECENAS, 0);
  encenderNumero(0);
  Serial.begin(9600);
}

int numero;
int numeroPrimo = 2;
int numeroContador = 0;
int numeroMotor;
int lectura;

bool btnSumarEstadoActual;
bool btnRestarrEstadoActual;
bool btnResetEstadoActual;

bool btnSumarEstadoAnterior = false;
bool btnRestarrEstadoAnterior = false;
bool btnResetEstadoAnterior = false;
bool estadoSwitchNumeros;
bool estadoSwitchIngreso;

estructura deteccionSuma;
estructura deteccionResta;
estructura deteccionReset;

void loop()
{
  btnSumarEstadoActual = !(digitalRead(AUMENTAR));
  btnRestarrEstadoActual = !(digitalRead(DISMINUIR));
  btnResetEstadoActual = !(digitalRead(RESET));
  estadoSwitchNumeros = !(digitalRead(SWITCH_NUMEROS));
  estadoSwitchIngreso = !(digitalRead(SWITCH_INGRESO));

  deteccionSuma = detectarPulsacion(btnSumarEstadoActual, btnSumarEstadoAnterior);
  btnSumarEstadoAnterior = deteccionSuma.estadoAnterior;

  deteccionResta = detectarPulsacion(btnRestarrEstadoActual, btnRestarrEstadoAnterior);
  btnRestarrEstadoAnterior = deteccionResta.estadoAnterior;

  deteccionReset = detectarPulsacion(btnResetEstadoActual, btnResetEstadoAnterior);
  btnResetEstadoAnterior = deteccionReset.estadoAnterior;
  
  /**
  *
  * Si el switch de ingresos está en HIGH, se podrán usar los
  * botones para aumentar, disminuir o resetear al valor mínimo
  * el número exhibido en el display y se podrá cambiar entre
  * el conteo normal de número y el conteo de números primos
  * exclusivamente.
  *
  * Si el switch de ingresos está en HIGH, los displays mostrarán
  * un número entre 0 y 99 y se aumentará la frecuencia del motor
  * hasta un máximo cercano a 3000 rpm según el valor devuelto por
  * el sensor de temperatura.
  *
  * Ambas aplicaciones funcionan aisladamente.
  *
  */
  
  if (estadoSwitchIngreso == HIGH) {

    if (estadoSwitchNumeros == HIGH){
      if (deteccionSuma.ejecucion){
      numeroContador++;
      }
      if (deteccionResta.ejecucion){
      numeroContador--;
      }
      if (deteccionReset.ejecucion){
      numeroContador = 0;
      }
      numeroContador = normalizarContador(numeroContador, 0, 99);
      numero = numeroContador;
    }
    else{
      if (deteccionSuma.ejecucion){
        numeroPrimo++;
        while (!esPrimo(numeroPrimo)){
          if (numeroPrimo > 99){
              numeroPrimo = 2;
              break;
          }
          numeroPrimo++;
          Serial.println(numeroPrimo);
        }
      }
      if (deteccionResta.ejecucion){
        numeroPrimo--;
        while (!esPrimo(numeroPrimo)){
          if (numeroPrimo < 2){
              numeroPrimo = 97;
              break;
          }
          numeroPrimo--;
        }
      }
      if (deteccionReset.ejecucion){
        numeroPrimo = 2;
      }
      numero = numeroPrimo;  
    }
  }
  else
  {
    /**
    *
    * Si el switch de ingreso esta en modo sensor, se obtendrá
    * el valor analógico del sensor de temperatura. Luego, se usará
    * la función map() para devolver el valor equivalente entre
    * 0 y 99 y éste se mostrará en los displays. Simultáneamente,
    * se usará map() para devolver el valor equivalente entre
    * 0 y 255, el cual definirá la frecuencia del motor
    * 
    */
    lectura = analogRead(SENSOR);
    numeroMotor = map(lectura, 20, 358, 0, 255);
    numero = map(lectura, 20, 358, 0, 99);
    Serial.println(numero);
    Serial.println(numeroMotor);
    analogWrite(MOTOR, numeroMotor);
    
  }
  
  Serial.println(numero);
  mostrarNumero(numero);
  
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



estructura detectarPulsacion(bool estadoActual, bool estadoAnterior){
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

bool esPrimo(int numero) {
  if (numero < 2) {
    return false;
  }
  for (int i = 2; i <= (numero / 2); i++) {
    if (numero % i == 0) {
    return false;
    }
}
  return true;
}