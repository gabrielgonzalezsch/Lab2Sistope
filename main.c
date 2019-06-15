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
  int tamanioBuffer = 2;

  Monitor** monitores = (Monitor**)malloc(sizeof(Monitor*)*c_discos);
  pthread_t  hebras[c_discos];

  // INICIALIZAR HEBRAS CONSUMIDORES HIJAS

  for (int i = 0; i < c_discos; i++) {

        // INICIALIZAR Monitor
        monitores[i] = (Monitor*)malloc(sizeof(Monitor));
        pthread_cond_init(&monitores[i] -> bufferNotEmpty, NULL);
        pthread_cond_init(&monitores[i] -> bufferNotFull, NULL);

        // INICIALIZAR BUFFER
        monitores[i] -> buffer =(Buffer**) malloc(sizeof(Buffer*) * tamanioBuffer);
        for (int j = 0; j < tamanioBuffer; j++) {
          monitores[i] -> buffer[j] =(Buffer*) malloc(sizeof(Buffer));
        }
        monitores[i] -> id = i;
        monitores[i] -> bandera = 0;
        //pthread_mutex_lock(&mutex);
        pthread_create(&hebras[i], NULL, procesarDatos, (void*)monitores[i]);
        //pthread_mutex_unlock(&mutex);
      }

  // ############################################### Leer archivo Y Mandar Visibilidades ###########################################################################
  FILE* archivo = fopen(nombre,"r");
  int cont = 0;
  double u,v,real,imag,ruido;
  int disco;
  int posicion;
  char c1,c2,c3,c4;
  // Aqui se lee el archivo guardando cada linea en el atributo dato de cada hebra
  while(!feof(archivo)){

      u = 0;
      v = 0;
      real = 0;
      imag = 0;
      ruido = 0;

      fscanf(archivo, "%lf %c %lf %c %lf %c %lf %c %lf", &u,&c1,&v,&c2,&real,&c3,&imag,&c4,&ruido);
      //printf("%lf %lf %lf %lf %lf\n", u,v,real,imag,ruido);

      disco = calcularDisco(u, v, anchoDisco, c_discos);
      //pthread_mutex_lock(&monitores[disco] -> mutex);
      //printf("disco: %d\n", disco);

      posicion = monitores[disco] -> cantidad_datos;
      //printf("seteando real\n");
      monitores[disco] -> buffer[posicion] -> real = real;
      //printf("seteando imag\n");
      monitores[disco] -> buffer[posicion] -> imag = imag;
      //printf("seteando ruido\n");
      monitores[disco] -> buffer[posicion] -> ruido = ruido;

      //imprimirDatos(monitores[disco]);
      //printf("seteando cantidad_datos: %d\n", monitores[disco]->cantidad_datos);
      posicion = posicion+1;
      monitores[disco] -> cantidad_datos = posicion;
      if (monitores[disco] -> cantidad_datos == tamanioBuffer)
      {
        //printf("WOLOLO\n");
        monitores[disco] -> bandera = 1;

        pthread_cond_signal(&monitores[disco] -> bufferNotFull);
        pthread_cond_wait(&monitores[disco] -> bufferNotEmpty,&monitores[disco] -> mutex);
      }

      //pthread_mutex_unlock(&monitores[disco] -> mutex);



  }


  for (int i = 0; i < c_discos; i++) {
    printf("bandera 2\n");
    monitores[i] -> bandera =  2;
    pthread_cond_signal(&monitores[i] -> bufferNotFull);
    printf("esperando hebra hija\n");
    //pthread_cond_wait(&monitores[disco] -> bufferNotEmpty, &monitores[disco] -> mutex);
    //printf("durmiendo padre\n");
  }

  for (int i = 0; i < c_discos; i++) {
    printf("esperanding %d\n", monitores[i] -> id);
    pthread_join(hebras[i],NULL);
    printf("hebra %d finalizada\n", monitores[i] -> id);
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
