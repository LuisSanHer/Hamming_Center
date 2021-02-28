/*
 * genetico.h
 *
 *  Created on: 08/10/2020
 *
 */

#ifndef GENETICO_H_
#define GENETICO_H_
#include "objetivo.h"
#include "mem_structs.h"

void Mutacion(POBLACION *Q, double Pm);
void Evaluacion(POBLACION *Q);
void Crossover(POBLACION *P, POBLACION *Q, double Pc);
void one_point_crossover(INDIVIDUO *P1, INDIVIDUO *P2, INDIVIDUO *H1, INDIVIDUO *H2, double Pc);
void bit_wise_mutation(INDIVIDUO *Q, double Pm);
void Inicializar(POBLACION *P);
void Display_pop(POBLACION *P);
void Display_ind(INDIVIDUO ind);
int Mejor_solucion(POBLACION *P);
int Peor_solucion(POBLACION *P);
void estadisticas(POBLACION *P, size_t i, FILE* file);
void Unir_poblaciones(POBLACION *P, POBLACION *Q, POBLACION *T);
void Seleccionar_mejores(POBLACION *T, POBLACION *P);
void Ordenar(POBLACION *T);

#endif /* GENETICO_H */
