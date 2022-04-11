#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 3
#define FROM_MASTER 1
#define FROM_WORKER 2
#define DEBUG 1

MPI_Status status;

static double a[SIZE][SIZE];
static double b[SIZE][SIZE];
static double c[SIZE][SIZE];

static void init_matriz(void){
    int i, j;
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++) {
              a[i][j] = 1;
              b[i][j] = 1;
       } //end for i
    }  //end for j
} //end init_matriz()

static void print_matriz(void){
	int i, j;
		for(i = 0; i < SIZE; i++) {
    		for(j = 0; j < SIZE; j++) {
        		printf("%7.2f", c[i][j]);
    		} //end for i
		printf("\n");
	}//end for j
}//end print_matriz

int main(int argc, char **argv){
	int id, nprocess;
	int linhas;
	int mtype;
	int destino, src, deslocamentoa, deslocamentob;
	double start_time, end_time;
	int i, j, k, l;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocess);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	linhas = SIZE/nprocess;  //tamanho de bloco
	mtype = FROM_MASTER; // = 1

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
				} //end for i
			}  //end for j
			end_time = MPI_Wtime();
			print_matriz();//---------------------------------
			printf("Numero de processos: %2d Tempo: %f\n", nprocess, end_time-start_time);
		} // end  if(nprocess == 1)
		else {
			for(l = 0; l < nprocess; l++){
				deslocamentob = linhas*l;  //comeca de (tamanho do bloco * id do processo)
				deslocamentoa = linhas;
				mtype = FROM_MASTER; // tag =1

				for(destino = 1; destino < nprocess; destino++){
					MPI_Send(&deslocamentoa, 1, MPI_INT, destino, mtype, MPI_COMM_WORLD);
					MPI_Send(&deslocamentob, 1, MPI_INT, destino, mtype, MPI_COMM_WORLD);
					MPI_Send(&linhas, 1, MPI_INT, destino, mtype, MPI_COMM_WORLD);
					MPI_Send(&a[deslocamentoa][0], linhas*SIZE, MPI_DOUBLE, destino, mtype, MPI_COMM_WORLD);
					MPI_Send(&b[0][deslocamentob], linhas*SIZE, MPI_DOUBLE, destino, mtype, MPI_COMM_WORLD);

					deslocamentoa += linhas;
					deslocamentob = (deslocamentob+linhas)%SIZE;
				} // end for destino
				deslocamentoa = linhas;
				deslocamentob = linhas*l;

				//--mult the final local and print final global mult
				for(i = 0; i < deslocamentoa; i++) {
					for(j = deslocamentob; j < deslocamentob+linhas; j++) {
						for(k = 0; k < SIZE; k++){
							c[i][j] = c[i][j] + a[i][k]*b[k][j];
						}//end for k
					} //end for j
				}// end for i
				/*- wait for results from all worker tasks */
				mtype = FROM_WORKER;
				for(src = 1; src < nprocess; src++){
					MPI_Recv(&deslocamentoa, 1, MPI_INT, src, mtype, MPI_COMM_WORLD, &status);
					MPI_Recv(&deslocamentob, 1, MPI_INT, src, mtype, MPI_COMM_WORLD, &status);
					MPI_Recv(&linhas, 1, MPI_INT, src, mtype, MPI_COMM_WORLD, &status);
					for(i = 0; i < linhas; i++) {
						MPI_Recv(&c[deslocamentoa+i][deslocamentob], deslocamentoa, MPI_DOUBLE, src, mtype, MPI_COMM_WORLD, &status);
					} //end for scr
				}//end for i
			} //end for l
			end_time = MPI_Wtime();
			print_matriz();
			printf("Numero de processos %2d Tempo: %f\n", nprocess, end_time-start_time);
		}//end else
	} //end if (id == 0)

	else{
	/*---------------------------- trabalhador----------------------*/
		if(nprocess > 1) {
			for(l = 0; l < nprocess; l++){
				mtype = FROM_MASTER;
				MPI_Recv(&deslocamentoa, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD,&status);
				MPI_Recv(&deslocamentob, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD,&status);
				MPI_Recv(&linhas, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD,&status);
				MPI_Recv(&a[deslocamentoa][0], linhas*SIZE, MPI_DOUBLE, 0, mtype,	MPI_COMM_WORLD, &status);
				MPI_Recv(&b[0][deslocamentob], linhas*SIZE, MPI_DOUBLE, 0, mtype,	MPI_COMM_WORLD, &status);

				for(i = deslocamentoa; i < deslocamentoa+linhas; i++) {
					for(j = deslocamentob; j < deslocamentob+linhas; j++) {
						for(k = 0; k < SIZE; k++){
							c[i][j] = c[i][j] + a[i][k]*b[k][j];
						} //end for j
					} //end for i
				} //end for l
	
				mtype = FROM_WORKER;
				MPI_Send(&deslocamentoa, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD);
				MPI_Send(&deslocamentob, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD);
				MPI_Send(&linhas, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD);
				for(i = 0; i < linhas; i++){
					MPI_Send(&c[deslocamentoa+i][deslocamentob], deslocamentoa, MPI_DOUBLE, 0, mtype, MPI_COMM_WORLD);
				} //end for i
			}//end for l
		} //end if (nprocess > 1)
	} // end else
	MPI_Finalize();
	return 0;
} //end main()
