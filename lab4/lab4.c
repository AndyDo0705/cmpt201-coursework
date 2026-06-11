#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 256

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len;

  if (data_size == sizeof(uint64_t)) {
    len = snprintf(buf, BUF_SIZE, format, *(uint64_t *)data);
  } else {
    len = snprintf(buf, BUF_SIZE, format, *(void **)data);
  }

  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

int main() {
  void *heap_space = sbrk(256);
  if (heap_space == (void *)-1) {
    handle_error("sbrk failed");
  }

  struct header *block1 = (struct header *)heap_space;
  struct header *block2 = (struct header *)((char *)heap_space + 128);

  block1->size = 128;
  block1->next = NULL;

  block2->size = 128;
  block2->next = block1;

  void *data1 = (void *)(block1 + 1);
  void *data2 = (void *)(block2 + 1);

  size_t payload_size = 128 - sizeof(struct header);

  memset(data1, 0, payload_size);
  memset(data2, 1, payload_size);

  print_out("first block:       %p\n", &block1, sizeof(block1));
  print_out("second block:      %p\n", &block2, sizeof(block2));

  print_out("first block size:  %lu\n", &block1->size, sizeof(block1->size));
  print_out("first block next:  %p\n", &block1->next, sizeof(block1->next));

  print_out("second block size: %lu\n", &block2->size, sizeof(block2->size));
  print_out("second block next: %p\n", &block2->next, sizeof(block2->next));

  char *d1 = (char *)data1;
  for (size_t i = 0; i < payload_size; i++) {
    char buf[32];
    ssize_t len = snprintf(buf, sizeof(buf), "%d\n", d1[i]);
    write(STDOUT_FILENO, buf, len);
  }

  char *d2 = (char *)data2;
  for (size_t i = 0; i < payload_size; i++) {
    char buf[32];
    ssize_t len = snprintf(buf, sizeof(buf), "%d\n", d2[i]);
    write(STDOUT_FILENO, buf, len);
  }

  return 0;
}
