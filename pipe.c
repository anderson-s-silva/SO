#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

void main(){
	int pid, pip[2], status, size;
	pipe(pip);
	
	pid = fork();
	
	if(pid < 0){
		perror("Erro fork");
	}else if(pid == 0){
		char *msg = "O loco tá pegando fogo bicho";
		size = strlen(msg);
		write(pip[1], msg, size);
	}else{
		char buffer[1024];
		wait(&status);
		read(pip[0], buffer, 1024); //processos nao compartilha dados
		printf("%s Chama o bombeiro lá\n\n", buffer);
	}
}
