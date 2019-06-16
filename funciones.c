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

void escribirArchivo(char* nombreSalida, Resultado** resultados, int c_discos)
{
  FILE* archivo = fopen(nombreSalida,"w");

  for (int i = 0; i < c_discos; i++)
  {
    fprintf(archivo, "disco %d: \n", i);
    fprintf(archivo, "Media Real: %f\n", resultados[i] -> mediaReal);
    fprintf(archivo, "Media Imaginaria: %f\n", resultados[i] -> mediaImag);
    fprintf(archivo, "Potencia: %f\n", resultados[i] -> potenciaTotal);
    fprintf(archivo, "Ruido Total: %f\n", resultados[i] -> ruidoTotal);
  }
  fclose(archivo);
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
    //printf("HOLA");
    //pthread_mutex_lock(&monitor -> mutex);

    if (monitor -> bandera == 0)
    {
      //printf("INFO: %d,%d,%d\n",monitor->id,monitor->cantidad_datos,monitor->bandera);
      //printf("ME MUERO\n ");
      pthread_cond_wait(&monitor -> bufferNotFull, &monitor -> mutex);
    }

    else if(monitor -> bandera == 1)
    {

      //pthread_cond_signal(&monitor -> bufferFull);
      printf("INFO BANDERA 1: %d,%d,%d\n",monitor->id,monitor->cantidad_datos,monitor->bandera);
      //printf("BANDERA == 1\n");
      //imprimirDatos(monitor);

      //pthread_mutex_lock(&monitor -> mutex);
      printf("mutex bloqueado\n");

      for (int i = 0; i < monitor -> cantidad_datos; i++)
      {
        sumReal = sumReal + monitor -> buffer[i] -> real;
        sumImag = sumImag + monitor -> buffer[i] -> imag;
        sumRuido = sumRuido + monitor -> buffer[i] -> ruido;
      }

      datosTotales = datosTotales + monitor -> cantidad_datos;

      /*
      printf("final monitor id: %d\n", monitor -> id);
      printf("sumaReal: %f\n", sumReal);
      printf("sumaImag: %f\n", sumImag);
      printf("sumaRuido: %f\n", sumRuido);
      */

      for (int i = 0; i < monitor -> cantidad_datos; i++)
      {
        //printf("RIP\n ");
        monitor -> buffer[i]-> real = 0;
        //printf("holi3\n");
        monitor -> buffer[i] -> imag = 0;
        //printf("holi4\n");
        monitor -> buffer[i] -> ruido = 0;
        //printf("holi5\n");
      }
      //printf("RIP\n ");

      monitor -> cantidad_datos = 0;
      monitor -> bandera = 0;

      printf("datos guardados y borrados, mandando señal a bufferFull y desbloqueando mutex\n");

      pthread_cond_signal(&monitor -> bufferFull);
      //pthread_mutex_unlock(&monitor -> mutex);
      printf("mutex desbloqueado\n");



      //pthread_mutex_unlock(&monitor -> mutex);
      //pthread_cond_wait(&monitor -> bufferFull, &monitor -> mutex);

    }

    else if(monitor -> bandera == 2)
    {
      //printf("BANDERA == 2\n");
      //imprimirDatos(monitor);
      printf("INFO BANDERA 2: %d,%d,%d\n",monitor->id,monitor->cantidad_datos,monitor->bandera);
      //ultima lectura
      for (int i = 0; i < monitor -> cantidad_datos; i++)
      {
        printf("vuelta %d\n", i);
        sumReal = sumReal + monitor -> buffer[i] -> real;
        sumImag = sumImag + monitor -> buffer[i] -> imag;
        sumRuido = sumRuido + monitor -> buffer[i] -> ruido;
        printf("vuelta %d lista\n", i);
      }

      datosTotales = datosTotales + monitor -> cantidad_datos;
      printf("resultados\n");
      printf("id: %d\n", monitor -> id);
      resultados[monitor -> id] -> mediaReal = sumReal/datosTotales;
      resultados[monitor -> id] -> mediaImag = sumImag/datosTotales;
      resultados[monitor -> id] -> ruidoTotal = sumRuido;
      resultados[monitor -> id] -> potenciaTotal = sqrt((sumReal*sumReal)+(sumImag*sumImag));
      printf("resultados listos\n");
      /*
      printf("final monitor id: %d\n", monitor -> id);
      printf("sumaReal: %f\n", sumReal);
      printf("sumaImag: %f\n", sumImag);
      printf("sumaRuido: %f\n", sumRuido);
        */
      break;
    }

  }

  printf("matando hebra\n");
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
      printf("disco: %d\n", disco);
      printf("id: %d\n", monitores[disco] -> id);

      pthread_mutex_lock(&monitores[disco] -> mutex);

      while (monitores[disco] -> cantidad_datos == tamanioBuffer)
      {
        printf("WOLOLO\n");

        pthread_cond_wait(&monitores[disco] -> bufferFull,&monitores[disco] -> mutex);
        printf("sali\n");
      }

      posicion = monitores[disco] -> cantidad_datos;
      printf("seteando real\n");
      monitores[disco] -> buffer[posicion] -> real = real;
      //printf("seteando imag\n");
      monitores[disco] -> buffer[posicion] -> imag = imag;
      //printf("seteando ruido\n");
      monitores[disco] -> buffer[posicion] -> ruido = ruido;

      //imprimirDatos(monitores[disco]);
      printf("seteando cantidad_datos: %d\n", monitores[disco]->cantidad_datos);

      posicion = posicion + 1;
      monitores[disco] -> cantidad_datos = posicion;

      if (monitores[disco] -> cantidad_datos == tamanioBuffer)
      {
        monitores[disco] -> bandera = 1;
        printf("bandera = 1\n");
      }
      pthread_cond_signal(&monitores[disco] -> bufferNotFull);
      pthread_mutex_unlock(&monitores[disco] -> mutex);

  }

  for (int i = 0; i < c_discos; i++)
  {
    printf("bandera 2\n");
    monitores[i] -> bandera =  2;
    pthread_cond_signal(&monitores[i] -> bufferNotFull);
    printf("esperando hebra hija\n");
    //pthread_cond_wait(&monitores[disco] -> bufferNotFull, &monitores[disco] -> mutex);
    //printf("durmiendo padre\n");
  }

  fclose(archivo);
}

/*double calcularMediaReal(Dato** datos, int cantidad_datos)
{
  double resultadoMediaReal = 0;
  for (int i = 0; i < cantidad_datos; i++)
  {
    resultadoMediaReal = datos[i] -> real + resultadoMediaReal;
  }

  resultadoMediaReal = resultadoMediaReal/cantidad_datos;
  return resultadoMediaReal;
}

double calcularMediaImaginaria(Dato** datos,int cantidad_datos)
{
  double resultadoMediaImaginaria = 0;
  for (int i = 0; i < cantidad_datos; i++)
  {
    resultadoMediaImaginaria = datos[i] -> imag + resultadoMediaImaginaria;
  }

  resultadoMediaImaginaria = resultadoMediaImaginaria/cantidad_datos;
  return resultadoMediaImaginaria;
}

double calcularPotenciaTotal(Dato** datos,int cantidad_datos)
{
  double resultadoPotenciaTotal = 0;
  for (int i = 0; i < cantidad_datos; i++)
  {
    double potencia = sqrt( (datos[i] -> real * datos[i] -> real) + (datos[i] -> imag * datos[i] -> imag) );
    resultadoPotenciaTotal = potencia + resultadoPotenciaTotal;
  }
  return resultadoPotenciaTotal;
}

double calcularRuidoTotal(Dato** datos,int cantidad_datos)
{
  double resultadoRuidoTotal = 0;
  for (int i = 0; i < cantidad_datos; i++)
  {
    resultadoRuidoTotal = datos[i] -> ruido + resultadoRuidoTotal;
  }
  return resultadoRuidoTotal;
}*/
