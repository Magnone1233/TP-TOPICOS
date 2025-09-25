#ifndef MIEMBROS_H_INCLUDED
#define MIEMBROS_H_INCLUDED
#include "fecha.h"
typedef struct {
    long dni;
    char nomyape[61];
    t_fecha fechaNacimiento;
    char sexo; // F O M
    t_fecha fechaAfiliacion;
    char categoria[10]; // MENOR ADULTO
    t_fecha fechaUltimaCuota;
    char estado; // ALTA O BAJA SE CREAN CON A
    char plan[10]; // BASIC PREMIUM VIP FAMILY
    char emailTutor[30];
} t_persona;


#endif // MIEMBROS_H_INCLUDED
