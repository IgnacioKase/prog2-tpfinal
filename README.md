# prog2-tpfinal

Trabajo práctico Final LCC - UNR FCEIA

Este trabajo consta de dos partes, la generación de un laberinto
en C y la solución del mismo en Python.

El programa en C genera un laberinto a partir de un archivo fuente,
un modelo del mismo se puede encontrar en 'in/test1.txt'. Que consta
con el siguiente formato:

dimension
int
obstaculos fijos
(x,y)
...
(z,w)
obstaculos aleatorios
int
posicion inicial
(a1,a2)
objetivo
(b1,b2)

Todos numeros enteros, no se pueden repetir coordenadas, y se debe
garantizar que el archivo fuente tenga solución. (Puede que el
resultado final no lo tenga por los obstaculos aleatorios, pero
en tal caso el programa de python se encaragará de generar otro
laberinto).

Una vez generado, el programa de python se encargará de encontrar
el camino mas corto de la posición incial al objetivo.

# Como usar

- Compilar el programa de C
- Ejecutar el programa en python

# Compilar

Para compilar el programa,

\$ gcc \_\_main.c

# Ejecutar programa en python

\$ python fileName arg1 arg2 arg3 arg4

O especificando la versión,

\$ python3 fileName arg1 arg2 arg3 arg4

# Argumentos

- arg1 = Path al programa compilado de C
- arg2 = Path al archivo de entrada de C
- arg3 = Path al archivo de salida de C (laberinto)
- arg3 = Path al archivo de salida de Python (solución)

# Boceto de funcionamiento del programa de Python

Se ejecuta el programa en C y se lee el archivo de salida,
con el mismo se forma una lista, que representara a nuestro
laberinto, el cual lo utilizaremos como un grafo implicito.

A la vez que leemos el laberinto del archivo, checkeamos
donde esta el punto incial y donde la salida y lo guardamos.

Una vez obtenido el grafo(laberinto) haremos BFS (busqueda
en anchura) utilizando a la posición inicial como nodo.
En este BFS, guardaremos el padre de cada nodo visitado,
para asi luego de encontrar el objetivo, hacer backtracking
hasta el punto inicial para reconstruir el camino.

En caso de no encontrar ningun camino, se vuelve a ejecutar
el programa de generación del laberinto.

Luego la salida se escribe en un archivo.

# Modalidad de testeo

El unit testing en las funciones de python se realiza de la siguiente
manera, cada función tiene un archivo que será abierto por la función
de testeo, en la carpeta 'test', con el nombre de la función a testear

En los archivos de testeo, se escribira una linea con la entrada de
la función y debajo otra linea con la salida esperada.

INPUT
OUPUT

Es decir que el archivo deberá tener una cantidad par.

Los parametros de cada función estan escritos en orden como la declaración
de la misma.

Tener en cuenta que las listas estan separadas por ';' en vez de ','.

### Este approach sirve para laberintos de ~10^4x10^4 dependiendo de las distancias entre el inicio y el objetivo
