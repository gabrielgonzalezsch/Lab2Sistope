#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "funciones.h"

pthread_mutex_t *mutex;


int main(int argc, char** argv)
{
  int opt;
  int largo;

  char* nombre =  (char*) malloc(sizeof(char*) * 20);
  char* nombreSalida =  (char*) malloc(sizeof(char*) * 20);
  int c_discos = -1;
  int anchoDisco = -1;
  int tamanioBuffer = -1;
  int bFLag = -1;


  while ((opt = getopt(argc, argv, ":iondsb:x")) != -1)
  {
    switch (opt)
    {
      case 'i':
        strcpy(nombre, argv[2]);
        printf("entrada .csv: %s\n", nombre);
        break;
      case 'o':
        strcpy(nombreSalida, argv[4]);
        printf("salida .txt: %s\n", nombreSalida);
        break;
      case 'n':
        if (atoi(argv[6]) > 0)
        {
          c_discos = atoi(argv[6]);
          printf("Numero de discos: %d\n", c_discos);
        }
        else
        {
          printf("El valor ingresado en \"numero de discos\" es invalido\n");
          exit(1);
        }
        break;
      case 'd':
        anchoDisco = atoi(argv[8]);
        printf("ancho: %d\n", anchoDisco);
        break;
      case 's':
        if (atoi(argv[10]) > 0)
        {
          tamanioBuffer = atoi(argv[10]);
          printf("Numero de discos: %d\n", tamanioBuffer);
        }
        else
        {
          printf("El valor ingresado en \"tamaño buffer\" es invalido\n");
          exit(1);
        }
        break;
      case 'b':
        bFLag = atoi(argv[12]);
        printf("bandera: %d\n", bFLag);
        break;
      case ':':
        printf("option needs a value\n");
        break;
      case '?':
        printf("unknown option: %c\n", optopt);
        break;
    }
  }

  // Este for captura los elementos extra del argumento
  for(optind = 12; optind < argc; optind++){
      printf("extra arguments: %s\n", argv[optind]);
  }

  printf("\n");


  //INICIALIZAR lista global compartida
  resultados = (double**) malloc(sizeof(double*)*c_discos);

  for (int i = 0; i < c_discos; i++)
  {
    resultados[i] = (double*) malloc(sizeof(double)*4);
    resultados[i][0] = 0;
    resultados[i][1] = 0;
    resultados[i][2] = 0;
    resultados[i][3] = 0;

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
    pthread_join(hebras[i],NULL);
  }

  //Escribiendo datos

  escribirArchivo(nombreSalida, resultados, c_discos);

  return 0;
}
