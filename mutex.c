/**
* @file ex.c
* @author Anderson Soares da Silva
* @date 25 abr 2022
* @brief  exercise mutex
*/

/**
* @title RESPOSTAS
* @subtitle EXERCICIO 1
* @container_begin 
* A)O método de sincronização não deu certo ao descomentar a solução de Peterson
* B)REORGANIZADO, turn = 0 e pthread_join() ganhou um laço for exclusivo.
* C)pthread_join() estava aguardando por tempo indeterminado porque estaria esperando a resposta da thread sem saber se todas as threads foram criadas, dessa forma, ao inserir pthread_join() em um laço separado ele seria executado após todas as threads existirem. Além disso, foi necessário mudar o turn que valia 1 para 0 para que a sincronização fosse feita.
* @container_end
* @subtitle EXERCICIO 2
* @container_begin
* 
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{                      //***********************************************
	int id;                      //Struct para configurar os argmentos das threads
	int length;
} thread_arg, *ptr_thread_arg; 	     //***********************************************

pthread_t threads[2];                //Declaração threads
int flag[2];
int turn;


void *thread_func(void *arg){
	ptr_thread_arg  targ = (ptr_thread_arg) arg;
	int i;
	for(i = targ->id; i < targ->length; i +=2){
		if(targ->id == 0){
			pthread_mutex_unlock(&mut);
			//flag[0] = 1;
			//while (flag[1] == 1 && turn == 1); 
			printf("Thread par  %d - valor %d\n", (int)pthread_self(),i);
			flag[0] = 0;
			turn = 1;
			sched_yield();
		}else{
			pthread_mutex_lock(&mut); 
			//flag[1] = 1;
			//while (flag[0] == 1 && turn == 0);
			printf("Thread impar  %d - valor %d\n", (int)pthread_self(),i);
			flag[1] = 0;
			turn = 0;
			sched_yield();
		}		
	}
}

int main(){
	thread_arg arguments[2];
	int i;
	flag[0] = 0;
	flag[1] = 1;
	turn = 0;	
	for(i = 0; i < 2; i++){
		arguments[i].id = i;           
		arguments[i].length = 10; //Confugurando os parâmetros das threads
		pthread_create(&(threads[i]), NULL, thread_func, &(arguments[i])); // Criando as threads
		
	}
	for(i = 0; i < 2; i++){
		pthread_join(threads[i], NULL); //Inserir as threads na fila de pronto
	}
	return 0;
}
