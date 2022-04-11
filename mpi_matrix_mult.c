/**
* @file mpi_matrix_mult.c
* @author Anderson Soares da Silva
* @date 11 abr 2022
* @brief  exercise mpi matrizes
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 3
#define FROM_MASTER 1
#define FROM_WORKER 2

MPI_Status status;

static double a[SIZE][SIZE];
static double b[SIZE][SIZE];
static double c[SIZE][SIZE];

static void init_matriz(void){
    int i, j;
    for (i = 0; i < SIZE; i++){
        for (j = 0; j < SIZE; j++) {
              a[i][j] = 1;
              b[i][j] = 1;
              c[i][j] = 0;
       } //fim for i
    }  //fim for j
} //fim init_matriz()

static void print_matriz(void){
	int i, j;
		for(i = 0; i < SIZE; i++) {
    		for(j = 0; j < SIZE; j++) {
        		printf("%7.2f", c[i][j]);
    		} //fim for i
		printf("\n");
	}//fim for j
}//fim print_matriz

int main(int argc, char **argv){
	int id, nprocess;
	int linhas;
	int mtype;
	int destino, src, deslocI, deslocJ;
	double start_time, end_time;
	int i, j, k, l;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocess);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	linhas = SIZE/nprocess;  //tamanho de bloco
	mtype = FROM_MASTER; // = 1
	/*---------------------------- gerente------------------------------*/
	if (id == 0) {
		/*Inicializacao*/
		printf("Tamanho = %d, numero de nos = %d\n", SIZE, nprocess);
		init_matriz();

		start_time = MPI_Wtime();

		if(nprocess == 1) {
			for(i = 0; i < SIZE; i++) {
				for(j = 0; j < SIZE; j++) {
					for(k = 0; k < SIZE; k++)
					c[i][j] = c[i][j] + a[i][k]*b[k][j];
				} //fim for i
			}  //fim for j
			end_time = MPI_Wtime();//pausa tempo de exec
			print_matriz();//---------------------------------
			printf("Numero de processos: %2d Tempo: %f\n", nprocess, end_time-start_time);
		} // end  if(nprocess == 1)
		else {
			for(l = 0; l < nprocess; l++){
				deslocJ = linhas*l;  //comeca do (tamanho do bloco * id do processo)
				deslocI = linhas;
				mtype = FROM_MASTER; // tag =1

				for(destino = 1; destino < nprocess; destino++){
					MPI_Send(&deslocI, 1, MPI_INT, destino, mtype, MPI_COMM_WORLD);
					MPI_Send(&deslocJ, 1, MPI_INT, destino, mtype, MPI_COMM_WORLD);
					MPI_Send(&linhas, 1, MPI_INT, destino, mtype, MPI_COMM_WORLD);
					MPI_Send(&a[deslocI][0], linhas*SIZE, MPI_DOUBLE, destino, mtype, MPI_COMM_WORLD);
					MPI_Send(&b[0][deslocJ], linhas*SIZE, MPI_DOUBLE, destino, mtype, MPI_COMM_WORLD);

					deslocI += linhas;
					deslocJ = (deslocJ+linhas)%SIZE;
				} // fim for destino
				deslocI = linhas;
				deslocJ = linhas*l;

				//--mult o local final e imprime o mult final global
				for(i = 0; i < deslocI; i++) {
					for(j = deslocJ; j < deslocJ+linhas; j++) {
						for(k = 0; k < SIZE; k++){
							c[i][j] = c[i][j] + a[i][k]*b[k][j];
						}//fim for k
					} //fim for j
				}// fim for i
				/*- espera os resultados de todas as tarefas do trabalhador */
				mtype = FROM_WORKER;
				for(src = 1; src < nprocess; src++){
					MPI_Recv(&deslocI, 1, MPI_INT, src, mtype, MPI_COMM_WORLD, &status);
					MPI_Recv(&deslocJ, 1, MPI_INT, src, mtype, MPI_COMM_WORLD, &status);
					MPI_Recv(&linhas, 1, MPI_INT, src, mtype, MPI_COMM_WORLD, &status);
					for(i = 0; i < linhas; i++) {
						MPI_Recv(&c[deslocI+i][deslocJ], deslocI, MPI_DOUBLE, src, mtype, MPI_COMM_WORLD, &status);
					} //fim for scr
				}//fim for i
			} //fim for l
			end_time = MPI_Wtime();//pausa o tempo de exec
			print_matriz();
			printf("Numero de processos %2d Tempo: %f\n", nprocess, end_time-start_time);
		}//end else
	} //fim if (id == 0)

	else{
	/*---------------------------- trabalhador----------------------*/
		if(nprocess > 1) {
			for(l = 0; l < nprocess; l++){
				mtype = FROM_MASTER;
				MPI_Recv(&deslocI, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD,&status);
				MPI_Recv(&deslocJ, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD,&status);
				MPI_Recv(&linhas, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD,&status);
				MPI_Recv(&a[deslocI][0], linhas*SIZE, MPI_DOUBLE, 0, mtype,	MPI_COMM_WORLD, &status);
				MPI_Recv(&b[0][deslocJ], linhas*SIZE, MPI_DOUBLE, 0, mtype,	MPI_COMM_WORLD, &status);

				for(i = deslocI; i < deslocI+linhas; i++) {
					for(j = deslocJ; j < deslocJ+linhas; j++) {
						for(k = 0; k < SIZE; k++){
							c[i][j] = c[i][j] + a[i][k]*b[k][j];
						} //fim for j
					} //fim for i
				} //fim for l

				mtype = FROM_WORKER;
				MPI_Send(&deslocI, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD);
				MPI_Send(&deslocJ, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD);
				MPI_Send(&linhas, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD);
				for(i = 0; i < linhas; i++){
					MPI_Send(&c[deslocI+i][deslocJ], deslocI, MPI_DOUBLE, 0, mtype, MPI_COMM_WORLD);
				} //fim for i
			}//fim for l
		} //fim if (nprocess > 1)
	} // end else
	MPI_Finalize();
	return 0;
} //end main()
