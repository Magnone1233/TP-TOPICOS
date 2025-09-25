#ifndef VALIDACIONES_H_INCLUDED
#define VALIDACIONES_H_INCLUDED
#include "miembros.h"

//void cargarmiembro(t_persona *m,t_fecha fechaproceso);
int calcularEdad(t_fecha nac, t_fecha proc) ;
int validarEmail(const char *email);
int validarFecha(t_fecha f);
int esBisiesto(int anio);
int esEspacio(char c);
char* normalizarcad(char *cad);
char aMayuscula(char c);
char aMinuscula(char c);
char* mistrcpy (char *dest, char *origen);
int mistrcmp(char *cad1,char *cad2);
char *mistrtok(char *s1,const char *s2);
int compararFechas(t_fecha f1, t_fecha f2);
int mistrlen(const char *cadena);
void limpiarSaltoLinea(char *cadena);


#endif // VALIDACIONES_H_INCLUDED
