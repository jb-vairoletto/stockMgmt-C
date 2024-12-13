#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "stdarg.h"

#include "../includes/baseUtils.h"
char *ESTADOS[] = {"OPERATIVO", "REPUESTO", "EN REPARACION"};
int MAX_ELEMENTOS;

/** \brief Si no existe database, seteo el max_elements segun config.ini . */

int defineMax(char *configFile)
{
    int max = DEFAULT_MAX;
    FILE *configPtr;

    if ((configPtr = fopen(configFile, "r+")) == NULL)
    {
        printf("error lectura config.ini");

        // fclose(configPtr);
        return max;
    }

    char *buf = (char *)malloc(sizeof(char) * 50);
    buf = (char *)memset(buf, 0, 50);
    fread(buf, sizeof(char), 50, configPtr);
    char *strPtr = strtok(buf, "=");

    if (strcmp(strPtr, "MAX_ELEMENTS") == 0)
    {
        strPtr = strtok(NULL, "=");
        int max = atoi(strPtr);

        if (max > 0)
        {
            free(buf);

            fclose(configPtr);
            return max;
        }
    }
    free(buf);
    fclose(configPtr);
    return DEFAULT_MAX;
}

/** \brief Si el archivo no existe, lo crea con la totalidad de los registros para acceso directo. */
void inicializarArchivo()
{
    FILE *filePointer;

    Elemento *temp = (Elemento *)malloc(sizeof(Elemento));
    temp = (Elemento *)memset(temp, 0, sizeof(Elemento));

    if ((filePointer = fopen(ARCHIVO, "r")) == NULL)
    {
        filePointer = fopen(ARCHIVO, "w");
        MAX_ELEMENTOS = defineMax(CONFIG);
        // Si no existe el archivo, lo creo
        for (int i = 0; i < MAX_ELEMENTOS; i++)
        {
            fwrite(temp, sizeof(Elemento), 1, filePointer); // Rellena el archivo para su posterior acceso directo
        }
    }
    else
    {
        int i = 0;
        filePointer = fopen(ARCHIVO, "r");
        fseek(filePointer, 0, SEEK_SET);
        while (!feof(filePointer))
        {
            fread(temp, sizeof(Elemento), 1, filePointer); // Bucle hasta que llego al EOF
            i++;
        }
        MAX_ELEMENTOS = i - 1;
    }
    printf("CANTIDAD MAX ELEMENTOS: %d\n", MAX_ELEMENTOS);
    fclose(filePointer);
}

/** \brief Vuelca la base de datos al vector pasado por referencia.
 *
 * \param Elemento elem[] : Puntero del vector a cargar con los elementos del archivo.
 * \return Int : En caso satisfactorio, cantidad de elementos cargados, devuelve -1 en caso de error.
 */
int cargarVector(Elemento elem[])
{
    FILE *filePointer;
    int i = 0, retorno = 0;

    if ((filePointer = fopen(ARCHIVO, "rb")) == NULL)
    {
        printf("Error leyendo archivo.");
        return -1;
    }
    else
    {
        while (!feof(filePointer))
        {
            if (fread(&elem[i], sizeof(Elemento), 1, filePointer))
                retorno++; // Bucle hasta que llego al EOF
            i++;
        }
    }
    fclose(filePointer);
    return retorno;
}

/** \brief Valida si una ID existe en la base o no, recibe una id por parametro, y devuelve:
 * \param Int id : ID a validar si existe o no.
 * \return Int : -1: Error de lectura, 0: ID existente 1: ID válida.
 */

int validaId(int id)
{

    FILE *filePointer;
    Elemento *temp = (Elemento *)malloc(sizeof(Elemento));
    temp = (Elemento *)memset(temp, 0, sizeof(Elemento));

    if (id <= 0 || id > MAX_ELEMENTOS)
        return -1;

    if ((filePointer = fopen(ARCHIVO, "r+")) == NULL)
    {
        printf("Error al leer archivo");
        return -1;
    }
    fseek(filePointer, (sizeof(Elemento) * (id - 1)), SEEK_SET);
    fread(temp, sizeof(Elemento), 1, filePointer);

    if (temp->id == 0)
    {
        free(temp);
        return 1; // Chequea si el elemento leido tiene id=0(LIBRE) o id!=0(OCUPADO)
    }
    free(temp);
    return 0;
}

/** \brief Genera Strings con la fecha/hora actual.
 * \param tiempo : puntero string con el espacio de memoria,
 * \param mode : Opcion deseada: 'h':hora, 'd':dia, 'b': ambas.
 */
void timer(char mode, char *tiempo)
{

    time_t now = time(0);
    struct tm *timeinfo;

    time(&now);
    timeinfo = localtime(&now); // Estructura con informacion de fecha y hora actual;

    if (mode == 'h')
    { // Segun modo deseado devuelve string con informacion
        snprintf(tiempo, 9 * sizeof(tiempo), "%d_%d_%d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    }
    if (mode == 'd')
    {
        snprintf(tiempo, 13 * sizeof(char), "%d-%d-%d", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
    }
    if (mode == 'b')
    {
        snprintf(tiempo, 22 * sizeof(char), "%d-%d-%d__%d_%d_%d", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    }
}

/** \brief Funcion de ingreso de datos string(permite espacios en blanco) y enteros.
 * \param char * tipo : 'c' : para el ingreso de string, 'i': para el ingreso de enteros.
 * \param void * segun modo elegido puntero correspondiente
 * \param limite int: en caso de string : limite de caracteres
 * \return int: 0: Error, !=0: Cantidad de caracteres o digitos.
 */
int miscanf(char tipo, ...)
{
    va_list ap;
    va_start(ap, tipo);
    char caracter = 0;

    switch (tipo)
    {
    case 'c':
        char *ptr = va_arg(ap, char *);
        int limite = va_arg(ap, int);

        int contador = 0;

        while ((caracter = getchar()) != '\n' && contador < limite)
        {
            *(ptr + contador) = caracter;
            contador++;
        }
        *(ptr + contador) = 0;
        if (caracter != '\n')
        {
            while ((caracter = getchar()) != '\n')
            {
            } // flush
        }
        return contador;

    case 'i':
        int *i_ptr = va_arg(ap, int *);

        char *buffer = malloc(sizeof(char) * 12); // –2147483648 a 2147483647 mas nulo final;
        buffer = memset(buffer, 0, 12);

        int digito = 0;

        caracter = getchar();
        while (caracter != '\n' && digito < 12)
        {
            buffer[digito] = caracter;
            digito++;
            caracter = getchar();
        }
        if (digito == 12)
        {
            while ((caracter = getchar()) != '\n')
            {
            }
        }

        *i_ptr = atoi(buffer);

        free(buffer);
        if ((*i_ptr))
            return digito;
        else
            return 0;

    default:
        return 0;
    }
}

void parseElemento(Elemento *temp, char *cadena)
{

    char *strPtr = strtok(cadena, ",");
    int id = atoi(strPtr);

    if (id != 0)
    {
        cadena = strtok(NULL, ",");
        char *nombre = (char *)malloc(sizeof(char) * 31);
        memset(nombre, 0, sizeof(*nombre));
        strcpy(nombre, cadena);

        cadena = strtok(NULL, ",");
        char *estado = (char *)malloc(sizeof(char));
        memset(estado, 0, sizeof(*estado));
        strcpy(estado, cadena);

        cadena = strtok(NULL, ",");
        char *observaciones = (char *)malloc(sizeof(char) * 51);
        memset(observaciones, 0, sizeof(*observaciones));
        strcpy(observaciones, cadena);

        // Aca elimino enter final
        observaciones[strlen(observaciones) - 1] = '\0';

        if (nombre[0] != '\0' && observaciones[0] != '\0')
        {
            if (*estado >= '0' && *estado <= '3')
            {
                strcpy(temp->nombre, nombre);
                strcpy(temp->observaciones, observaciones);
                temp->estado = *estado - '0';
                temp->id = id;
                printf("\nPROCESANDO:\t %-10d %-30s %-15s %s", temp->id, temp->nombre, ESTADOS[temp->estado], temp->observaciones);
            }
        }
        free(nombre);
        free(estado);
        free(observaciones);
    }
}

int cmpNombre(const void *a, const void *b)
{
    Elemento *elem1 = (Elemento *)a;
    Elemento *elem2 = (Elemento *)b;

    char *_elem1 = elem1->nombre;
    char *_elem2 = elem2->nombre;

    int cantidad = 0;
    int len1 = strlen(_elem1);
    int len2 = strlen(_elem2);

    cantidad = len1 > len2 ? len2 : len1;

    for (int i = 0; i < cantidad; i++)
    {
        if (_elem1[i] > _elem2[i])
            return 1;
        if (_elem2[i] > _elem1[i])
            return -1;
    }
    return len1 - len2; // 0 si las cadenas son igual de largas, sino resultado
}

int cmpEstado(const void *a, const void *b)
{
    Elemento *elem1 = (Elemento *)a;
    Elemento *elem2 = (Elemento *)b;
    return elem1->estado - elem2->estado;
}
