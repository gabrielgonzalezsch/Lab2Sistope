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
  char* nombreSalida = "salida.txt";
  int c_discos = 5;
  int anchoDisco = 10;
  int bandera = 0;  // 0 no mostrar en pantalla y 1 mostrar en pantalla
  int largo;
  int tamanioBuffer = 2;

  //INICIALIZAR lista global compartida
  Resultado** resultados = (Resultado**) malloc(sizeof(Resultado*)*c_discos);
  for (int i = 0; i < c_discos; i++)
  {
    resultados[i] = (Resultado*) malloc(sizeof(Resultado));
  }

  Monitor** monitores = (Monitor**)malloc(sizeof(Monitor*)*c_discos);
  pthread_t  hebras[c_discos];

  // INICIALIZAR HEBRAS CONSUMIDORES HIJAS
  for (int i = 0; i < c_discos; i++)
  {
    // INICIALIZAR Monitor
    monitores[i] = (Monitor*)malloc(sizeof(Monitor));
    pthread_cond_init(&monitores[i] -> bufferNotFull, NULL);
    pthread_cond_init(&monitores[i] -> bufferFull, NULL);
    pthread_mutex_init(&monitores[i] -> mutex, NULL);

    // INICIALIZAR BUFFER
    monitores[i] -> buffer = (Buffer**) malloc(sizeof(Buffer*) * tamanioBuffer);
    for (int j = 0; j < tamanioBuffer; j++) {
      monitores[i] -> buffer[j] =(Buffer*) malloc(sizeof(Buffer));
    }
    monitores[i] -> id = i;
    monitores[i] -> bandera = 0;
    //pthread_mutex_lock(&mutex);
    //pthread_mutex_unlock(&mutex);
  }

  //Creando hebras
  for (int i = 0; i < c_discos; i++)
  {
    pthread_create(&hebras[i], NULL, procesarDatos, (void*)monitores[i]);
  }

  leerArchivo(monitores, nombre, anchoDisco, c_discos, tamanioBuffer);

  //Esperando hijos
  for (int i = 0; i < c_discos; i++)
  {
    printf("esperanding %d\n", monitores[i] -> id);
    pthread_join(hebras[i],NULL);
    printf("hebra %d finalizada\n", monitores[i] -> id);
  }

  //Escribiendo datos
  for (int i = 0; i < c_discos; i++)
  {
    escribirArchivo(nombreSalida, resultados, c_discos);
  }


  /*int i = 0;
  while (i < c_discos)
  {
    //pthread_mutex_lock(&mutex);
    pthread_create(&hilo[i], NULL, procesarDatos, (void *) hebras[i]);
    //pthread_mutex_unlock(&mutex);
    i++;
  }*/

  return 0;
}
