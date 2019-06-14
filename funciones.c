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
double calcularDisco(int u, int v)
{
  return (sqrt(u*u+v*v));
}

double** leerDatos(char* nombre,int largo)
{

    FILE* archivo = fopen(nombre,"r");

    int cont = 0;
    double u;
    double v;
    double real;
    double imag;
    double ruido;
    double disco;
    char c1,c2,c3,c4;
    double** datos = (double**) malloc(sizeof(double) * largo);

    for (int i = 0; i < largo; i++) {
      datos[i] = (double*) malloc(sizeof(double) * 5);
    }

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

        datos[cont][0] = u;
        datos[cont][1] = v;
        datos[cont][2] = real;
        datos[cont][3] = imag;
        datos[cont][4] = ruido;
        cont++;

    }

    return datos;

}

Dato** agregarEspacio(Dato** datoHebras, int largoActual, double* nuevoDato) //verificar punteros
{
  Dato** newDatos = (Dato**) malloc(sizeof(Dato*) * (largoActual + 1));
  for (int i = 0; i < (largoActual + 1); i++)
  {
    newDatos[i] = (Dato*) malloc(sizeof(Dato));
  }

  int i = 0;
  while(i < largoActual)
  {
    newDatos[i] -> u = datoHebras[i] -> u;
    newDatos[i] -> v = datoHebras[i] -> v;
    newDatos[i] -> real = datoHebras[i] -> real;
    newDatos[i] -> imag = datoHebras[i] -> imag;
    newDatos[i] -> ruido = datoHebras[i] -> ruido;
    /*
    printf("##############\n");
    printf("dato: %d\n", i);
    printf("u: %f\n", newDatos[i] -> u);
    printf("v: %f\n", newDatos[i] -> v);
    printf("real: %f\n", newDatos[i] -> real);
    printf("imag: %f\n", newDatos[i] -> imag);
    printf("ruido: %f\n", newDatos[i] -> ruido);
    printf("##############\n");
    */s
    i++;

  }

  newDatos[i] -> u = nuevoDato[0];
  newDatos[i] -> v = nuevoDato[1];
  newDatos[i] -> real = nuevoDato[2];
  newDatos[i] -> imag = nuevoDato[3];
  newDatos[i] -> ruido = nuevoDato[4];

  return newDatos;
}

Hebra** repartirDatos(Hebra** hebras, double** datos, int largo, int c_discos, int anchoDisco)
{
  double disco;
  int posicion = -1;
  for (int i = 0; i < largo; i++)
  {
    disco = calcularDisco(datos[i][0], datos[i][1]);
    posicion = (int) trunc(disco / anchoDisco);

    if (posicion >= c_discos)
    {
      posicion = c_discos - 1;
    }

    hebras[posicion] -> dato = agregarEspacio(hebras[posicion] -> dato, hebras[posicion] -> largo, datos[i]);
    hebras[posicion] -> largo ++;
  }
}

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
