/*
 * objetivo.h
 *
 *  Created on: 03/11/2020
 *
 */

#ifndef TRANFORMADAWD_H_
#define TRANFORMADAWD_H_

#include "mem_structs.h"

void Leer_Conjunto(POBLACION *B);
int xor(int a, int b);
int hamming_distance(INDIVIDUO* A, INDIVIDUO *B);
void aptitud(POBLACION* B, INDIVIDUO *p);

#endif /* TRANFORMADA_H */
