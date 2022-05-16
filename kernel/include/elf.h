#pragma once

#include <stddef.h>
#include <stdint.h>

#include "stivale2.h"

typedef void (*void_function_t)();

/**
 * @brief load the elf format into memory and return entry point
 * of the loaded binary.
 *
 * @param p pointer points to the start of the elf file
 * @param size the size of the elf file, not used
 * @return void_function_t the entry of the loaded binary
 */
void_function_t load(uintptr_t p, size_t size);

/**
 * @brief exec_module load and execute the stivale2 submodule
 *
 * @param module the stivale2 submodule
 */
void exec_module(struct stivale2_module module, const char *argument);