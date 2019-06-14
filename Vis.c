
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "estructuras.h"


//solo se puede recibir un argumento al parecer
void* calcularMediaReal(void* param1, void* param2)
{
  double** datos = (double**) param1;
  int cantidad_datos = (int) param2;

  double resultadoMediaReal = 0;
  for (int i = 0; i < cantidad_datos; i++)
  {
    resultadoMediaReal = datos[i][2] + resultadoMediaReal;
  }

  resultadoMediaReal = resultadoMediaReal/cantidad_datos;
  //return resultadoMediaReal; guardar esto en la estructura de datos
}


void* calcularMediaImaginaria(void* param1, void* param2)
{
  double** datos = (double**) param1;
  int cantidad_datos = (int) param2;

  double resultadoMediaImaginaria = 0;
  for (int i = 0; i < cantidad_datos; i++)
  {
    resultadoMediaImaginaria = datos[i][3] + resultadoMediaImaginaria;
  }

  resultadoMediaImaginaria = resultadoMediaImaginaria/cantidad_datos;
  //return resultadoMediaImaginaria; guardar esto en la estructura de datos
}


void* calcularPotenciaTotal(void* param1, void* param2)
{
  double** datos = (double**) param1;
  int cantidad_datos = (int) param2;

  double resultadoPotenciaTotal = 0;
  for (int i = 0; i < cantidad_datos; i++)
  {
    double potencia = sqrt( (datos[i][2] * datos[i][2]) + (datos[i][3] * datos[i][3]) );
    resultadoPotenciaTotal = potencia + resultadoPotenciaTotal;
  }
  //return resultadoPotenciaTotal; guardar esto en la estructura de datos
}


void* calcularRuidoTotal(void* param1, void* param2)
{
  double** datos = (double**) param1;
  int cantidad_datos = (int) param2;

  double resultadoRuidoTotal = 0;
  for (int i = 0; i < cantidad_datos; i++)
  {
    resultadoRuidoTotal = datos[i][4] + resultadoRuidoTotal;
  }
  //return resultadoRuidoTotal;  guardar esto en la estructura de datos
}


double** agregarEspacio(double** datos, double* nuevo, int largo)
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

// Cambiar este main
int main(){

    double nuevo[6];
    double** datos = (double**) malloc(sizeof(double));
    int largo = 0;
    int flag = 0;

    while (flag == 0)
    {

      //read(STDIN_FILENO, &nuevo, sizeof(nuevo));

      if (nuevo[5] < 0)
      {

        // CALCULAR Y DEVOLVER RESULTADOS
        //######################################################################################
        double resultados[5];

        resultados[0] = 0;
        resultados[1] = 0;
        resultados[2] = 0;
        resultados[3] = 0;
        resultados[4] = 0;

        if (datos != NULL)
        {
          double mediaReal;
          double mediaImaginaria;
          double potenciaTotal;
          double ruidoTotal;
          // Hacer calculos
          mediaReal = calcularMediaReal(datos, largo);
          mediaImaginaria = calcularMediaImaginaria(datos, largo);
          potenciaTotal = calcularPotenciaTotal(datos, largo);
          ruidoTotal = calcularRuidoTotal(datos, largo);
          // Guardar valores en estructura
          resultados[0]  = mediaReal;
          resultados[1]  = mediaImaginaria;
          resultados[2]  = potenciaTotal;
          resultados[3]  = ruidoTotal;
          resultados[4]  = datos[0][5];

          if (nuevo[0] != 0)
          {
            fprintf(stderr, "Soy el hijo de pid %d, procese %d visibilidades\n", getpid(), largo);
          }


          write(STDOUT_FILENO, &resultados, sizeof(resultados));

          flag = 1;
          break;
        }
        else
        {
          fprintf(stderr, "CASO NULO");
          exit(0);
        }

      }
      else
      {
        datos = agregarEspacio(datos, nuevo, largo);
        largo++;
      }

    }

    return 0;
}
