#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

#include "../includes/baseUtils.h"
char * ESTADOS[] = {"OPERATIVO", "REPUESTO", "EN REPARACION"};


/** \brief Si el archivo no existe, lo crea con la totalidad de los registros para acceso directo. */  
void inicializarArchivo(){  
    FILE *filePointer;
    Elemento temp = {0, "", 0, ""};
    if ((filePointer = fopen(ARCHIVO, "r")) == NULL){
            filePointer = fopen(ARCHIVO, "w");                          //Si no existe el archivo, lo creo
            for (int i= 0; i< MAX_ELEMENTOS; i++){
                fwrite(&temp, sizeof(Elemento), 1, filePointer);        //Rellena el archivo para su posterior acceso directo
            }
    } 
    fclose (filePointer);
}

/** \brief Vuelca la base de datos al vector pasado por referencia. 
 * 
 * \param Elemento elem[] : Puntero del vector a cargar con los elementos del archivo.
 * \return Int : En caso satisfactorio, cantidad de elementos cargados, devuelve -1 en caso de error. 
*/
int cargarVector(Elemento elem[]){
    FILE *filePointer;
    int i = 0, retorno = 0;

    if ((filePointer = fopen(ARCHIVO, "rb")) == NULL){
            printf("Error leyendo archivo.");
            return -1;
    } else {
        while (!feof(filePointer)){
            if(fread(&elem[i], sizeof(Elemento), 1, filePointer)) retorno++;        //Bucle hasta que llego al EOF
            i++;
        }
    }
    fclose (filePointer);
    return retorno;
}


/** \brief Valida si una ID existe en la base o no, recibe una id por parametro, y devuelve:  
 * \param Int id : ID a validar si existe o no.
 * \return Int : -1: Error de lectura, 0: ID existente 1: ID válida.
*/

int validaId(int id){
    
    FILE *filePointer;
    Elemento aux = {0, "", 0, ""};

    if(id<=0 || id>MAX_ELEMENTOS) return -1;

    if ((filePointer = fopen(ARCHIVO, "r+")) == NULL){
        printf ("Error al leer archivo");
        return -1;
    }

    fseek (filePointer, (sizeof(Elemento)*(id-1)), SEEK_SET);
    fread(&aux, sizeof(Elemento), 1, filePointer);

    if (aux.id == 0) return 1;  //Chequea si el elemento leido tiene id=0(LIBRE) o id!=0(OCUPADO)
    return 0;
}

/** \brief Genera Strings con la fecha/hora actual. 
 * \param char * tiempo : puntero string con el espacio de memoria, char * mode : Opcion deseada: 'h':hora, 'd':dia, 'b': ambas.
*/
void timer(char mode, char * tiempo){
   
   time_t now = time(0);
   struct  tm * timeinfo;

   time (&now);
   timeinfo = localtime (&now);     //Estructura con informacion de fecha y hora actual;
   
   if (mode == 'h') {               //Segun modo deseado devuelve string con informacion
        snprintf(tiempo, 9*sizeof(tiempo),"%d_%d_%d", timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec);
   }
   if (mode == 'd') {
        snprintf(tiempo, 13*sizeof(char), "%d-%d-%d", timeinfo->tm_mday,timeinfo->tm_mon + 1, timeinfo->tm_year +1900);
   }
   if (mode == 'b') {
        snprintf(tiempo, 22*sizeof(char), "%d-%d-%d__%d_%d_%d", timeinfo->tm_mday,timeinfo->tm_mon + 1, timeinfo->tm_year +1900, timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec);
   }

}
