/*
 TRABAJO PRÁCTICO FINAL
 GENERACION/SOLUCION Laberinto C/Python

 ALUMNO:
       - KASEVICH, IGNACIO

 GIT: https://github.com/IgnacioKase/prog2-tpfinal

 28/11/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STR_LENGTH 200
#define LOG_FILE "log.txt"

/*
    Estructura utilizada para representar
    una coordenada en el laberinto
*/
struct pair
{
    int first;
    int second;
};

void ltrim(char *str, const char *seps);
void rtrim(char *str, const char *seps);
void trim(char *str, const char *seps);
int checkInput(int argc);
int getInt(FILE *f, int *x);
int checkLine(FILE *f, char *str);
int getWalls(FILE *f, char *maze, const int n);
int getObjectives(FILE *f, char *maze, const int dimension);
int getPair(char *str, struct pair *p);
int insertInMaze(char *maze, int n, struct pair *p, char value);
int insertPair(char *str, char value, char *maze, int n);
void generateRandoms(char *maze, const int n, int r);
void printMaze(FILE *f, char *maze, const int n);
void printError(char *str);

int main(int argc, char *argv[])
{
    //Checkeamos los argumentos
    if (checkInput(argc) != 0)
        return -1;
    char *PATH_IN = argv[1];
    char *PATH_OUT = argv[2];

    //Abrimos el archivo
    FILE *fIn = fopen(PATH_IN, "r");
    FILE *fOut = fopen(PATH_OUT, "w");

    //Si no se puede abrir el archivo, cortamos la ejecución
    if (fIn == NULL || fOut == NULL)
    {
        printError("Error al abrir el archivo");
        return -1;
    }
    ///////////////////////////////////////////////////

    //Variables a utilizar
    int dimension = 0;
    int nRandoms = 0;
    char *maze; // será utilizada como matriz de dos dimensiones
                // pero para pasar (maze) como argumento a funciones,
                // lo declaramos como un puntero
    ///////////////////////////////////////////////////

    //Lee la dimensión del laberinto
    if (checkLine(fIn, "dimension") != 0 || getInt(fIn, &dimension) != 0)
        return -1;

    // Inicializamos la matriz para el laberinto
    maze = (char *)malloc(dimension * dimension * sizeof(char));
    if (maze == NULL)
    {
        printError("Memoria insuficiente para este laberinto.\n");
        return -1;
    }
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            *(maze + (i * dimension + j)) = '0';
    ///////////////////////////////////////////////////

    //Lee el archivo y genera la primer parte del laberito
    if (checkLine(fIn, "obstaculos fijos") == -1)
        return -1;
    if (getWalls(fIn, maze, dimension) != 1)
        return -1;
    ///////////////////////////////////////////////////

    // Lee cantidad de obstaculos aleatorios
    if (getInt(fIn, &nRandoms) != 0)
        return -1;
    ///////////////////////////////////////////////////

    //Lee y escribe los objetivos de entrada y salida del laberinto
    getObjectives(fIn, maze, dimension);

    //Genera los obstaculos aleatorios
    generateRandoms(maze, dimension, nRandoms);

    //Imprime el laberinto
    printMaze(fOut, maze, dimension);

    free(maze);
    fclose(fIn);
    return 0;
}

/************************ STRING UTILITIES ****************************/

/*
    Toma una string(array de char) y elimina
    los caracteres: en @seps o los siguientes si
    seps es NULL:
    \t\n\v\f\r (incluido espacios en blancos)
    del lado izquierdo de la string
*/
void ltrim(char *str, const char *seps)
{
    size_t totrim;
    if (seps == NULL)
    {
        seps = "\t\n\v\f\r ";
    }
    totrim = strspn(str, seps);
    if (totrim > 0)
    {
        size_t len = strlen(str);
        if (totrim == len)
        {
            str[0] = '\0';
        }
        else
        {
            memmove(str, str + totrim, len + 1 - totrim);
        }
    }
}

/*
    Toma una string(array de char) y elimina
    los caracteres: en @seps o los siguientes si
    seps es NULL:
    \t\n\v\f\r (incluido espacios en blancos)
    del lado derecho de la string
*/
void rtrim(char *str, const char *seps)
{
    int i;
    if (seps == NULL)
    {
        seps = "\t\n\v\f\r ";
    }
    i = strlen(str) - 1;
    while (i >= 0 && strchr(seps, str[i]) != NULL)
    {
        str[i] = '\0';
        i--;
    }
}

/*
    Toma una string(array de char) y elimina
    los caracteres: en @seps o los siguientes si
    seps es NULL:
    \t\n\v\f\r (incluido espacios en blancos)
    del lado derecho e izquierdo de la string
*/
void trim(char *str, const char *seps)
{
    rtrim(str, seps);
    ltrim(str, seps);
}

/*********************** FILE AUX. FUNCTIONS **************************/

/*
    Checkea que la cantidad de parametros
    recibidos en el main sea la correcta,
    en caso de error muestra un mensaje.
    Retorna 0 en caso de terminar correctamente,
    -1 en caso contrario.
*/
int checkInput(int argc)
{
    //Checkeamos recibir los parametros correctamente
    if (argc != 3)
    {
        char output[410] = "\n---------------------------------------\n\n";
        strcat(output, "Error, ingrese el path del archivo:\n\n");
        strcat(output, "1 - Archivo de entrada\n\n");
        strcat(output, "Ejemplo:\n\n");
        strcat(output, "$ gcc __main.c\n");
        strcat(output, "$ fileName in/test1.txt maze/maze1.txt\n\n");
        strcat(output, "---------------------------------------\n");
        printError(output);
        return -1;
    }
    return 0;
}

/*
    Lee la siguiente linea de un archivo
    y guarda el valor leido en un entero (se pasa el puntero).
    Retorna 0 en caso de terminar correctamente,
    -1 en caso contrario.
*/
int getInt(FILE *f, int *x)
{
    char buffer[MAX_STR_LENGTH]; //buffer para lecutura

    if (fgets(buffer, MAX_STR_LENGTH - 1, f) == NULL)
        return -1;
    trim(buffer, NULL);
    sscanf(buffer, "%d", x);
    return 0;
}

/*
    Lee la siguiente linea de un archivo
    y compara la linea leida con una string
    @str.
    Retorna 0 en caso de ser iguales,
    -1 en caso contrario.
*/
int checkLine(FILE *f, char *str)
{
    char buffer[MAX_STR_LENGTH]; //buffer para lecutura

    if (fgets(buffer, MAX_STR_LENGTH - 1, f) != NULL)
    {
        trim(buffer, NULL);
        if (strcmp(buffer, str) != 0)
        {
            printError("Error en el formato de entrada");
            return -1;
        }
    }
    else
        return -1;
    return 0;
}

/*
    Lee los obstaculos fijos del archivo
    y los inserta en la matriz @maze,
    checkeando que las coordenadas sean válidas.
    Retorna 0 en caso de terminar correctamente,
    -1 en caso contrario.
*/
int getWalls(FILE *f, char *maze, const int n)
{
    char buffer[MAX_STR_LENGTH]; //buffer para lecutura

    int flag = 0;
    while (flag == 0)
    {
        if (fgets(buffer, MAX_STR_LENGTH - 1, f) != NULL)
        {
            trim(buffer, NULL);
            if (strcmp(buffer, "obstaculos aleatorios") != 0)
            {
                flag = insertPair(buffer, '1', maze, n);
                if (flag == -1)
                    printError("Error, coordenadas invalidas.");
            }
            else
            {
                flag = 1;
            }
        }
    }
    return flag;
}

/*
    Lee los objetivos del archivo
    y los inserta en la matriz @maze,
    checkeando que las coordenadas sean válidas.
    Retorna 0 en caso de terminar correctamente,
    -1 en caso contrario.
*/
int getObjectives(FILE *f, char *maze, const int dimension)
{
    char buffer[MAX_STR_LENGTH]; //buffer para lecutura

    // Lee posicion inicial y la inserta en la matriz
    if (checkLine(f, "posicion inicial") == -1)
        return -1;
    fgets(buffer, MAX_STR_LENGTH - 1, f);
    if (insertPair(buffer, 'I', maze, dimension) == -1)
    {
        printError("Error, coordenadas invalidas.\n");
        return -1;
    }

    ///////////////////////////////////////////////////

    // Lee posicion final en la matriz
    if (checkLine(f, "objetivo") == -1)
        return -1;
    fgets(buffer, MAX_STR_LENGTH - 1, f);
    if (insertPair(buffer, 'X', maze, dimension) == -1)
    {
        printError("Error, coordenadas invalidas.\n");
        return -1;
    }
    ///////////////////////////////////////////////////
    return 0;
}

/************************* AUX. FUNCTIONS *****************************/

/*
    Recibe una string @str de la forma: 
        (x[int], y[int])
    Lee los enteros de la string y los escribe
    en una estructura pair (se pasa el puntero).
    p.first = x
    p.second = y
    Retorna 0 en caso de terminar correctamente,
    -1 en caso contrario.
*/
int getPair(char *str, struct pair *p)
{
    trim(str, "()\t\n\v\f\r "); // quita espacios y parentesis del inicio y fin de la string
    int i = 0;
    int newLine = 0;
    int countCommas = 0;
    while (str[i] != '\0')
    {
        if (str[i] == ',')
        {
            str[i] = '\0';
            countCommas++;
            newLine = i + 1;
        }
        i++;
    }
    if (countCommas != 1)
        return -1;
    sscanf(str, "%d", &p->first);
    sscanf((str + newLine), "%d", &p->second);
    return 0;
}

/*
    Inserta el caracter @value en la posicion indicada
    por la estructura pair @p en el laberinto @maze.
    (A las coordenadas se les resta 1).
    Checkeando que las coordenadas sean válidas.
    Retorna 0 en caso de terminar correctamente,
    -1 en caso contrario.
*/
int insertInMaze(char *maze, int n, struct pair *p, char value)
{
    int y = p->first - 1;
    int x = p->second - 1;

    if (x > (n - 1) || x < 0 || y > (n - 1) || y < 0)
        return -1;

    if (*(maze + (x * n + y)) != '0')
        return -1;

    *(maze + (x * n + y)) = value;
    return 0;
}

/*
    Recibe una string con coordenadas @str, un valor @value,
    el laberinto @maze y la dimensión del mismo @n
    e inserta el valor @value en las coordenadas que indique @str
    en @maze.
    Retorna 0 en caso de terminar correctamente,
    -1 en caso contrario.
*/
int insertPair(char *str, char value, char *maze, int n)
{
    struct pair p;
    if (getPair(str, &p) == -1)
        return -1;
    return insertInMaze(maze, n, &p, value);
}

/*
    Genera @r obstaculos aleatorios 
    en el laberinto @maze.
*/
void generateRandoms(char *maze, const int n, int r)
{
    srand(time(0));
    struct pair p;
    while (r > 0)
    {
        p.first = rand() % n + 1;
        p.second = rand() % n + 1;
        if (insertInMaze(maze, n, &p, '1') == 0)
            r--;
    }
}

/*
    Imprime el laberinto en por consola
    (standard output)
*/
void printMaze(FILE *f, char *maze, const int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            fprintf(f, "%c ", *(maze + (i * n + j)));
        fprintf(f, "\n");
    }
}

void printError(char *str)
{
    FILE *f = fopen(LOG_FILE, "a+");
    fprintf(f, "%ld\t%s", time(0), str);
    fclose(f);
}
