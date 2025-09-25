#include <stdio.h>
#include <stdlib.h>
#include "miembros.h"
#include "validaciones.h"
#include "extraccion_archivo.h"

int main()

{  t_fecha fechaproceso;
   /*
    do{
    printf ("ingrese la fecha proceso (dd-mm-aaaa): \n");
    scanf("%d-%d-%d",&fechaproceso.dia,&fechaproceso.mes,&fechaproceso.anio);
    }while (!validarFecha(fechaproceso));
    */

    printf("Fecha de proceso seteada para pruebas: 07-02-2025 \n");
    fechaproceso.dia=7;
    fechaproceso.mes=2;
    fechaproceso.anio=2025;
    archivo_cargado_validado(fechaproceso);



    return 0;
}

