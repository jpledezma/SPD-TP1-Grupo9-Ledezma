// UTN FRA - Sistema de Procesamiento de Datos
// Juan Pablo Ledezma - Iván Laurito

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
#define UNIDADES A0
#define DECENAS A1
#define DELAY 10

// Estructura de datos
// Como una función sólo puede devolver un valor, se creará esta estructura que contiene 2 variables, 
// para así poder devolver ambos valores dentro de un único tipo de dato.
typedef struct {
  // Variable para decidir ejecutar o no una acción de acuerdo a su valor.
  bool ejecucion;
  // Variable para guardar en memoria el estado anterior de un botón.
  bool estadoAnterior;
}estructura;



/**
 * Detectar el cambio en el estado de una entrada digital.
 *
 * Esta función toma el estado actual de una entrada y la compara con su estado anterior
 * para ejecutar una acción por única vez en base a esos datos.
 *
 * @param estadoActual El estado actual de la entrada (activo/no activo).
 * @param estadoAnterior Estado inmediatamente anterior de la entrada.
 * @return Se devuelve el estado anterior de la entrada, y la ejecución (o no) de la acción.
 */
estructura detectarPulsacion(bool estadoActual, bool estadoAnterior);

/**
 * Muestra un número en los displays
 *
 * Se muestra el número ingresado por parámetro, y es mostrado
 * usando la función de multiplexación.
 *
 * @param num Número a ser mostrado.
 */
void mostrarNumero(int num);



/**
 * Enciende los displays usando la multiplexación:
 *
 * 1. Se enciende un display de las unidades/decenas con el dígito 
 * 2. Se apaga el otro display.
 * 3. Esto queda así durante los 10 ms. que dura el delay.
 * 4. Se repite el proceso.
 *
 * Dado que los pines de salida para el display de las unidades 
 * es el mismo que para el de las decenas, no podemos enviar 
 * dígitos diferentes para cada display en el mismo instante. 
 * En su lugar, lo que hacemos es mostrar uno de los dos dígitos 
 * del número a la vez, intercalándolos tan rápido que el ojo 
 * humano no nota este proceso.
 *
 * @param posicion es el display que será encendido.
 */
void encenderDisplays(int posicion);



/**
 * Muestra un número en un display
 *
 * Se encienden los LED del display para mostrar un número.
 *
 * @param numero Número a ser mostrado.
 */
void encenderNumero(int numero);



/**
 * Cambia un número por si se sale de los límites establecidos.
 *
 * Esta función toma un número y lo compara con el límite superior, y con el límite inferior.
 * Si es mayor que el límite superior, calcula la diferencia y se la suma al límite inferior.
 * Si es menor que el límite inferior, calcula la diferencia y se la resta al límite superior.
 * Si está dentro de los límites, lo deja como está.
 *
 * @param contador El número a ser comparado.
 * @param limiteInferior El valor mínimo que puede tener el número.
 * @param limiteSuperior El valor máximo que puede tener el número.
 * @return Se devuelve el número ingresado con las modificaciones necesarias.
 */
int normalizarContador(int contador, int limiteInferior, int limiteSuperior);



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
  pinMode(RESET, INPUT_PULLUP);
  pinMode(AUMENTAR, INPUT_PULLUP);
  pinMode(DISMINUIR, INPUT_PULLUP);
  pinMode(UNIDADES, OUTPUT);
  pinMode(DECENAS, OUTPUT);
  digitalWrite(UNIDADES, 0);
  digitalWrite(DECENAS, 0);
  encenderNumero(0);
  Serial.begin(9600);
}

// Declaración de variables.

// Número que será mostrado en el display.
int numero = 0;



// Variables para detectar la pulsación de los botones.
bool btnSumarEstadoActual;
bool btnRestarEstadoActual;
bool btnResetEstadoActual;



/* 
 * Variables para obtener el estado anterior de los botones, y así poder
 * ejecutar una acción por única vez, de acuerdo al cambio de estado del botón.
 * Se inicializan en false, ya que al principio del programa no estarán siendo presionados.
 */
bool btnSumarEstadoAnterior = false;
bool btnRestarEstadoAnterior = false;
bool btnResetEstadoAnterior = false;



// Estructruras de datos para obtener el cambio de estado en un botón
// y actualizar su estado anterior
estructura deteccionSuma;
estructura deteccionResta;
estructura deteccionReset;



// Loop principal.
void loop()
{
  // Cambiar el valor del número en caso de que esté fuera de los límites establecidos.
  numero = normalizarContador(numero, 0, 99);

  /* Detectar los cambios de estado de los pulsadores
   * Como en la configuración INPUT_PULLUP, el estado por defecto es HIGH, 
   * se niegan las estradas para hacer que el estado "no presionado" sea LOW, 
   * y el estado "presionado" sea HIGH.
   */
  btnSumarEstadoActual = !(digitalRead(AUMENTAR));
  btnRestarEstadoActual = !(digitalRead(DISMINUIR));
  btnResetEstadoActual = !(digitalRead(RESET));
  
  // Obtención del estado anterior de los botones y la variable de ejecución de cada uno.
  deteccionSuma = detectarPulsacion(btnSumarEstadoActual, btnSumarEstadoAnterior);
  btnSumarEstadoAnterior = deteccionSuma.estadoAnterior;

  deteccionResta = detectarPulsacion(btnRestarEstadoActual, btnRestarEstadoAnterior);
  btnRestarEstadoAnterior = deteccionResta.estadoAnterior;

  deteccionReset = detectarPulsacion(btnResetEstadoActual, btnResetEstadoAnterior);
  btnResetEstadoAnterior = deteccionReset.estadoAnterior;

  // Ejecutar una acción en base al estado de los botones anteriormente mencionados.

  // Aumentar el contador en 1.
  if (deteccionSuma.ejecucion){
    numero++;
  }
  // Disminuir el contador en 1.
  if (deteccionResta.ejecucion){
    numero--;
  }
  // Reiniciar el contador (devolverlo a 0)
  if (deteccionReset.ejecucion){
    numero = 0;
  }
  
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
  // Declaración de la variable principal.
  estructura devolucion;
  // Asignación del estado anterior de la variable que será devuelta, 
  //en base al parámetro.
  devolucion.estadoAnterior = estadoAnterior;
  
  /* 
   * En caso de haber un cambio en el estado, el estado anterior
   * pasa a ser igual al estado actual.
   * La ejecución toma el valor del estado actual de la entrada.
   */
  if (estadoActual != estadoAnterior){
    devolucion.estadoAnterior = estadoActual;
    devolucion.ejecucion = estadoActual;
  }
  // Si no hay un cambio en el estado, la acción no será ejecutada.
  else{
    devolucion.ejecucion = false;
  }
      
  return devolucion;
}

int normalizarContador(int contador, int limiteInferior, int limiteSuperior){

  int diferencia;
  
  if (contador > limiteSuperior){
    diferencia = contador - limiteSuperior;
    // El -1 es necesario para que también se tenga en cuenta al número del límite inferior
    contador = limiteInferior + diferencia - 1;
  }
  if (contador < limiteInferior){
    diferencia = limiteInferior - contador;
    // El +1 es necesario para que también se tenga en cuenta al número del límite superior
    contador = limiteSuperior - diferencia + 1;
  }

  return contador;
}