#ifndef BASEUTILS_H
#define BASEUTILS_H

//#define MAX_ELEMENTOS 10000
#define DEFAULT_MAX 10000
#define ARCHIVO "database.dat"
#define LOG "logUpdates.txt"
#define CONFIG "config.ini"
#define LOTE "lote.txt"

/*Definicion de tipos de datos y prototipos de funciones*/

typedef enum {
    OPERATIVO, REPUESTO, EN_REPARACION
} Estado;

typedef struct {
    int id;
    char nombre[31];
    Estado estado;
    char observaciones[51];
} Elemento;

/* UTILS */
int defineMax(char *);
void inicializarArchivo ();
int cargarVector (Elemento[]);
int validaId (int);
void timer(char, char *);
int miscanf(char, ...);
void parseElemento (Elemento * , char *);

#endif