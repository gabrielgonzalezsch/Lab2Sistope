#include "estructuras.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int largoArchivo(char* nombre);

int calcularDisco(double u, double v, double ancho, int c_discos);
void imprimirDatos(Monitor* monitor);

/*
void* procesarDatosSimples(void* param1);

void* imprimirDatos(void* param);
*/
void* procesarDatos(void* param);
