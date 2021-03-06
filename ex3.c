/**
 * @file ex.c
 * @author Anderson Soares da Silva
 * @date 14 Mar 2022
 * @brief  exercicio 2 aula posix
 */

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int cont = 0;

void handler(int signum) {
  printf("\nSIGALRM\n");
  exit(0);
}

void handler2(int signum) {

  char escolha;
  printf("\nSIGINT\n");
  cont++;
  if (cont == 3) {
    alarm(20);
    printf("deseja sair do programa?\n(Y/N)\n");
     read(0, &escolha, 1);

    if (escolha == 'Y' || escolha == 'y') {
      exit(0);
    } else if (escolha == 'N' || escolha == 'n') {
      cont = 0;
      alarm(0);
      printf("\nresposta:%c\ncontinuando\n", escolha);
    } else {
      cont = 0;
      printf("\nentrada invalida!\ncontinuando\n");
      alarm(0);
    }
  }
}

void handler3(int signum) {
  printf("\nSIGTSTP\ncontador de SIGINT: %d\n", cont);
  pause();
}

int main(int argc, char *argv[]) {

  signal(SIGALRM, handler);
  signal(SIGINT, handler2);
  signal(SIGTSTP, handler3);

  while (1);

  return 0;
}
