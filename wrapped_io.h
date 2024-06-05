#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_INPUT_STR_LEN 8192
#define NEW_FILE_MODE     (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) 

#define _READ(f, b)                           \
	if((read(f, b, MAX_INPUT_STR_LEN)) == -1) { \
		perror("read");                           \
		exit(EXIT_FAILURE);                       \
  }                                           \

#define _WRITE(f, b)                          \
	if(write(f, b, strlen(b)) == -1) {          \
		perror("write");                          \
		exit(EXIT_FAILURE);                       \
	}                                           \

#define _CLOSE(f)                             \
	if(close(f) == -1) {                        \
		perror("close");                          \
		exit(EXIT_FAILURE);                       \
	}                                           \

#define _OPEN(p, f, n)                        \
({                                            \
  int64_t fd;                                 \
  if(n) {                                     \
    fd = open(p, f, NEW_FILE_MODE);           \
  }                                           \
  else {                                      \
    fd = open(p, f, NEW_FILE_MODE);           \
  }                                           \
  if(fd == -1) {                              \
    fprintf(stderr, "open(%s): ", p);         \
  	perror("");                               \
  	exit(EXIT_FAILURE);                       \
  }                                           \
  fd;                                         \
})                                            \
