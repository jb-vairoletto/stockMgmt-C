#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../includes/persist.h"

extern char * ESTADOS[];
extern int MAX_ELEMENTOS;

/** \brief Guarda el Elemento elem en la base de datos. 
 * \param Elemento elem: Recibe el elemento a guardar.
 * \return Int : Devuelve 0 en caso de error, 1 en caso de exito.
*/
int guardarElemento (Elemento * elem) {

    FILE *filePointer;
    int retorno = 0;

    if ((filePointer = fopen(ARCHIVO, "r+")) == NULL) {
        printf("Error al sobreescribir archivo");
    } else {
            fseek(filePointer, (sizeof(Elemento)*(elem->id-1)), SEEK_SET);      //Situo puntero en posicion ID-1
            retorno = fwrite(elem, sizeof(Elemento), 1, filePointer);
            fclose(filePointer);
            return retorno;
    }
    return 0;  
}

/** \brief Recibe un ID, y un puntero de Elemento, busca el id en la base y lo devuelve en el puntero aux. 
 * \param Int id : ID del elemento a buscar, Elemento * aux: Puntero del espacio de memoria para el elemento. 
*/
void leerElemento (int id, Elemento * aux){

    FILE *filePointer;
    

    if ((filePointer = fopen(ARCHIVO, "r+")) == NULL){
        printf ("Error al leer archivo");
    } else {
        fseek (filePointer, (sizeof(Elemento)*(id-1)), SEEK_SET);
        fread(aux, sizeof(Elemento), 1, filePointer);           //Lee una estructura Elemento, no chequea el contenido
    }
    fclose(filePointer);
}

/** \brief Borra un elemento de la base de datos. 
 * \param int id : ID del elemento a borrar.
*/
void borrarElemento(int id){
    
    FILE *filePointer;
    Elemento * elem = (Elemento *) malloc (sizeof(Elemento));
    elem = (Elemento *) memset (elem, 0, sizeof(Elemento));

    if ((filePointer = fopen(ARCHIVO, "r+")) == NULL) {
        printf("Error al leer archivo");
        return;
    }

    fseek(filePointer,(sizeof(Elemento)*(id-1)), SEEK_SET);
    fwrite(elem, sizeof(Elemento), 1, filePointer);            //Sobreescribe el Elemento id con uno con todos los miembros seteados en 0

    fclose(filePointer);   
}


void logUpdate(Elemento * elem, Estado viejoEstado){
    FILE * logPtr;
    char * fechahora = malloc (sizeof(char)*60);
    timer('b', fechahora);

    if ((logPtr = fopen(LOG, "a+")) == NULL){
        printf("Error leyendo log de actualizaciones");
    } else {
        fprintf(logPtr,"* %s ----> ID:%d, NOMBRE:%s, ESTADO ACTUALIZADO [de %s a %s] ----> DETALLES: %s\n", fechahora, elem->id, elem->nombre, ESTADOS[viejoEstado], 
                                                                                                            ESTADOS[elem->estado], elem->observaciones );
        fclose(logPtr); 
    }

}

/** \brief Genera un archivo CSV, copia de la base de datos actual. 
 * \image html backupCSV.png
*/
void generaCopiaCSV(){
    char * hh_mm_ss = (char *) malloc (20*sizeof(char));
    hh_mm_ss = (char *) memset (hh_mm_ss, 0, 20);
    timer('h', hh_mm_ss);
    hh_mm_ss = strcat(hh_mm_ss,"- COPIA BDD.csv");      //string con nombre completo de archivo 
    
    FILE *writer = fopen(hh_mm_ss, "w");
    Elemento vecElementos[MAX_ELEMENTOS];
    int cantidad = cargarVector (vecElementos); 

    for (int i = 0; i < cantidad; i++){
        if (vecElementos[i].id != 0) {
            fprintf(writer,"%d, %s, %s, %s\n",vecElementos[i].id, vecElementos[i].nombre, ESTADOS[vecElementos[i].estado],vecElementos[i].observaciones);
        }
    }
    free (hh_mm_ss);
    fclose(writer);
}

/** \brief Genera un Informe con los elementos pendientes de reparacion. 
 * \image html informePendientes.png
*/
void generarInformePendientes(){
    
    char * dia = (char *) malloc (20*sizeof(char));
    char * nombreArchivo = (char *) malloc (52*sizeof(char));
    
    dia = (char *) memset (dia, 0, 20);
    nombreArchivo = (char *) memset (nombreArchivo, 0, 52);

    timer('d', dia);        //String para usar dentro del informe

    nombreArchivo = strcpy(nombreArchivo, dia);
    strcat(nombreArchivo, " - INFORME PENDIENTES DE REPARACION.TXT");       //Nombre completo de archivo 
    
    FILE *writer = fopen(nombreArchivo, "w");
    Elemento vecElementos[MAX_ELEMENTOS];
    int cantidad = cargarVector (vecElementos);

    fprintf(writer, "Siendo el dia %s, se procede a listar los elementos pendientes de reparacion: \n", dia);
    for (int i = 0; i < cantidad; i++){
        if (vecElementos[i].id != 0 && vecElementos[i].estado == 2) {
            fprintf(writer,"\t**%d, %s, %s\n",vecElementos[i].id, vecElementos[i].nombre, vecElementos[i].observaciones);
        }
    }
    free (dia);
    free (nombreArchivo);

    fclose(writer);
}
