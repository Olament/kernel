#pragma once

#include <stddef.h>

typedef long ssize_t;

// write content buf with size count to terminal
ssize_t write(int fd, const void *buf, size_t count);

// read content with size count from keyboard input to buf
ssize_t read(int fd, void *buf, size_t count);

// execute submodule with file_name
int exec(const char *file_name, const char *argv);

// execute init submodule
int exit(int status);
