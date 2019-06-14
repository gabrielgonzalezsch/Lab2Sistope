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

double** agregarEspacio(double** datos,double* nuevo, int largo)
{
  double** nuevaLista = (double**) malloc(sizeof(double) * (largo +1 ));

  for (int i = 0; i < largo+1; i++)
  {
    nuevaLista[i] = (double*) malloc(sizeof(double) * 6);
  }

  int i = 0;
  while (i < largo)
  {
    nuevaLista[i][0] = datos[i][0];
    nuevaLista[i][1] = datos[i][1];
    nuevaLista[i][2] = datos[i][2];
    nuevaLista[i][3] = datos[i][3];
    nuevaLista[i][4] = datos[i][4];
    nuevaLista[i][5] = datos[i][5];
    i++;
  }

  nuevaLista[i][0] = nuevo[0];
  nuevaLista[i][1] = nuevo[1];
  nuevaLista[i][2] = nuevo[2];
  nuevaLista[i][3] = nuevo[3];
  nuevaLista[i][4] = nuevo[4];
  nuevaLista[i][5] = nuevo[5];

  free(datos);

  return nuevaLista;
}

void* procesarDatos(void* param){

    Hebra* hebra = (Hebra*) param;

    hebra -> mediaReal = calcularMediaReal(hebra -> dato, hebra -> largo);
    hebra -> mediaImag = calcularMediaImaginaria(hebra -> dato, hebra -> largo);
    hebra -> potenciaTotal = calcularPotenciaTotal(hebra -> dato, hebra -> largo);
    hebra -> ruidoTotal = calcularRuidoTotal(hebra -> dato, hebra -> largo);

    return 0;
}
