#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


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
  int id;
  Dato** dato;
  int largo;
  int bandera;
  double mediaReal;
  double mediaImag;
  double potenciaTotal;
  double ruidoTotal;


  int datos[5]; // prueba
  int resultados[5]; //prueba
  int y; //prueba

  pthread_mutex_t mutex; //prueba, pero podria servir
} Hebra;
