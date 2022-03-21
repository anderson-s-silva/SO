/**
 * @file processes.c
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
	pid = fork();//pai criando filho
	
	if(pid < 0){//nao e pai e nem filho
		perror("Error: erro no fork \n");
	}else if(pid == 0){//pid == filho
		printf("filho\tpid = %d\n", getpid());
		while(1);
	}else{//pid == pai
		printf("pai\tpid = %d\n", getpid());
		wait(&status);//espera filho terminar de execeutar
	}
	return 0;
}
