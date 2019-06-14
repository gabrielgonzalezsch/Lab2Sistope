#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "funciones.h"

pthread_mutex_t *mutex;


int main(int argc, char const *argv[])
{


  char* nombre = "test.csv";
  int c_discos = 5;
  int anchoDisco = 10;
  int bandera = 0;  // 0 no mostrar en pantalla y 1 mostrar en pantalla
  int largo;

  Hebra** hebras = (Hebra**) malloc(sizeof(Hebra*) * c_discos);  //10 = c_discos
  pthread_t hilo[c_discos];

  for (int i = 0; i < c_discos; i++) {
    hebras[i] = (Hebra*) malloc(sizeof(Hebra));
    hebras[i] -> largo = 0;
    hebras[i] -> id = i;
    hebras[i] -> mediaReal = 0;
    hebras[i] -> mediaImag = 0;
    hebras[i] -> potenciaTotal = 0;
    hebras[i] -> ruidoTotal = 0;
    hebras[i] -> bandera = bandera;
    hebras[i] -> mutex = mutex;
  }

  //pthread_mutex_init(&mutex, NULL);

  largo =  largoArchivo(nombre);

  double** datos = leerDatos(nombre, largo);

  hebras = repartirDatos(hebras, datos, largo, c_discos, anchoDisco);

  free(datos);
  

  int i = 0;
  while (i < c_discos)
  {
    //pthread_mutex_lock(&mutex);
    pthread_create(&hilo[i], NULL, procesarDatos, (void *) hebras[i]);
    //pthread_mutex_unlock(&mutex);
    i++;
  }

  return 0;
}
