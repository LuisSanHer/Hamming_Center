/*
 * genetico.c
 *
 *  Created on: 08/10/2020
 *
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "rand.h"
#include "genetico.h"
#include "objetivo.h"

void Inicializar(POBLACION *P){
	size_t i,j;
	for(i=0 ; i< P->size ; i++){
		for(j=0 ; j<mop.nbin ; j++){
			if(randomperc() < 0.5){
				P->ind[i].x[j] = 0;
			}else{
				P->ind[i].x[j] = 1;
			}
		}
	}
}

void Display_pop(POBLACION *P){
	size_t i,j;
	for(i=0 ; i<P->size ; i++){
		printf("Individuo %zu: f: %lf \t x: ", i, P->ind[i].f);
		for(j=0 ; j<mop.nbin ; j++){
			printf("%d", P->ind[i].x[j]);
		}
		printf("\n");
	}
}

void Mutacion(POBLACION *Q, double Pm){
	size_t i;
	size_t n = Q->size;
	for(i=0 ; i<n ; i++){ //Para todos los individuos
		bit_wise_mutation(&Q->ind[i], Pm); //Mutar bit a bit
	}
}

void Evaluacion(POBLACION *Q){
	size_t i;
	int n = Q->size;
	for(i=0 ; i<n ; i++){ //Para todos los individuos
		NoLinealidad(&Q->ind[i]); //Calcular la No linealidad.
		SAC_0(&Q->ind[i]); 				//Calcular el criterio estricto de avalancha cero.
		aptitud(&Q->ind[i]); 			//Calcular la aptitud en base a NL y SAC_0 (restricciones).
	}
}

void Crossover(POBLACION *P, POBLACION *Q, double Pc){
	size_t i,j;
	size_t n = P->size ;
	int *A;
	int *p;

	A = (int*)malloc(sizeof(int) * n);
	p = (int*)malloc(sizeof(int) * n);
	for(i=0 ; i<n ; i++){
		A[i] = i;
	}
	shuffle(A, n); //Individuos de P ordenados aleatoriamente
	//Primer Torneo binario
	j = 0;
	for(i=0 ; i<n ; i=i+2){
		if( P->ind[A[i]].f < P->ind[A[i+1]].f){ //Se evalúa cual es
			p[j] = A[i];												  //mejor en terminos
		}else{																	//de minimización
			p[j] = A[i+1];
		}
		j++;
	}
	shuffle(A, n); //Individuos de P ordenados aleatoriamente
	//Segundo Torneo binario
	for(i=0 ; i<n ; i=i+2){
		if( P->ind[A[i]].f < P->ind[A[i+1]].f){ //Se evalúa cual es
			p[j] = A[i];												  //mejor en terminos
		}else{																	//de minimización
			p[j] = A[i+1];
		}
		j++;
	}
	//Crossver P1 x P2 regresando 2 hijos.
	for(i=0 ; i<n ; i=i+2){
		one_point_crossover( &P->ind[p[i]], &P->ind[p[i+1]], &Q->ind[i], &Q->ind[i+1], Pc);
	}
	free(A);
	free(p);
}

void one_point_crossover(INDIVIDUO *P1, INDIVIDUO *P2, INDIVIDUO *H1, INDIVIDUO *H2, double Pc){
	int p, i;
	if(randomperc() < Pc){ 				//Si random caé en la probabilidad de cruza
		p = rndint(1, mop.nbin-2);		//buscar un punto de cruza
		for(i=0 ; i<mop.nbin ; i++){	//Para toda la cadena binaria
			if(i<p){					//Si aun no llegas al punto de cruza
				H1->x[i] = P1->x[i];	//los hijos heredan el bit [i]
				H2->x[i] = P2->x[i];	//de los padres
			}else{
				H1->x[i] = P2->x[i]; 	//Si ya llegaste al punto de cruza
				H2->x[i] = P1->x[i];	//invertir los bits restantes.
			}
		}
	}else{
		/*for(i=0 ; i<mop.nbin ; i++){
			H1->x[i] = P1->x[i];
			H2->x[i] = P2->x[i];
		}*/
		memcpy(H1->x, P1->x, sizeof(int)*mop.nbin); //Si no cayeron en la probabilidad de cruza
		memcpy(H2->x, P2->x, sizeof(int)*mop.nbin); //entonces pasan exactamente como están.
	}
}

void bit_wise_mutation(INDIVIDUO *Q, double Pm){
	size_t i;
	for(i=0 ; i<mop.nbin ; i++){ //Para toda la cadena binaria
		if(randomperc() < Pm){ //Si random caé dentro del Parametro de mutación
			Q->x[i] = 1 - Q->x[i]; // MUTAR (Cambia de 0 a 1 y viceversa).
		}
	}
}

void Display_ind(INDIVIDUO ind){
	size_t i;
	// El *-1 solo es para mostrar en transformada
	printf("  \033[1;41m NL: %.3lf\033[0m", - ind.NL);
	printf("  \033[1;41m SAC: %.3lf\033[0m", ind.SAC);
	printf("  \033[1;41m f: %.3lf\033[0m x: ", - ind.f);
	for(i=0 ; i<mop.nbin ; i++){
		printf("%d", ind.x[i]);
	}
	mop.nbin >= 121?printf("\n"):printf("\n\n");
}

int Mejor_solucion(POBLACION *P){
	size_t i, index;
  INDIVIDUO *mejor = (INDIVIDUO*)malloc(sizeof(INDIVIDUO));
  mejor->x=(int*)malloc(sizeof(int) * mop.nbin);
	mejor->esp=(int*)malloc(sizeof(int) * mop.nbin);
  cpy_ind(mejor, &P->ind[0]);
	index = 0;
	for(i=0 ; i<P->size ; i++){
		if( mejor->f > P->ind[i].f ){	// Evaluacion en términos de
			cpy_ind(mejor, &P->ind[i]);// minimización.
			index = i;                  //
		}
	}
	free(mejor->esp);
  free(mejor->x);
  free(mejor);
	return index;
}

int Peor_solucion(POBLACION *P){
	size_t i, index;
  INDIVIDUO *peor = (INDIVIDUO*)malloc(sizeof(INDIVIDUO));
  peor->x=(int*)malloc(sizeof(int) * mop.nbin);
	peor->esp=(int*)malloc(sizeof(int) * mop.nbin);
  cpy_ind(peor, &P->ind[0]);
	index = 0;
	for(i=0 ; i<P->size ; i++){
		if( peor->f < P->ind[i].f ){	// Evaluacion en términos de
			cpy_ind(peor, &P->ind[i]);  // minimización.
			index = i;                  //
		}
	}
	free(peor->esp);
  free(peor->x);
  free(peor);
	return index;
}

void estadisticas(POBLACION *P, size_t i, FILE* file){
	int mejor;
	printf("\033[1;44mGeneración: %.3zu\033[0m", i);
	mejor = Mejor_solucion(P);
	Display_ind(P->ind[mejor]);
	// El *-1 solo es para mostrar en transformada
	fprintf(file,"%lf\n", - P->ind[mejor].f);
}

void Unir_poblaciones(POBLACION *P, POBLACION *Q, POBLACION *T){
	size_t i;
	for(i=0 ; i<T->size ; i++){
		if(i < P->size){
			cpy_ind(&T->ind[i], &P->ind[i]);
		}else{
			cpy_ind(&T->ind[i], &Q->ind[i-P->size]);
		}
	}
}

void Seleccionar_mejores(POBLACION *T, POBLACION *P){
	Ordenar(T);
	size_t i;
	for(i=0 ; i<P->size ; i++){
		cpy_ind(&P->ind[i], &T->ind[i]);
	}
}

void Ordenar(POBLACION *T){
	size_t i,j;
	INDIVIDUO *aux = (INDIVIDUO*)malloc(sizeof(INDIVIDUO));
  aux->x=(int*)malloc(sizeof(int) * mop.nbin);
	aux->esp=(int*)malloc(sizeof(int) * mop.nbin);
	for(i = 1; i < T->size; i++) {
		for(j = 0; j < (T->size - i); j++){
			if(T->ind[j].f > T->ind[j+1].f ){ 		// Ordenamiento en términos
         cpy_ind(aux, &T->ind[j]);          // minimización.
			   cpy_ind(&T->ind[j], &T->ind[j+1]);	//
			   cpy_ind(&T->ind[j+1], aux);
		   }
		}
	}
	free(aux->esp);
  free(aux->x);
  free(aux);
}
