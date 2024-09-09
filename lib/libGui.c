#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../includes/baseUtils.h"
#include "../includes/gui.h"
#include "../includes/persist.h"

extern char * ESTADOS[];
extern int MAX_ELEMENTOS;
/* ------------------------------------------------------------ */
/** \brief INTERFAZ DEL USUARIO. 
 * \image html menuPrincipal.png
*/
int menuPrincipal(){
    
    char * OPCIONES[] = {"1- AÑADIR ELEMENTO", "2- BORRAR ELEMENTO", "3- ACTUALIZAR ESTADO", "4- BUSCAR ELEMENTO POR ID", 
                        "5- LISTAR PENDIENTES", "6- GENERAR BACKUP CSV", "7- GENERAR INFORME ELEMENTOS PENDIENTE REPARAR",
                        "8 -INGRESO POR LOTE", "0- SALIR"};
    
    int opcion = 0;
    int cantidadOpciones = sizeof(OPCIONES)/sizeof(OPCIONES[0]);
    
    printf("\n\t\t\tGestor de Stock VAIRO2000\n\n\t\t\t**************************\n");
    
    for (int i = 0; i<cantidadOpciones; i++){
        printf("\n\t\t%s \n",OPCIONES[i]);      
    }
    printf("\n -> ");
    miscanf('i', &opcion);
    
    return opcion;
}

/**
 * \image html menuBuscar.png
 */
void menuLeer(){
    
    int id= 0;

    Elemento * elemPtr = (Elemento *) malloc (sizeof(Elemento));
    elemPtr = (Elemento *) memset (elemPtr, 0, sizeof(Elemento));

    printf("\nIntroducir el ID del elemento a buscar:  \n ->");
    miscanf('i', &id);

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

    Elemento * elemPtr = (Elemento *) malloc (sizeof(Elemento));
    elemPtr = (Elemento *) memset (elemPtr, 0, sizeof(Elemento));

    printf("\n-Introducir el ID del elemento a agregar: [1 - %d]-> ", MAX_ELEMENTOS);
    miscanf('i', &id);

    if (validaId(id) != 1){ 
        printf("\nError agregando el elemento ID: %d, ID existente o no valida\n", elemPtr->id);     
    } else {
        elemPtr->id = id;
        
        printf("\n-Ingresa el nombre: -> ");
        miscanf('c',elemPtr->nombre, 30);
        
        printf("\n-Estado: (1-OPERATIVO, 2-REPUESTO, 3-EN REPARACION) ->");
        Estado estado_temporal = 0;
        do {
                miscanf('i', &estado_temporal);
        } while (estado_temporal<1 && estado_temporal>3);
        elemPtr->estado = estado_temporal-1;

        printf("\n-Ingresa las observaciones: -> ");
        
        miscanf('c', elemPtr->observaciones, 50);
        
        guardarElemento(elemPtr);
        printf("\nGuardado exitosamente elemento ID: [%d] \n\n", elemPtr->id);
    }
    free(elemPtr);
}

void menuBorrar(){
    int id = 0;
    
    printf("\nIntroducir el ID del elemento a eliminar:  \n ->");
    miscanf('i', &id);

    if (validaId(id) == 0){ 
        borrarElemento (id);
        printf("\nElemento %d eliminado", id);     
        } else {
        printf("\nError eliminando ID %d, fuera de rango o inexistente", id);
    }
}

void menuEditar(){
    int id = 0;

    printf("\nIntroducir el ID del elemento a editar:  \n ->");
    miscanf('i', &id);

    Elemento * elemPtr = (Elemento *) malloc (sizeof(Elemento));
    elemPtr = (Elemento *) memset (elemPtr, 0, sizeof(Elemento));

    leerElemento(id, elemPtr);

    if (elemPtr->id != 0) {
        printf ("ID: %d\tElemento: %s\tEstado: %u\tObservaciones:%s",elemPtr->id, elemPtr->nombre, elemPtr->estado, elemPtr->observaciones);

        printf("\n\nNombre: -> ");
        miscanf('c', elemPtr->nombre, 30);
        
        printf("\nEstado: (1-OPERATIVO, 2-REPUESTO, 3-EN REPARACION) -> ");
        Estado estado_temporal = 0;
        do {
            miscanf('i', &estado_temporal);
        } while (estado_temporal<1 && estado_temporal>3);
        elemPtr->estado = estado_temporal-1;

        printf("\nObservaciones: -> ");
        miscanf('c', elemPtr->observaciones, 50);
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
    
    printf("\nIntroducir el ID del elemento a actualizar:  \n ->");
    miscanf('i', &id);

    Elemento * elemPtr = (Elemento *) malloc (sizeof(Elemento));
    elemPtr = (Elemento *) memset (elemPtr, 0, sizeof(Elemento));

    leerElemento(id, elemPtr);
    
    if (elemPtr->id != 0) {
        printf ("ID: %d\tElemento: %s\tEstado: %s\tObservaciones:%s",elemPtr->id, elemPtr->nombre, ESTADOS[elemPtr->estado], elemPtr->observaciones);
    
        printf("\nIngrese el nuevo estado: (1-OPERATIVO, 2-REPUESTO, 3-EN REPARACION) -> ");
        
        Estado nuevo_estado = 0;
        Estado viejo_estado = elemPtr->estado;
        do {
            miscanf('i', &nuevo_estado);
        } while (nuevo_estado<1 && nuevo_estado>3);
        elemPtr->estado = nuevo_estado-1;

        printf("\nObservaciones: -> ");
        miscanf('c', elemPtr->observaciones, 50);

        if (guardarElemento(elemPtr) && (viejo_estado != elemPtr->estado)){
            logUpdate(elemPtr, viejo_estado);
        }       
    } else {
        printf ("\nElemento [%d] no existente", id);
    }

    free(elemPtr);
}