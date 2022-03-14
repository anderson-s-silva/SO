/**
* @file ex.c
* @author Anderson Soares da Silva
* @date 14 Mar 2022
* @brief  exercicio 2 aula posix
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

int cont = 0, cont_total = 0;

void handler(int signum){
	printf("\nSIGALRM\n");
	exit(0);
}

void handler2(int signum){
	alarm(20);
	char escolha;
	printf("\nSIGINT\n");
	cont++;
	cont_total++;
	if(cont == 3){
		
		printf("deseja sair do programa?\n(Y/N)\n");
		scanf("%c", &escolha);
		
		if(escolha == 'Y' || escolha == 'y'){
			exit(0);
		}else if(escolha == 'N' || escolha == 'n'){
			cont = 0;
			printf("\nresposta:%c\ncontinuando\n", escolha);
		}else{
			cont = 0;
			printf("\nentrada invalida!\ncontinuando\n");
			
		}	
	}
}

void handler3(int signum){
	alarm(20);
	printf("\nSIGTSTP\ncontador de SIGINT: %d\n", cont_total);
	pause();
}

int main(int argc, char* argv[]){
	
	signal(SIGALRM, handler);
	signal(SIGINT, handler2);
	signal(SIGTSTP, handler3);
 
  	while(1);
  	
  	return 0;
}
