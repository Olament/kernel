#include "syscall.h"

#include "elf.h"
#include "gdt.h"
#include "keyboard.h"
#include "kstdio.h"
#include "page.h"

typedef long ssize_t;  // signed size_t
typedef long long off_t;

static struct stivale2_struct_tag_modules *modules;

void syscall_init(struct stivale2_struct_tag_modules *mod) {
    modules = mod;
    idt_set_handler(0x80, syscall_entry, IDT_TYPE_TRAP);
}

ssize_t sys_read(int fd, void *buf, size_t count) {
    size_t index = 0;
    char *buffer = (char *)buf;

    while (index < count) {
        char ch = kgetc();

        if (ch == 8) {  // backspace
            index = index == 0 ? 0 : index - 1;
        } else {
            buffer[index] = ch;
            index += 1;
        }
    }

    return index;
}

ssize_t sys_write(int fd, const void *buf, size_t count) {
    char *buffer = (char *)buf;
    size_t write_count = 0;

    for (size_t i = 0; i < count; i++) {
        kprintf("%c", buffer[i]);
        write_count += 1;
    }

    return write_count;
}

intptr_t sys_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
    uintptr_t root = read_cr3();
    intptr_t start = ((intptr_t)addr / PAGE_SIZE) * PAGE_SIZE;  // left-align the address
    length += ((intptr_t)addr - start);

    for (size_t offset = 0; offset < length; offset += PAGE_SIZE) {
        if (!vm_map(root, start + offset, true, true, true)) {
            kprintf("mmap: vm_map failed!\n");
        }
    }

    return start;
}

int sys_exec(const char *file_name, const char *arg) {
    for (uint64_t i = 0; i < modules->module_count; i++) {
        struct stivale2_module module = modules->modules[i];
        if (strcmp(module.string, file_name) == 0) {
            exec_module(module, arg);
            return 0;
        }
    }
    return -1;
}

int sys_exit(int status) {
    for (uint64_t i = 0; i < modules->module_count; i++) {
        struct stivale2_module module = modules->modules[i];
        if (strcmp(module.string, "init") == 0) {
            exec_module(module, NULL);
            return 0;
        }
    }
    return -1;
}

int syscall_handler(uint64_t nr, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3,
                    uint64_t arg4, uint64_t arg5) {
    switch (nr) {
        case SYS_read:
            return sys_read(arg0, arg1, arg2);
        case SYS_write:
            return sys_write(arg0, arg1, arg2);
        case SYS_mmap:
            return sys_mmap(arg0, arg1, arg2, arg3, arg4, arg5);
        case SYS_exec:
            return sys_exec(arg0, arg1);
        case SYS_exit:
            return sys_exit(arg0);
        default:
            return -1;
    }
}