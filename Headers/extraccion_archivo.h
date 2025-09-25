#ifndef EXTRACCION_ARCHIVO_H_INCLUDED
#define EXTRACCION_ARCHIVO_H_INCLUDED
#include <stdio.h>   // para FILE*
#include "miembros.h"
int cargar_miembro_desde_texto(const char *linea, t_persona *m, t_fecha fechaproceso,FILE *pfErr, int nroRegistro);
void archivo_cargado_validado(t_fecha fechaproceso);
void mostrar_archivo(const char *bin_name);
void mostrar_errores(const char *nombreErr) ;

#endif // EXTRACCION_ARCHIVO_H_INCLUDED
