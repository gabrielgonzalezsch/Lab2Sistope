#include "estructuras.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int largoArchivo(char* nombre);

double calcularDisco(int u, int v);

double** leerDatos(char* nombre,int largo);

Dato** agregarEspacio(Dato** datoHebras, int largoActual, double* nuevoDato);

Hebra** repartirDatos(Hebra** hebras, double** datos, int largo, int c_discos, int anchoDisco);
/*
void* procesarDatosSimples(void* param1);

void* imprimirDatos(void* param);
*/
void* procesarDatos(void* param);
