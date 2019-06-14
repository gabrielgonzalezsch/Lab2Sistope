#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct hijos // Estructura que Guardar los datos del fichero de entrada
{
  int pid;
  int visibilidades;
  int pipeA[2];
  int pipeB[2];
  int ver;
} Hijo;

typedef struct datos
{
  double u;
  double v;
  double real;
  double imag;
  double ruido;
} Dato;

typedef struct hebras
{
  Dato dato[0];
  int datos[5]; // prueba
  int resultados[5]; //prueba
  int y; //prueba
  pthread_mutex_t mutex; //prueba, pero podria servir
} Hebra;
