#include "stdio.h"
#include "stdlib.h"
#include "includes/baseUtils.h"
#include "includes/gui.h"
#include "includes/persist.h"
#include "includes/stack.h"
int main(int argc, char const *argv[])
{
    int cant = 0;
    StackNodePtr *stackPointer = (StackNodePtr *)malloc(sizeof(StackNodePtr));
    inicializarArchivo();
    char flag = 'v';
    do
    {
        int opcion = menuPrincipal();

        switch (opcion)
        {
        case 0:
            flag = 'f';
            break;
        case 1:
            menuAgregar(stackPointer);
            printf("STACKPOINTER: [%s]", (*stackPointer)->nombre);
            break;
        case 2:
            menuBorrar(stackPointer);
            printf("STACKPOINTER: [%s]", (*stackPointer)->nombre);
            break;
        case 3:
            actualizarEstado(stackPointer);
            printf("STACKPOINTER: [%s]", (*stackPointer)->nombre);
            break;
        case 4:
            menuLeer();
            break;
        case 5:
            menuPendientes();
            break;
        case 6:
            generaCopiaCSV();
            break;
        case 7:
            generarInformePendientes();
            break;
        case 8:
            cant = ingresoPorLote();
            printf("\nSe ingresaron con exito %d registtos a la base de datos.\n", cant);
            break;
        case 9:
            menuOrdenar();
            break;
        default:
            printf("\nOpcion Incorrecta \n");
        }
    } while ((flag == 'v'));

    return 0;
}
