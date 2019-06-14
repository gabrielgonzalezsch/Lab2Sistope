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
  pthread_mutex_t* mutex;

} Hebra;
