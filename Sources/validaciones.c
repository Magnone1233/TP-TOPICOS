#include <stdio.h>
#include <stdlib.h>
#include "miembros.h"
#include "validaciones.h"

/*CARGAMOS MIEMBRO */
int calcularEdad(t_fecha nac, t_fecha proc)
{
    int edad = proc.anio - nac.anio;
    if (proc.mes < nac.mes || (proc.mes == nac.mes && proc.dia < nac.dia))
        edad--;
    return edad;
}

int validarEmail(const char *email)
{
    int arroba=0, punto=0;
    while (*email) {
        if (*email=='@') arroba=1;
        if (*email=='.') punto=1;
        email++;
    }
    return (arroba && punto);
}
void limpiarSaltoLinea(char *cadena)
 {
    int i = 0;
    while (cadena[i] != '\0') {
        if (cadena[i] == '\n' || cadena[i] == '\r') {
            cadena[i] = '\0';
            break;
        }
        i++;
    }
}
int mistrlen(const char *cadena)
 {
    int longitud = 0;

    // Mientras no lleguemos al car�cter nulo
    while (cadena[longitud] != '\0') {
        longitud++;
    }

    return longitud;
}
int validarFecha(t_fecha f)
{
    int diasMes[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (f.anio < 1900 || f.mes < 1 || f.mes > 12)
        return 0;
    if (esBisiesto(f.anio)) diasMes[2] = 29;
    return (f.dia >= 1 && f.dia <= diasMes[f.mes]);
}

int esBisiesto(int anio)
{
    return (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0));
}

int compararFechas(t_fecha f1, t_fecha f2)
{
    if (f1.anio != f2.anio) return f1.anio - f2.anio;
    if (f1.mes != f2.mes) return f1.mes - f2.mes;
    return f1.dia - f2.dia;
}

char *mistrtok(char *s1,const char *s2)
{
    char *p=s1;

    while(*p!='\0')
    {
        if (*p=='\n')
        {
            *p='\0';
             break;
        }
        p++;
    }
    return s1;
}


int mistrcmp(char *cad1,char *cad2) // 0 si son iguales , neg si la prim cadena es menor y pos si la prim cadena es mayor
{
    while (*cad1 != '\0' && *cad1 == *cad2) {
        cad1++;
        cad2++;
    }

    return (unsigned char)*cad1 - (unsigned char)*cad2;
}

char* mistrcpy (char *dest, char *origen)
{
  char *p=dest;

   while (*origen!='\0')
    {
        *p=*origen;
        p++;
        origen++;
    }
    *p='\0';

    return dest;

}

/*VALIDAMOS EL NOMRE*/

int esEspacio(char c)
{
    return (c == ' '|| c == '\t'|| c == '\n'|| c == '\r'|| c == '\f'||c == '\v');
}

char* normalizarcad(char *cad)
{
    char *lec = cad;   // lectura
    char *esc = cad;   // escritura
    int primeraLetra = 1;

    while (*lec != '\0') {
        if (esEspacio(*lec)) {
            // copiar un �nico espacio y marcar pr�xima como primera letra
            if (esc > cad && *(esc-1) != ' ') {
                *esc = ' ';
                esc++;
                primeraLetra = 1;
            }
            lec++;
        } else {
            if (primeraLetra) {
                *esc = aMayuscula(*lec);
                primeraLetra = 0;
            } else {
                *esc = aMinuscula(*lec);
            }
            esc++;
            lec++;
        }
    }

    *esc = '\0';   // terminar cadena

     char *p = cad;
    while (*p != '\0' && *p != ' ')  //<- poner la coma si no existe
    {
        p++;
    }
    if (*p == ' ') {

        char *fin = esc;
        while (fin >= p) {
            *(fin+1) = *fin;
            fin--;
        }
        *p = ',';
    }


    return cad;
}


char aMayuscula(char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 32;   // convierte a may�scula
    return c;            // si no es min�scula, lo deja igual
}

char aMinuscula(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + 32;   // convierte a min�scula
    return c;            // si no es may�scula, lo deja igual
}
