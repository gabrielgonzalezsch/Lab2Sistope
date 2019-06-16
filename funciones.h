#include "estructuras.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef GLOBAL_VARIABLES
#define GLOBAL_VARIABLES

double** resultados;

#endif

int largoArchivo(char* nombre);

int calcularDisco(double u, double v, double ancho, int c_discos);

void imprimirDatos(Monitor* monitor);

void leerArchivo(Monitor** monitores,char* nombre, int anchoDisco,int c_discos, int tamanioBuffer);

void escribirArchivo(char* nombreSalida, double** resultados, int c_discos);

void* procesarDatos(void* param);

void escribirEnPantalla(double** resultados, int c_discos);
