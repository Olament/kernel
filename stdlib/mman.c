#include "mman.h"

#include "syscall.h"

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
    return syscall(SYS_mmap, addr, length, prot, flags, fd, offset);
}