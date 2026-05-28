#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  printf("Start PID = %d, parent = %d\n", getpid(), getppid());

  pid_t pid = fork();
  if (pid == 0) {
    sleep(1);
    printf("Child: PID = %d, parent PID = %d\n", getpid(), getppid());
  } else {
    printf("Parent: PID = %d, child PID = %d\n", getpid(), pid);
  }
  return 0;
}
