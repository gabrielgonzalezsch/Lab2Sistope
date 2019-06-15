#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


typedef struct Buffer
{
  double real;
  double imag;
  double ruido;
} Buffer;


typedef struct Resultados
{
  double mediaReal;
  double mediaImag;
  double potenciaTotal;
  double ruidoTotal;
} Resultado;


typedef struct Monitor
{

  int id;
  int bandera;
  int cantidad_datos;
  Buffer** buffer;
  pthread_mutex_t mutex;
  pthread_cond_t bufferNotEmpty;
  pthread_cond_t bufferNotFull;


} Monitor;
