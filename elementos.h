#ifndef ELEMENTOS_H
#define ELEMENTOS_H

#define MAX_ELEMENTOS 10000
#define ARCHIVO "database.dat"
#define LOG "logUpdates.txt"

/*Definicion de tipos de datos y prototipos de funciones*/

typedef enum {
    OPERATIVO, REPUESTO, EN_REPARACION
} Estado;

typedef struct
{
    int id;
    char nombre[30];
    Estado estado;
    char observaciones[50];
} Elemento;

void inicializarArchivo ();
int guardarElemento (Elemento *);
void leerElemento (int, Elemento*);
void borrarElemento(int);
int cargarVector (Elemento[]);
void generarInformePendientes();
void generaCopiaCSV();
int validaId (int);
void timer(char, char *);
void logUpdate(Elemento *, Estado);

int menuPrincipal();
void menuBorrar();
void menuEditar();
void menuAgregar();
void menuLeer();
void menuPendientes();
void actualizarEstado ();

#endif