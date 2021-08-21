/*  Implementación de la transformada rápida de WH
 * objetivo.c
 *
 *  Created on: 03/11/2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "objetivo.h"

void Leer_Conjunto(POBLACION *B){
	FILE* fichero;
	if (n == 4) {
		fichero = fopen("Files_Prueba/pruebas4.txt","r");
	}else if(n == 6){
		fichero = fopen("Files_Prueba/pruebas6.txt","r");
	}else{
		fichero = fopen("Files_Prueba/pruebas8.txt","r");
	}
	size_t i,j;
	int aux;
	fscanf(fichero, "%d", &aux);
	radio = aux;
	for(i=0 ; i<B->size ; i++){
		for(j=0 ; j<mop.nbin ; j++){
			fscanf(fichero, "%d", &aux);
			B->ind[i].x[j] = aux;
		}
	}
	fclose(fichero);
}

int xor(int a, int b){
	//Si son iguales dame 0, si no, dame 1.
	if(a == b){
		return 0;
	}else{
		return 1;
	}
	//return a==b ? 0:1;
}

int hamming_distance(INDIVIDUO* A, INDIVIDUO *B){
  int hamming = 0;
  //Aplicando el XOR, distancia de hamming.
	for(int i=0 ; i<mop.nbin ; i++){
		if(xor(A->x[i], B->x[i]) == 1){
      hamming++;
    }
	}
	return hamming;
}

void aptitud(POBLACION* S, INDIVIDUO *p){
	int cont, aux;
  cont = 0;
  //printf("Distancias de hamming\n");
  for (int i=0 ; i<k ; i++) {
    p->dist[i] = hamming_distance(&S->ind[i], p);
    //printf("%d ", distancias[i]);
    if (p->dist[i] <= radio) {
      cont++;
    }
  }
  //printf("\nCont = %d\n", -cont);
  p->f = -cont;
}
