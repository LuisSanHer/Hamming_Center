#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "rand.h"
#include "genetico.h"
#include "objetivo.h"
#include "mem_structs.h"

MOP mop;
GA ga;
int n;
int k;
int radio;

int main(int argc, char *argv[]){

	POBLACION B; // Conjunto B de k cadenas binarias.
	POBLACION P,Q,T;
	size_t i=0, mejor_p, peor_q;
	int alg, longitud, poblacion, gmax, semilla, k_aux, r_aux;
	char algoritmo[10], r[10], p[10], max[10], sem[10], k_tmp[10], r_tmp[10];
	FILE* file;

	printf("\t1.- Genetico simple\n");
	printf("\t2.- Genetico con elitismo\n");
	printf("\t3.- Genetico Mu + Lambda\n");
	do {
		printf("Elige el algoritmo a ejecutar (1,2,3): ");
		scanf("%s", algoritmo);
		alg = atoi(algoritmo);
		//printf("%d\n", alg);
		if (alg<1 || alg>3)
			printf("\tOpción invalida\n");
	} while(alg<1 || alg>3 );


	do {
		printf("Ingrese exponente (4,6,8) para la longitud de la cadena binaria (genotipo): ");
		scanf("%s", r);
		longitud = atoi(r);
		//printf("%d\n", longitud);
		if (longitud!=4 && longitud!=6 && longitud!=8)
			printf("\tOpción invalida\n");
	} while(longitud!=4 && longitud!=6 && longitud!=8);


	do {
		printf("\nIngrese el tamaño de la población: ");
		scanf("%s", p);
		poblacion = atoi(p);
		//printf("%d\n", poblacion);
		if (poblacion%2 != 0 || poblacion == 0 )
			printf("\tIngrese un número par\n");
	} while(poblacion%2 != 0 || poblacion == 0);


	do {
		printf("\nIngrese el máximo de generaciones: ");
		scanf("%s", max);
		gmax = atoi(max);
		//printf("%d\n", gmax);
		if (gmax == 0 )
			printf("\tIngrese un número \n");
	} while(gmax == 0);

	do {
		printf("\nIngrese el una semilla(1-10): ");
		scanf("%s", sem);
		semilla = atoi(sem);
		//printf("%d\n", semilla);
		if ( semilla<1 || semilla>10 )
			printf("\tOpción invalida\n");
	} while( semilla<1 || semilla>10 );

	do {
		printf("\nIngrese el tamaño (k) del conjunto B de cadenas binarias: ");
		scanf("%s", k_tmp);
		k_aux = atoi(k_tmp);
		//printf("%d\n", gmax);
		if (k_aux == 0 )
			printf("\tIngrese un número \n");
	} while(k_aux == 0);

  k = k_aux;
	n = longitud;
	mop.nbin = pow(2, n);
	mop.nobj = 1;

	ga.psize = poblacion;
	ga.Pc = 0.9;
	ga.Pm = 1.0/mop.nbin;
	ga.Gmax = gmax;

	alloc_pop(&B, k); //Crear memoria para el conjunto B

	alloc_pop(&P, ga.psize);
	alloc_pop(&Q, ga.psize);
	alloc_pop(&T, ga.psize*2);

	randomize(semilla/10.0);

	//Inicializar(&B); //Inicializar el conjunto B de manera aleatoria
	Leer_Conjunto(&B); //Leer el conjunto de prueba junto a un radio.
	Display_pop(&B);

	Inicializar(&P);
	Evaluacion(&P, &B);

	switch(alg){
		case 1:
			file = fopen("Experimentos/simple.txt", "w");
			printf("\n\t\t\t \033[1;42m Genetico Simple \033[0m \n\n");
				for(i=0 ; i<ga.Gmax ; i++){
					Crossover(&P,&Q, ga.Pc);
					Mutacion(&Q, ga.Pm);
					Evaluacion(&Q, &B);
					cpy_pop(&P, &Q);
					//fprintf(file,"%zu\t", i);
					estadisticas(&P, i, file);
				}
				fclose(file);
				printf("\n\t\t\t \033[1;42m Genetico Simple terminado \033[0m \n\n");
		break;
		case 2:
			file = fopen("Experimentos/elitismo.txt", "w");
			printf("\n\t\t\t \033[1;42m Genetico con elitismo \033[0m \n\n");
			for(i=0 ; i<ga.Gmax ; i++){
				Crossover(&P,&Q, ga.Pc);
				Mutacion(&Q, ga.Pm);
				Evaluacion(&Q, &B);
				/*********************APLICANDO ELITISMO**************************/
				mejor_p = Mejor_solucion(&P); 				//Obtener indice de la mejor solucion en P
				peor_q = Peor_solucion(&Q); 				//Obtener indice de la peor solucion en Q
				cpy_ind(&Q.ind[peor_q], &P.ind[mejor_p]);	//Reemplazando.
				cpy_pop(&P, &Q);
				//fprintf(file,"%zu\t", i);
				estadisticas(&P, i, file);
			}
			fclose(file);
			printf("\n\t\t\t \033[1;42m Genetico con elitismo terminado \033[0m \n\n");
		break;
		case 3:
			file = fopen("Experimentos/miulambda.txt", "w");
			printf("\n\t\t\t \033[1;42m Genetico Miu plus Lambda \033[0m \n\n");
			for(i=0 ; i<ga.Gmax; i++){
				Crossover(&P,&Q, ga.Pc);
				Mutacion(&Q, ga.Pm);
				Evaluacion(&Q, &B);
				/*********************APLICANDO Miu + Lambda**************************/
				Unir_poblaciones(&P,&Q,&T);
				Seleccionar_mejores(&T,&P);
				//fprintf(file,"%zu\t", i);
				estadisticas(&P, i, file);
			}
			fclose(file);
			printf("\n\t\t\t \033[1;42m Genetico Miu plus Lambda terminado \033[0m \n\n");
		break;
		default:
			printf("Opción inválida\n");
		break;
	}

	free_pop(&B);
	free_pop(&T);
	free_pop(&P);
	free_pop(&Q);
	return 0;
}
