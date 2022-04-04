#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

void main(){
	int pid, pip[2], pip2[2],status, size;
	pipe(pip);
	pipe(pip2);
	
	pid = fork();
	
	if(pid < 0){
		perror("Erro fork");
	}else if(pid == 0){
		char *msg = "O loco tá pegando fogo bicho";
		size = strlen(msg);
		write(pip[1], msg, size);
		write(pip2[1], &size, sizeof(size));
	}else{
		int tamanho;
		wait(&status);
		read(pip2[0], &tamanho, sizeof(tamanho));
		char buffer[tamanho];
		read(pip[0], buffer, tamanho); //processos nao compartilha dados
		

		printf("%s Chama o bombeiro lá\n\ntamanho -> %d\n\n", buffer, tamanho);
	}
}
