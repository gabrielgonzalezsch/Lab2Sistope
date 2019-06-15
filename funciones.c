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
int calcularDisco(double u,double v,double ancho, int c_discos){

  double disco = sqrt(u*u+v*v);
  int disco2 = trunc(disco/ancho);

  if (disco2 >= c_discos) {
    disco2 = c_discos-1;

  }
  return disco2;

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
void* procesarDatos(void* monitor_param){   /// FUNCION QUE UTILIZA LA HEBRA HIJA

  Monitor* monitor = (Monitor*)monitor_param;
  double sumReal = 0;
  double sumImag = 0;
  double sumRuido = 0;
  int datosTotales = 0;
  Resultado* resultados = (Resultado*) malloc(sizeof(Resultado));

  while(1){
    //printf("HOLA");
    //pthread_mutex_lock(&monitor -> mutex);

    if (monitor -> bandera == 0) {
    //  printf("INFO: %d,%d,%d\n",monitor->id,monitor->cantidad_datos,monitor->bandera);
      //printf("ME MUERO\n ");

      pthread_cond_wait(&monitor -> bufferNotFull, &monitor -> mutex);
    }

    else if(monitor->bandera == 1){

      //pthread_cond_signal(&monitor -> bufferNotFull);
      //printf("INFO: %d,%d,%d\n",monitor->id,monitor->cantidad_datos,monitor->bandera);
      //printf("BANDERA == 1\n");
      //imprimirDatos(monitor);
      for (int i = 0; i < monitor -> cantidad_datos; i++)
      {
        sumReal = sumReal + monitor -> buffer[i] -> real;
        sumImag = sumImag + monitor -> buffer[i] -> imag;
        sumRuido = sumRuido + monitor -> buffer[i] -> ruido;
        //printf("ME quier conaa\n ");

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
        monitor -> cantidad_datos = 0;
      }
      //printf("RIP\n ");

      pthread_cond_signal(&monitor -> bufferNotEmpty);
      //pthread_mutex_unlock(&monitor -> mutex);

      monitor -> bandera = 0;
      //pthread_cond_wait(&monitor -> bufferNotFull, &monitor -> mutex);

    }

    else if(monitor -> bandera == 2){
      printf("BANDERA == 2\n");
      imprimirDatos(monitor);
      printf("INFO: %d,%d,%d\n",monitor->id,monitor->cantidad_datos,monitor->bandera);
      //ultima lectura
      for (int i = 0; i < monitor -> cantidad_datos; i++)
      {
        sumReal = sumReal + monitor -> buffer[i] -> real;
        sumImag = sumImag + monitor -> buffer[i] -> imag;
        sumRuido = sumRuido + monitor -> buffer[i] -> ruido;
      }

      datosTotales = datosTotales + monitor -> cantidad_datos;

      resultados -> mediaReal = sumReal/datosTotales;
      resultados -> mediaImag = sumImag/datosTotales;
      resultados -> ruidoTotal = sumRuido;
      resultados -> potenciaTotal = sqrt((sumReal*sumReal)+(sumImag*sumImag));

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
/*
  printf("soy la hebra %d.\n", monitor -> id);
  printf("u: %f\n", monitor -> buffer -> u);
  printf("v: %f\n", monitor -> buffer -> v);
  printf("real: %f\n", monitor -> buffer -> real);
  printf("imag: %f\n", monitor -> buffer -> imag);
  printf("ruido: %f\n", monitor -> buffer -> ruido);
*/
  /*Hebra* hebra = (Hebra*) param;

  hebra -> mediaReal = calcularMediaReal(hebra -> dato, hebra -> largo);
  hebra -> mediaImag = calcularMediaImaginaria(hebra -> dato, hebra -> largo);
  hebra -> potenciaTotal = calcularPotenciaTotal(hebra -> dato, hebra -> largo);
  hebra -> ruidoTotal = calcularRuidoTotal(hebra -> dato, hebra -> largo);*/

  //pthread_mutex_lock(&hebra -> mutex);

  //pthread_mutex_unlock(&hebra -> mutex);







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







/*
void* procesarDatosSimples(void* param1)
{

  Hebra* x = (Hebra*) param1;
  int y = x -> y;
  pthread_mutex_lock(&x -> mutex);
  printf("procesando datos\n");
  for (int i = 0; i < 5; i++) {
    printf("%d\n", x -> datos[i]);
  }

  for (int i = 0; i < 5; i++) {
    x -> resultados[i] = x -> datos[i] % y;
  }
  pthread_mutex_unlock(&x -> mutex);

}

void* imprimirDatos(void* param)
{


  Hebra* x = (Hebra*) param;
  pthread_mutex_lock(&x -> mutex);
  printf("imprimiendo datos\n");

  for (int i = 0; i < 5; i++) {
    printf("%d\n", x -> resultados[i]);
  }
  pthread_mutex_unlock(&x -> mutex);

}
*/
