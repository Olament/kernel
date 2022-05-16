#include "unistd.h"

#include <stdint.h>

#include "syscall.h"

ssize_t write(int fd, const void *buf, size_t count) { return syscall(SYS_write, fd, buf, count); }

ssize_t read(int fd, void *buf, size_t count) { return syscall(SYS_read, fd, buf, count); }

int exec(const char *file_name, const char *argv) { return syscall(SYS_exec, file_name, argv); }

int exit(int status) { return syscall(SYS_exit, status); }