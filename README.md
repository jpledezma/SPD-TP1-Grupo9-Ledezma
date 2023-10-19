# SPD Parcial domiciliario - Grupo 9


## Integrantes 
- Iván Laurito
- Juan Pablo Ledezma

# Doble display de 7 segmentos

## Parte 1: Contador.
![Tinkercad](./img/diseño-parte-1.png)

### Descripción
Este circuito controla 2 displays de 7 segmentos para mostrar un número. El mismo está inicializado en 0, y se puede aumentar o disminuir su valor en 1, o resetearlo a 0; pulsando el botón correspondiente.

Para mostrar controlar los 2 display con una única salida, se utiliza la técnica de la multiplexación, la cual consiste en encender un display por un instante, luego apagarlo y encerder el otro, repitiendo este proceso varias veces por segundo. De esta manera se logra engañar al ojo humano, el cuál percibe que ambos displays están encendidos al mismo tiempo.

La configuración de los displays es cátodo común.


### Funciones principales
Estas son las funciones que se encargan de la multiplexación.

UNIDADES es un #define del pin de salida que está conectado al cátodo del display derecho (el que muestra las unidades), mientras que DECENAS es el pin conectado al cátodo del display izquierdo (que muestra las decenas).

La función encenderDisplays recibe como parámetro la posición del número que será mostrado; si la misma es UNIDADES, se enciende el display derecho y se apaga el izquierdo, caso contrario se enciende el display izquierdo y se apaga el derecho.

La función mostrarNumero recibe como parámetro al número de 2 cifras que será mostrado. Comienza apagando ambos displays, a continuación, usa el operador módulo "%" para obtener el resto entre la división del número ingresado y 10, para así separar las unidades, y selecciona los segmentos correspondientes del display. Luego, llama a la función encenderDisplays para prender el display de las unidades. Después, vuelve a apagar ambos displays y vuelve a hacer los mismo, pero esta vez para las decenas. Para separar las decenas de las unidades, se divide al número por 10. Las unidades quedarían en la parte fraccionaria luego de la división, y al estar usando números enteros, se truncaría el resultado, quedando solo con las decenas.

~~~ C
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
~~~


## Parte 2: Implementación de números primos.
![Tinkercad](./img/diseño-parte-2.png)

### Descripción
Este circuito consta de 2 displays que que muestran el número ingresado por el usuario mediante los componentes. Presenta
dos modos de ingreso del valor númerico: por medio de los botones y acorde al valor devuelto por el sensor de temperatura
que oscila entre los -40ºC y los 125ºC.

Si el interruptor del switch de ingreso se encuentra hacia su izquierda, se hallará en modo Botones. Este modo permite
usar los botones para aumentar, disminuir y resetar el contador del número mostrado en los displays al mínimo. En este
modo, si el switch está en estado HIGH, funciona como un contador lineal, que cambia su valor en 1 por cada pulsación.
Si está en estado LOW, el contador calcula el próximo número primo, en caso que se presione el botón "Aumentar"; o el anterior número primo, en caso que se presione el botón "Disminuir".

Si el interruptor del switch de ingreso se encuentra hacia su derecha, se hallará en modo Sensor. Este modo devolverá el
número de 0 a 99 a mostrarse en los displays según el valor obtenido del sensor de temperatura. Además, en este modo se
activará el motor en una frecuencia equivalente al valor del sensor de temperatura, siendo su valor mínimo equivalente a 0 rpm y su valor máximo equivaente a 2941 rpm.


### Función principal
La función toma como argumento un número entero, y devuelve `true` si el número es primo, o `false` si el número NO es primo.

~~~ C
bool esPrimo(int numero) {
  // No existen números primos menores a 2.
  if (numero < 2) {
    return false;
  }
  /*
   * Se buscan divisores del número, entre 2 (el número primo más pequeño), 
   * y la mitad del número (los números superiores a su mitad no pueden ser divisores).
   * Si encuentra un divisor, se devuelve false.
   */
  for (int i = 2; i <= (numero / 2); i++) {
    if (numero % i == 0) {
    return false;
    }
  }
  return true;
}
~~~

## Parte 3: Fotodiodo.
![Tinkercad](./img/diseño-parte-3.png)

### Descripción
Este circuito simula un sensor de seguridad de 3 zonas. Para ello, utiliza 3 fotodiodos, los cuales, en este caso, funcionan como sensores de movimiento. 

Un fotodiodo en polarización directa funciona como un diodo normal, permite el paso de la corriente en un sentido, siempre y cuando se supere la tensión necesaria; pero tiene una función especial en polarización inversa: genera una pequeña corriente cuando es expuesto a la luz. De esta manera, puede ser utilizado para detectar la incidencia de luz o la falta de ella, con un tiempo de respuesta muy rápido. Dependiendo de su material de construcción, detecta diferentes frecuencias del espectro electromagnético.

En este circuito hay 3 fotodiodos, cada uno está conectado a una entrada analógica del arduino. Si se detecta una disminución en la cantidad de luz del 80% en alguno de ellos, se enciende el LED que está enlazado a ese fotodiodo, y aumenta el contador en el display. Una vez que se encendió el LED, seguirá estando encendido hasta que se apague manualmente, mediante el botón RESET.

### Funciones principales
Para tomar la lectura del fotodiodo, se utiliza la función map()

~~~ C
  lecturaSensor1 = map(lecturaSensor1,524,1023,0,100);
  lecturaSensor2 = map(lecturaSensor2,524,1023,0,100);
  lecturaSensor3 = map(lecturaSensor3,524,1023,0,100);
~~~
De esta manera, se pueden leer los valores como porcentaje de la cantidad de luz que ingresa al fotodiodo.

Luego, se encienden los LED de advertencia en función de la lectura de los fotodiodos, y se aumenta el contador.

~~~ C
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

///////////////////////////////////////////////////////////////////

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
~~~


## :robot: Link al proyecto
- [Parte 1](https://www.tinkercad.com/things/fl4kxIXSfHR?sharecode=D6jwre6D70nH2UNetzScZsd0BpX8h9_mCDD-rFe1n2s)
- [Parte 2](https://www.tinkercad.com/things/bWPrIjv5mIT?sharecode=_IR0yJF-QYcyFThGaMv9oA8YFEt4gtaEelkMpPUhj4I)
- [Parte 3](https://www.tinkercad.com/things/cTIY6pEUOiK?sharecode=tQ9t70NPiFNzLOMxvOk7tkYJcTSMVF6QCmDrY3vUBYQ)
