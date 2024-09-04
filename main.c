#include "stdio.h"

#include "includes/baseUtils.h"
#include "includes/gui.h"
#include "includes/persist.h"

int main(int argc, char const *argv[])
{   
    inicializarArchivo();
    

    char flag = 'v';
    do {
        int opcion = menuPrincipal();
        switch (opcion)
        {
        case 0:
            flag = 'f';
            break;
        case 1: 
            menuAgregar();
            break;
        case 2:
            menuBorrar();
            break;
        case 3:
            actualizarEstado();
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
        default:
            printf("\nOpcion Incorrecta \n");
        }
    } while ((flag == 'v'));
    
    return 0;
    
}

