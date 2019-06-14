#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "funciones.h"


int main(int argc, char const *argv[])
{


  char* nombre = "test.csv";
  int c_discos = 5;
  int anchoDisco = 10;
  int bandera = 0;  // 0 no mostrar en pantalla y 1 mostrar en pantalla
  int largo;

  Hebra** hebras = (Hebra**) malloc(sizeof(Hebra*) * c_discos);  //10 = c_discos

  for (int i = 0; i < c_discos; i++) {
    hebras[i] = (Hebra*) malloc(sizeof(Hebra));
    hebras[i] -> largo = 0;
    hebras[i] -> id = i;
    hebras[i] -> mediaReal = 0;
    hebras[i] -> mediaImag = 0;
    hebras[i] -> potenciaTotal = 0;
    hebras[i] -> ruidoTotal = 0;
    hebras[i] -> bandera = bandera;
  }


  largo =  largoArchivo(nombre);
  double** datos = leerDatos(nombre, largo);

  hebras = repartirDatos(hebras, datos, largo, c_discos, anchoDisco);

  pthread_t hilo[c_discos];

  int i = 0;
  while (i < c_discos)
  {
    pthread_create(&hilo[i], NULL, procesarDatos()/*Funcion qla*/, /* parametro qlo */)
  }



  Hebra* hebra = (Hebra*) malloc(sizeof(Hebra));

  hebra -> datos[0] = 11;
  hebra -> datos[1] = 22;
  hebra -> datos[2] = 33;
  hebra -> datos[3] = 44;
  hebra -> datos[4] = 55;
  hebra -> y = 3;

  /*
  pthread_mutex_init(&hebra -> mutex, NULL);

  printf("antes de definir y crear thread\n");

  //pthread_t hilo;  //definit hilo/hebra

  pthread_create(&hilo, NULL, &procesarDatosSimples, (void*) hebra);

  pthread_create(&hilo, NULL, &imprimirDatos, (void*) hebra);

  pthread_join(hilo, NULL);

  printf("final\n");
  */

  return 0;
}
