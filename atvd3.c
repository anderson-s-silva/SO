/**
 * @file atvd3.c
 * @author Anderson Soares da Silva
 * @date 21 Mar 2022
 * @brief  pratica processos
 */

#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int pid;//process ID
	int status;
  int aux=0;
	pid = fork();//pai criando filho
	
	if(pid < 0){//nao e pai e nem filho
		perror("Error: erro no fork \n");
	}else if(pid == 0){//pid == filho
    aux+=100;
		printf("filho\tpid = %d\naxiliar\t = %d", getpid(), aux);
		while(1);
	}else{//pid == pai
    aux+=10;
		printf("filho\tpid = %d\naxiliar\t = %d", getpid(), aux);
		wait(&status);//espera filho terminar de execeutar
	}
	return 0;
}

/*
REPOSTA: VERDADEIRO
Aux recebe 10 no inicio, e depois recebe mais 100, porém ao exibir a variável 
acrescida de 100 era esperado o resultado de 110 e não 100 como exibido na saída. 
Isso ocorre porque pai e filho não conseguem compartilhar dados.
*/
