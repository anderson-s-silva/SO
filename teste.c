/**
 * @file ex2-statuswait.c
 * @author Anderson Soares da Silva
 * @date 21 Mar 2022
 * @brief  Exercício 2 (status/wait)
 */

#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int pid;//process ID
	int status;
	int number = 5;
	pid = fork();//pai criando filho

	if(pid < 0){//nao e pai e nem filho
		perror("Error: erro no fork \n");
		WIFEXITED(status);
	}else if(pid == 0){//pid == filho
		printf("Eu sou o processo %d\tFilho de %d\n", getpid(), getppid());
		pid = fork();
		WIFEXITED(status)
		//while(1);
	}else{//pid == pai
		pid= fork();
		printf("Eu sou o processo %d\tFilho de %d\n", getpid(), getppid());
		wait(&status);//espera filho terminar de execeutar
	}
	return 0;
}
