#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "elementos.h"

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

/** \brief Recibe un ID, y un puntero de Elemento, busca el id en la base y lo devuelve en el puntero aux, pendiente manejo de errores. 
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

/** \brief Borra un elemento de la base de datos, pendiente manejo de errores. 
 * \param int id : ID del elemento a borrar.
*/
void borrarElemento(int id){
    
    FILE *filePointer;
    Elemento elem = {0,"",0,""};

    if ((filePointer = fopen(ARCHIVO, "r+")) == NULL) {
        printf("Error al leer archivo");
        return;
    }

    fseek(filePointer,(sizeof(Elemento)*(id-1)), SEEK_SET);
    fwrite(&elem, sizeof(Elemento), 1, filePointer);            //Sobreescribe el Elemento id con uno con todos los miembros seteados en 0

    fclose(filePointer);   
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

/** \brief Log de actualizaciones de estado para casos de auditoria//Log de actualizaciones de estado. 
 * \image html logActualizaciones.png
*/
void logUpdate(Elemento * elem, Estado viejoEstado){
    FILE * logPtr;
    char * fechahora = malloc (sizeof(char)*60);
    timer('b', fechahora);
    char * ESTADOS[] = {"OPERATIVO", "REPUESTO", "EN REPARACION"};

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
    char * ESTADOS[] = {"OPERATIVO", "REPUESTO", "EN REPARACION"};
    
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

/* ------------------------------------------------------------ */
/** \brief INTERFAZ DEL USUARIO. 
 * \image html menuPrincipal.png
*/
int menuPrincipal(){
    
    char * OPCIONES[] = {"1- AÑADIR ELEMENTO", "2- BORRAR ELEMENTO", "3- ACTUALIZAR ESTADO", "4- BUSCAR ELEMENTO POR ID", 
                        "5- LISTAR PENDIENTES", "6- GENERAR BACKUP CSV", "7- GENERAR INFORME ELEMENTOS PENDIENTE REPARAR",
                        "0- SALIR"};
    char tempChar[50] = "";
    int opcion = 0;
    int cantidadOpciones = sizeof(OPCIONES)/sizeof(OPCIONES[0]);

    printf("\n\t\t\tGestor de Stock\n\n\t\t\t****************\n");
    
    for (int i = 0; i<cantidadOpciones; i++){
        printf("\n\t\t%s \n",OPCIONES[i]);      
    }
    printf("\n -> ");
    do {
        fgets(tempChar, 5, stdin);
        sscanf(tempChar, "%d", &opcion);
    } while ( opcion >= cantidadOpciones);

    return opcion;
}

/**
 * \image html menuBuscar.png
 */
void menuLeer(){
    char * ESTADOS[] = {"OPERATIVO", "REPUESTO", "EN REPARACION"};
    char tempChar[50] = "";
    int id= 0;
    Elemento * elemPtr = (Elemento *) malloc (sizeof(Elemento));
    elemPtr = (Elemento *) memset (elemPtr, 0, sizeof(Elemento));

    printf("\nIntroducir el ID del elemento a buscar:  \n ->");
    fgets(tempChar, sizeof(tempChar), stdin);
    sscanf(tempChar, "%d", &id);

    leerElemento(id, elemPtr);

    if (elemPtr->id != 0) {
        printf ("\nID: %d\tElemento: %s\tEstado: %s\tObservaciones:%s\n",elemPtr->id, elemPtr->nombre, ESTADOS[elemPtr->estado], elemPtr->observaciones);
    } else {
        printf ("\nError leyendo ID [%d], inexistente o no valido\n", id);
    }
    free(elemPtr);
}

/**
 * \image html menuAgregar.png
 */
void menuAgregar(){
    int id = 0;
    char tempChar[50] = "";

    Elemento * elemPtr = (Elemento *) malloc (sizeof(Elemento));
    elemPtr = (Elemento *) memset (elemPtr, 0, sizeof(Elemento));

    printf("\n-Introducir el ID del elemento a agregar: [0 - %d]-> ", MAX_ELEMENTOS);
    fgets(tempChar, sizeof(tempChar), stdin);
    sscanf(tempChar, "%d", &(elemPtr->id));

    if (validaId(elemPtr->id) != 1){ 
        printf("\nError agregando el elemento ID: %d, ID existente o no valida\n", elemPtr->id);     
    } else {
        printf("\n-Ingresa el nombre: -> ");
        fgets(elemPtr->nombre, sizeof(elemPtr->nombre), stdin);
        elemPtr->nombre[strcspn(elemPtr->nombre, "\n")] = 0; //remueve el enter final
        printf("\n-Estado: (1-OPERATIVO, 2-REPUESTO, 3-EN REPARACION) ->");
        int estado_temporal = 0;
        do {
                fgets(tempChar, sizeof(tempChar), stdin);
                sscanf(tempChar, "%d", &estado_temporal);

        } while (estado_temporal<1 && estado_temporal>3);
        elemPtr->estado = estado_temporal-1;

        printf("\n-Ingresa las observaciones: -> ");
        fgets(elemPtr->observaciones, sizeof(elemPtr->observaciones), stdin);
        elemPtr->observaciones[strcspn(elemPtr->observaciones, "\n")] = 0; //remueve el enter final
        guardarElemento(elemPtr);
        printf("\nGuardado exitosamente elemento ID: [%d] \n\n", elemPtr->id);

    }
    free(elemPtr);
}

void menuBorrar(){
    int id = 0;
    char tempChar[50] = "";

    printf("\nIntroducir el ID del elemento a eliminar:  \n ->");
    fgets(tempChar, sizeof(tempChar), stdin);
    sscanf(tempChar, "%d", &id);

    if (validaId(id) == 0){ 
        borrarElemento (id);
        printf("\nElemento %d eliminado", id);     
        } else {
        printf("\nError eliminando ID %d, fuera de rango o inexistente", id);
    }
}

void menuEditar(){
    int id = 0;
    char tempChar[50] = "";

    printf("\nIntroducir el ID del elemento a editar:  \n ->");
    fgets(tempChar, sizeof(tempChar), stdin);
    sscanf(tempChar, "%d", &id);

    Elemento * elemPtr = (Elemento *) malloc (sizeof(Elemento));
    elemPtr = (Elemento *) memset (elemPtr, 0, sizeof(Elemento));

    leerElemento(id, elemPtr);

    if (elemPtr->id != 0) {
        printf ("ID: %d\tElemento: %s\tEstado: %u\tObservaciones:%s",elemPtr->id, elemPtr->nombre, elemPtr->estado, elemPtr->observaciones);

        printf("\n\nNombre: -> ");
        fgets(elemPtr->nombre, sizeof(elemPtr->nombre), stdin);
        elemPtr->nombre[strcspn(elemPtr->nombre, "\n")] = 0; //remueve el enter final
        
        printf("\nEstado: (1-OPERATIVO, 2-REPUESTO, 3-EN REPARACION) -> ");
        int estado_temporal = 0;
        do {
            fgets(tempChar, sizeof(tempChar), stdin);
            sscanf(tempChar, "%d", &estado_temporal);
        } while (estado_temporal<1 && estado_temporal>3);
        elemPtr->estado = estado_temporal-1;

        printf("\nObservaciones: -> ");
        fgets(elemPtr->observaciones, sizeof(elemPtr->observaciones), stdin);
        elemPtr->observaciones[strcspn(elemPtr->observaciones, "\n")] = 0; //remueve el enter final

        guardarElemento(elemPtr);        
    } else {
        printf ("\nElemento [%d] no existente", id);
    }
    free(elemPtr);
}

/**
 * \image html menuListarPendientes.png
 */
void menuPendientes(){
    Elemento vecElementos[MAX_ELEMENTOS];
    Elemento elementoTemp = {0, "", 0, ""};

    int cantidad = cargarVector(vecElementos);
    if (cantidad>0) {
        printf("\n\tLISTA PENDIENTES: \n\t***************\n");
        printf("%-5s%-20s%-30s\n\n","ID","NOMBRE","OBSERVACIONES");
        for (int i=0 ; i<cantidad; i++){
            if (vecElementos[i].id != 0 && vecElementos[i].estado == 2){
                printf("%-5d", vecElementos[i].id);
                printf("%-20s", vecElementos[i].nombre);
                printf("%-30s\n", vecElementos[i].observaciones);
            }
        }
    }

}

/**
 * \image html menuActualizoEstado.png
 */
void actualizarEstado(){
    int id = 0;
    char tempChar[50] = "";
    char * ESTADOS[] = {"OPERATIVO", "REPUESTO", "EN REPARACION"};

    printf("\nIntroducir el ID del elemento a actualizar:  \n ->");
    fgets(tempChar, sizeof(tempChar), stdin);
    sscanf(tempChar, "%d", &id);

    Elemento * elemPtr = (Elemento *) malloc (sizeof(Elemento));
    elemPtr = (Elemento *) memset (elemPtr, 0, sizeof(Elemento));

    leerElemento(id, elemPtr);
    
    if (elemPtr->id != 0) {
        printf ("ID: %d\tElemento: %s\tEstado: %s\tObservaciones:%s",elemPtr->id, elemPtr->nombre, ESTADOS[elemPtr->estado], elemPtr->observaciones);
    
        printf("\nIngrese el nuevo estado: (1-OPERATIVO, 2-REPUESTO, 3-EN REPARACION) -> ");
        
        Estado nuevo_estado = 0;
        Estado viejo_estado = elemPtr->estado;
        do {
            fgets(tempChar, sizeof(tempChar), stdin);
            sscanf(tempChar, "%u", &nuevo_estado);
        } while (nuevo_estado<1 && nuevo_estado>3);
        elemPtr->estado = nuevo_estado-1;

        printf("\nObservaciones: -> ");
        fgets(elemPtr->observaciones, sizeof(elemPtr->observaciones), stdin);
        elemPtr->observaciones[strcspn(elemPtr->observaciones, "\n")] = 0; //remueve el enter final

        if (guardarElemento(elemPtr) && (viejo_estado != elemPtr->estado)){
            logUpdate(elemPtr, viejo_estado);
        }       
    } else {
        printf ("\nElemento [%d] no existente", id);
    }

    free(elemPtr);
}