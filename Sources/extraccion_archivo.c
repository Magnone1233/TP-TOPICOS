#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miembros.h"
#include "extraccion_archivo.h"
#include "validaciones.h"
//#include <direct.h>  // para _getcwd

int cargar_miembro_desde_texto(const char *linea, t_persona *m, t_fecha fechaproceso,FILE *pfErr, int nroRegistro)
{
    char fechaNac[20], fechaAfi[20], fechaUltCuota[20];
    char emailtutortemp[30]="N/A"; // LE DOY UN VALOR POR DEFECTO


           //aca abajo voy a parsear la linea con sscanf usando |
     int cant= sscanf(linea,"%ld|%60[^|]|%19[^|]| %c|%19[^|]|%9[^|]|%19[^|]| %c|%9[^|]|%29[^\n]",&m->dni,m->nomyape,fechaNac,&m->sexo,fechaAfi,m->categoria,fechaUltCuota,&m->estado,m->plan,
        emailtutortemp);

        if (cant < 9) {
        fprintf(pfErr, "Registro %d: Campos insuficientes\n", nroRegistro);
        return 0;
    }


    mistrcpy(m->emailTutor,emailtutortemp);

    limpiarSaltoLinea(m->nomyape);    //esto de los saltos de linea lo hago porque me estaba tirando todo mal por culpa de un espacio
    limpiarSaltoLinea(m->categoria);
    limpiarSaltoLinea(m->plan);
    limpiarSaltoLinea(m->emailTutor);

    // abajo arranco a normalizar si el archivo no tiene errores
    //VALIDO EL NOMBRE Y APELLIDO
    normalizarcad(m->nomyape);
    //VALIDO EL PLAN
    m->plan[0] = aMayuscula(m->plan[0]);
    for (int i = 1; m->plan[i]; i++)
    {
        m->plan[i] = aMayuscula(m->plan[i]);
    }

    // VALIDO EL ESTADO

    m->estado=aMayuscula(m->estado);

    //aca paso las fechas a enteros

    sscanf(fechaNac, "%d/%d/%d", &m->fechaNacimiento.dia, &m->fechaNacimiento.mes, &m->fechaNacimiento.anio);
    sscanf(fechaAfi, "%d/%d/%d", &m->fechaAfiliacion.dia, &m->fechaAfiliacion.mes, &m->fechaAfiliacion.anio);
    sscanf(fechaUltCuota, "%d/%d/%d", &m->fechaUltimaCuota.dia, &m->fechaUltimaCuota.mes, &m->fechaUltimaCuota.anio);

    int valido =1;

    //valido el sexo

     m->sexo = aMayuscula(m->sexo);
    if (m->sexo != 'M' && m->sexo != 'F') {
        fprintf(pfErr, "Registro %d: Sexo invalido (%c)\n", nroRegistro, m->sexo);
        valido = 0;
    }

    // aca validos las fechas

    if (!validarFecha(m->fechaNacimiento) || calcularEdad(m->fechaNacimiento, fechaproceso) < 10) {
        fprintf(pfErr, "Registro %d: Fecha nacimiento invalida\n", nroRegistro);
        valido = 0;
    }

    if (!validarFecha(m->fechaAfiliacion) || compararFechas(m->fechaAfiliacion, fechaproceso) > 0 || compararFechas(m->fechaAfiliacion, m->fechaNacimiento) <= 0)
        {
    fprintf(pfErr, "Registro %d: Fecha afiliacion invalida\n", nroRegistro);
    valido = 0;
        }
    if (!validarFecha(m->fechaUltimaCuota) || m->fechaUltimaCuota.anio > fechaproceso.anio) {
        fprintf(pfErr, "Registro %d: Fecha ultima cuota invalida\n", nroRegistro);
        valido = 0;
    }

    // valido los planes
    if (mistrcmp(m->plan,"BASIC")!=0 && mistrcmp(m->plan,"PREMIUM")!=0 &&mistrcmp(m->plan,"VIP")!=0 && mistrcmp(m->plan,"FAMILY")!=0)
        {
        fprintf(pfErr, "Registro %d: Plan invalido (%s)\n", nroRegistro, m->plan);
        valido = 0;
        }


    // valido el tutor si es menor
    int edad = calcularEdad(m->fechaNacimiento, fechaproceso);

     if (edad < 18) {
        mistrcpy(m->categoria, "MENOR");
    } else {
        mistrcpy(m->categoria, "ADULTO");
    }

    if (edad < 18) {
        if (!validarEmail(m->emailTutor))
        {
            fprintf(pfErr, "Registro %d: Email tutor invalido (%s)\n", nroRegistro, m->emailTutor);
            valido = 0;
        }
    } else {
        mistrcpy(m->emailTutor, "N/A");
    }

    return valido;
}


void archivo_cargado_validado(t_fecha fechaproceso)
{

    char bin_name[64], err_name[64];

    sprintf(bin_name, "miembros-VC-%04d%02d%02d.dat",
            fechaproceso.anio, fechaproceso.mes, fechaproceso.dia);

    sprintf(err_name, "error-VC-%04d%02d%02d.txt",
            fechaproceso.anio, fechaproceso.mes, fechaproceso.dia);


    FILE *in  = fopen("miembros-VC.txt", "r");

    if (!in)
    {
    printf("No se pudo abrir miembros-VC.txt\n");
    return;
    }

    FILE *bin = fopen(bin_name, "wb");
    if (!bin)
      { printf("No se pudo crear %s\n", bin_name);
        fclose(in);
        return;
      }

    FILE *err = fopen(err_name, "w");
    if (!err)
        { printf("No se pudo crear %s\n", err_name); fclose(in); fclose(bin);
        return;
    }

    char linea[512];
    int nro=0, ok=0, bad=0;
    t_persona m;

        while (fgets(linea, sizeof(linea), in))     // de aca
        {
            nro++;
            // justo después de leer la línea con fgets
           if (nro == 1 && (unsigned char)linea[0] == 0xEF &&
                    (unsigned char)linea[1] == 0xBB &&
                    (unsigned char)linea[2] == 0xBF) {
        // desplazar para sacar los 3 bytes de BOM
        memmove(linea, linea+3, mistrlen(linea)-2); // hasta aca lo que hace esto es sacar los 3 bytes del bom sino aparece algo q no debe "==|" y da error
        }

        if (cargar_miembro_desde_texto(linea, &m, fechaproceso, err, nro)) {
            fwrite(&m, sizeof(t_persona), 1, bin);  // válido → binario
            ok++;
        } else {
            bad++;                                  // inválido → error
        }
    }

    fclose(in); fclose(bin); fclose(err);
    printf("Grabados: %d validos en %s | Descartados: %d en %s\n",
           ok, bin_name, bad, err_name);
           mostrar_archivo(bin_name);
           mostrar_errores(err_name);

}

void mostrar_archivo(const char *bin_name)
{
    FILE *pf = fopen(bin_name, "rb");
    if (!pf) {
        printf("No se pudo abrir %s\n", bin_name);
        return;
    }

    t_persona m;
    printf("\n\t---------------------------------------------------------------------- LISTA DE VALIDOS ----------------------------------------------------------\n\n\n");
   printf("     DNI     |  NOMBRE Y APELLIDO   |FECHA DE NACIMIENTO|SEXO|FECHA DE AFILIACION|CATEGORIA|FECHA DE ULTIMA CUOTA PAGA| ESTADO | PLAN      | EMAIL TUTOR \n");
     printf("==================================================================================================================================================================\n");
   while (fread(&m, sizeof(t_persona), 1, pf) == 1) {

        printf("| %-10ld | %-20s |     %02d/%02d/%-4d    | %-3c|     %02d/%02d/%-4d    | %-7s |         %02d/%02d/%-4d       |   %1c    | %-9s | %-20s \n",
               m.dni,
               m.nomyape,
               m.fechaNacimiento.dia, m.fechaNacimiento.mes, m.fechaNacimiento.anio,
               m.sexo,
               m.fechaAfiliacion.dia, m.fechaAfiliacion.mes, m.fechaAfiliacion.anio,
               m.categoria,
               m.fechaUltimaCuota.dia, m.fechaUltimaCuota.mes, m.fechaUltimaCuota.anio,
               m.estado,
               m.plan,
               m.emailTutor);
               printf("==================================================================================================================================================================\n");
    }

    fclose(pf);
}

void mostrar_errores(const char *nombreErr)
{
    FILE *pf = fopen(nombreErr, "rb");
    if (!pf) {
        printf("No se pudo abrir el archivo de errores %s\n", nombreErr);
        return;
    }

    char linea[256];
    printf("\n------ LISTA DE ERRORES ------\n");
    while (fgets(linea, sizeof(linea), pf)) {
        printf("%s", linea);
    }
    fclose(pf);
}

