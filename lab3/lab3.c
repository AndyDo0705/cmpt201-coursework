#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_history(char *history[], int total_count) {
  int start = 0;
  int num_to_print = total_count;

  if (total_count > 5) {
    start = total_count - 5;
    num_to_print = 5;
  }

  for (int i = 0; i < num_to_print; i++) {
    int index = (start + i) % 5;
    printf("%s", history[index]);
  }
}

int main() {
  char *history[5] = {NULL};

  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int count = 0;
  while (1) {
    printf("Enter input: ");

    read = getline(&line, &len, stdin);

    if (read == -1) {
      break;
    }

    int index = count % 5;

    if (history[index] != NULL) {
      free(history[index]);
    }

    history[index] = line;
    count++;

    line = NULL;
    len = 0;

    if (strcmp(history[index], "print\n") == 0) {
      print_history(history, count);
    }
  }

  for (int i = 0; i < 5; i++) {
    if (history[i] != NULL) {
      free(history[i]);
    }
  }
  free(line);

  printf("\n(Ended program)\n");
  return 0;
}
