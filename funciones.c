#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "funciones.h"


//Entrada: Char con el nombre del fichero
//Funcionamiento: Funcion que obtiene el largo lineas que tiene el fichero de entrada
//Salida:entero Largo arreglo datos del fichero
// La lectura de archivos presento problemas ya que en algunos comutadores leia un elemento
// extra, en caso de que no corra usar largo = 1
int largoArchivo(char* nombre)
{
  FILE* archivo = fopen(nombre, "r");
  int largo = 0;  // largo = 1;
  char linea[400];

  while (!feof(archivo))
  {
    fgets (linea, 400, archivo);
    //printf("%s\n", linea);
    largo++;
  }
  fclose(archivo);
  return largo;
}

//Entrada: enteros u y v
//Funcionamiento: calcula la norma de < u,v >
//Salida: la norma de < u,v >
int calcularDisco(double u,double v,double ancho, int c_discos)
{

  double disco = sqrt(u*u+v*v);
  int disco2 = trunc(disco/ancho);

  if (disco2 >= c_discos) {
    disco2 = c_discos-1;

  }
  return disco2;

}

void escribirArchivo(char* nombreSalida, double** resultados, int c_discos)
{
  FILE* archivo = fopen(nombreSalida,"w");

  for (int i = 0; i < c_discos; i++)
  {
    if (resultados[i][2] == 0)
    {
      fprintf(archivo, "disco %d: \n", i+1);
      fprintf(archivo, "Media Real: 0\n");
      fprintf(archivo, "Media Imaginaria: 0\n");
      fprintf(archivo, "Potencia: 0\n");
      fprintf(archivo, "Ruido Total: 0\n");
    }
    else
    {
      fprintf(archivo, "disco %d: \n", i);
      fprintf(archivo, "Media Real: %f\n", resultados[i][0]);
      fprintf(archivo, "Media Imaginaria: %f\n", resultados[i][1]);
      fprintf(archivo, "Potencia: %f\n", resultados[i][2]);
      fprintf(archivo, "Ruido Total: %f\n", resultados[i][3]);
    }
  }
  fclose(archivo);
}

void escribirEnPantalla(double** resultados, int c_discos)
{
  for (int i = 0; i < c_discos; i++)
  {
    printf("Soy la hebra %d, y procese %d Visibilidades.\n", i+1, (int) resultados[i][4]);
  }
  printf("\n");
}

void imprimirDatos(Monitor* monitor)
{
  for (int i = 0; i < monitor -> cantidad_datos; i++)
  {
    printf("#########\n");
    printf("##real: %f\n", monitor -> buffer[i] -> real);
    printf("##imag: %f\n", monitor -> buffer[i] -> imag);
    printf("##ruido: %f\n", monitor -> buffer[i] -> ruido);
  }
}

//Consumidor
void* procesarDatos(void* monitor_param) /// FUNCION QUE UTILIZA LA HEBRA HIJA
{

  Monitor* monitor = (Monitor*)monitor_param;
  double sumReal = 0;
  double sumImag = 0;
  double sumRuido = 0;
  int datosTotales = 0;

  while(1){
    if (monitor -> bandera == 0)
    {
      //printf("INFO: %d,%d,%d\n",monitor->id,monitor->cantidad_datos,monitor->bandera);
      pthread_cond_wait(&monitor -> bufferNotFull, &monitor -> mutex);
    }

    else if(monitor -> bandera == 1)
    {

      //printf("INFO BANDERA 1: %d,%d,%d\n",monitor->id,monitor->cantidad_datos,monitor->bandera);

      //pthread_mutex_lock(&monitor -> mutex);

      for (int i = 0; i < monitor -> cantidad_datos; i++)
      {
        sumReal = sumReal + monitor -> buffer[i] -> real;
        sumImag = sumImag + monitor -> buffer[i] -> imag;
        sumRuido = sumRuido + monitor -> buffer[i] -> ruido;
      }

      datosTotales = datosTotales + monitor -> cantidad_datos;

      for (int i = 0; i < monitor -> cantidad_datos; i++)
      {
        monitor -> buffer[i]-> real = 0;
        monitor -> buffer[i] -> imag = 0;
        monitor -> buffer[i] -> ruido = 0;
      }

      monitor -> cantidad_datos = 0;
      monitor -> bandera = 0;


      pthread_cond_signal(&monitor -> bufferFull);
      //pthread_mutex_unlock(&monitor -> mutex);

    }

    else if(monitor -> bandera == 2)
    {
      //printf("INFO BANDERA 2: %d,%d,%d\n",monitor->id,monitor->cantidad_datos,monitor->bandera);
      //ultima lectura
      for (int i = 0; i < monitor -> cantidad_datos; i++)
      {
        sumReal = sumReal + monitor -> buffer[i] -> real;
        sumImag = sumImag + monitor -> buffer[i] -> imag;
        sumRuido = sumRuido + monitor -> buffer[i] -> ruido;
      }

      datosTotales = datosTotales + monitor -> cantidad_datos;
      resultados[monitor -> id][0] = sumReal/datosTotales;
      resultados[monitor -> id][1] = sumImag/datosTotales;
      resultados[monitor -> id][2] = sqrt((sumReal*sumReal)+(sumImag*sumImag));
      resultados[monitor -> id][3] = sumRuido;
      resultados[monitor -> id][4] = datosTotales;

      break;
    }

  }

}

void leerArchivo(Monitor** monitores,char* nombre, int anchoDisco,int c_discos, int tamanioBuffer)
{
  // ############################################### Leer archivo Y Mandar Visibilidades ###########################################################################
  FILE* archivo = fopen(nombre,"r");
  int cont = 0;
  double u,v,real,imag,ruido;
  int disco;
  int posicion;
  char c1,c2,c3,c4;
  // Aqui se lee el archivo guardando cada linea en el atributo dato de cada hebra
  while(!feof(archivo))
  {

      u = 0;
      v = 0;
      real = 0;
      imag = 0;
      ruido = 0;

      fscanf(archivo, "%lf %c %lf %c %lf %c %lf %c %lf", &u,&c1,&v,&c2,&real,&c3,&imag,&c4,&ruido);
      //printf("%lf %lf %lf %lf %lf\n", u,v,real,imag,ruido);

      disco = calcularDisco(u, v, anchoDisco, c_discos);

      pthread_mutex_lock(&monitores[disco] -> mutex);

      while (monitores[disco] -> cantidad_datos == tamanioBuffer)
      {
        pthread_cond_wait(&monitores[disco] -> bufferFull,&monitores[disco] -> mutex);
      }

      posicion = monitores[disco] -> cantidad_datos;
      monitores[disco] -> buffer[posicion] -> real = real;
      monitores[disco] -> buffer[posicion] -> imag = imag;
      monitores[disco] -> buffer[posicion] -> ruido = ruido;

      posicion = posicion + 1;
      monitores[disco] -> cantidad_datos = posicion;

      if (monitores[disco] -> cantidad_datos == tamanioBuffer)
      {
        monitores[disco] -> bandera = 1;
      }
      pthread_cond_signal(&monitores[disco] -> bufferNotFull);
      pthread_mutex_unlock(&monitores[disco] -> mutex);

  }

  for (int i = 0; i < c_discos; i++)
  {
    monitores[i] -> bandera =  2;
    pthread_cond_signal(&monitores[i] -> bufferNotFull);
    //pthread_cond_wait(&monitores[disco] -> bufferNotFull, &monitores[disco] -> mutex);
  }

  fclose(archivo);
}
