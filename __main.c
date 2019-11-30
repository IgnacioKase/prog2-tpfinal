/*
 TRABAJO PRÁCTICO N° 2
 GENERACIÓN/LECTURA DE ARCHIVOS - C

 INTEGRANTES:
       - KASEVICH, IGNACIO

 GIT: https://github.com/IgnacioKase/prog2-tp2

 17/11/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LENGTH 200
#define TEST_PATH "in/test1.txt"

struct pair {
    int first;
    int second;
};

void ltrim(char *str, const char *seps);
void rtrim(char *str, const char *seps);
void trim(char *str, const char *seps);
int checkInput(int argc);
int checkFiles(FILE *files[], int n);
int getInputInt(FILE *f, char *buffer, int *x);
int getWithCheckString(char *str, FILE *f, char *buffer);
int insertPair(char *str, char value, char *maze, int n);
int getPair(char *str, struct pair *p);
int insertInMaze(char *maze, int n, struct pair *p, char value);

int main(int argc, char *argv[])
{
    /*if (checkInput(argc) != 0)
        return -1;*/

    //Abrimos los archivos correspondientes
    //char *PATH_IN = argv[1];
    char *PATH_IN = TEST_PATH;

    FILE *fIn = fopen(PATH_IN, "r");

    //Si no se puede abrir algún archivo, cortamos la ejecución
    if (fIn == NULL)
    {
        printf("Error al abrir el archivo");
        return -1;
    }
    ///////////////////////////////////////////////////

    char line[MAX_STR_LENGTH]; //buffer para lecutura

    if (getWithCheckString("dimension", fIn, line) == -1)
        return -1;

    int dimension = 0;
    if(getInputInt(fIn, line, &dimension) != 0)
		return -1;
		
    if (getWithCheckString("obstaculos fijos", fIn, line) == -1)
        return -1;

    char maze[dimension][dimension];
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            maze[i][j] = '0';

    int flag = 0;
    while (flag == 0)
    {
		if(fgets(line, MAX_STR_LENGTH - 1, fIn) != NULL ){
			trim(line, NULL);
			if (strcmp(line, "obstaculos aleatorios") != 0){
				flag = insertPair(line, '1', maze, dimension);
				if(flag == -1){
					printf("Error, coordenadas invalidas.");
				}			
			}else{
				flag = 1;
			}
		}
    }
    
    
    if(flag != 1)
		return 0;
		
	int nRandoms = 0;
	
    if(getInputInt(fIn, line, &nRandoms) != 0)
		return -1;
		
	struct pair p;
	printf("%d\n", nRandoms);
	while(nRandoms > 1){
		p.first = rand() % dimension +1 ;
		p.second = rand() % dimension + 1;
		printf("%d\t%d\n", p.first, p.second);
		if(insertInMaze(&maze, dimension, &p, '1') == 0)
			nRandoms--;
	}
	
	printf("%d\n", nRandoms);
		
    if (getWithCheckString("posicion inicial", fIn, line) == -1)
        return -1;
        
    fgets(line, MAX_STR_LENGTH - 1, fIn);
     	
	if(insertPair(line, 'I', &maze, dimension) == -1){
		printf("Error, coordenadas invalidas.");
	}	
	
	if (getWithCheckString("objetivo", fIn, line) == -1)
        return -1;
    
    fgets(line, MAX_STR_LENGTH - 1, fIn);

	if(insertPair(line, 'X', &maze, dimension) == -1){
		printf("Error, coordenadas invalidas.");
	}		
		
    for (int i = 0; i < dimension; i++){
        for (int j = 0; j < dimension; j++){
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
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

/************************* AUX. FUNCTIONS *****************************/

int checkInput(int argc)
{
    //Checkeamos recibir los parametros correctamente
    if (argc != 4)
    {
        printf("---------------------------------------\n\n");
        printf("Error, ingrese el path de los archivos:\n\n");
        printf("1 - Archivo de ciudades\n");
        printf("2 - Archivo de personas\n");
        printf("3 - Archivo de salida\n\n");
        printf("Ejemplo:\n\n");
        printf("$ gcc __main.c\n");
        printf("$ fileName db/codigoLocalidades.txt db/personas.txt out/out.txt\n\n");
        printf("---------------------------------------\n");
        return -1;
    }
    return 0;
}

int getInputInt(FILE *f, char *buffer, int *x){
	if (fgets(buffer, MAX_STR_LENGTH - 1, f) == NULL)
		return -1;
	trim(buffer, NULL);
	sscanf(buffer, "%d", x);
	return 0;
}

int getWithCheckString(char *str, FILE *f, char *buffer)
{
    if (fgets(buffer, MAX_STR_LENGTH - 1, f) != NULL)
    {
        trim(buffer, NULL);
        if (strcmp(buffer,str) != 0)
        {
            printf("Error en el formato de entrada");
            return -1;
        }
    }
    return 0;
}

int insertPair(char *str, char value, char *maze, int n){
	struct pair p;
	if(getPair(str, &p) == -1)
		return -1;
	return  insertInMaze(maze, n, &p, value);
}

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

int insertInMaze(char *maze, int n, struct pair *p, char value)
{
	int y = p->first -1;
	int x = p->second -1;
	
	if(x > (n-1) || x < 0 || y > (n-1) || y < 0){
		return -1;
	}
	
	if(*(maze + (x*n+y)) != '0'){
		return -1;
	}
	
	*(maze + (x*n+y)) = value;
	return 0;
}
