#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while (1) {
    printf("Enter programs to run.\n");
    printf("> ");

    read = getline(&line, &len, stdin);

    if (read == -1) {
      break;
    }

    if (read > 0 && line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }

    pid_t pid = fork();

    if (pid < 0) {
      perror("Fork failed");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      execl(line, line, NULL);

      printf("Exec failure\n");
      exit(1);
    } else {
      int wstatus;
      if (waitpid(pid, &wstatus, 0) == -1) {
        perror("waitpid error");
        exit(EXIT_FAILURE);
      }
    }
  }

  free(line);
  return 0;
}
