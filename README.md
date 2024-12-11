# Gestión de Stock de Materiales

Esta aplicación en C permite gestionar un inventario de materiales, registrar su estado (operativo, en reparación, repuesto), y generar informes y copias de seguridad. Es ideal para un sistema de mantenimiento que necesita seguimiento detallado de los elementos.

## Funcionalidades

1. **Agregar Materiales:**
   - Registra nuevos materiales con un ID único, nombre, estado y observaciones.

2. **Eliminar Materiales:**
   - Permite borrar materiales del sistema.

3. **Modificar Estado de Materiales:**
   - Actualiza el estado de los materiales (operativo, en reparación, repuesto) y añade observaciones.

4. **Generar Informe de Materiales en Reparación:**
   - Crea un archivo de texto con los detalles de los materiales en reparación, útil para su uso en correos electrónicos.

5. **Búsqueda de Materiales por ID:**
   - Permite consultar información de un material específico ingresando su ID.

6. **Generación de Copia de Seguridad en CSV:**
   - Exporta los datos a un archivo CSV para respaldo o análisis externo.

7. **Log de Actualizaciones con Fecha y Hora:**
   - Registra todas las modificaciones realizadas sobre los materiales para auditorías. (logUpdates.txt)


## Compilación y Ejecución

Puedes compilar el proyecto usando GCC y ejecutar el programa desde la terminal. Aquí hay un ejemplo de cómo hacerlo:

```bash

gcc -Wall -o gestor main.c -I ./includes ./lib/libGui.c ./lib/libPersist.c ./lib/libBaseUtils.c

./gestor

```

Para configurar cantidad de materiales editar config.ini, no tiene que haber database.dat creado.

Para ingreso de materiales por lote, revisar formato en muestra-lote.txt : 
ID,NOMBRE_ELEMENTO,ESTADO(0=OPERATIVO,1=REPUESTO,2=EN_REPARACION),OBSERVACIONES
Y guardar en un archivo que se llame lote.txt

