#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "estructuras.h"


double calcularMediaReal(Dato** datos, int cantidad_datos)
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
}

void* procesarDatos(void* param)
{


  Hebra* hebra = (Hebra*) param;

  hebra -> mediaReal = calcularMediaReal(hebra -> dato, hebra -> largo);
  hebra -> mediaImag = calcularMediaImaginaria(hebra -> dato, hebra -> largo);
  hebra -> potenciaTotal = calcularPotenciaTotal(hebra -> dato, hebra -> largo);
  hebra -> ruidoTotal = calcularRuidoTotal(hebra -> dato, hebra -> largo);

  pthread_mutex_lock(&hebra -> mutex);

  printf("soy la hebra %d y procese %d datos.\n", hebra -> id, hebra -> largo);

  pthread_mutex_unlock(&hebra -> mutex);
}
