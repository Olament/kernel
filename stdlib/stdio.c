#include "stdio.h"

#include <stdint.h>

#include "stdarg.h"
#include "string.h"

#define STDOUT 1

// Print a single character to the terminal
void print_c(char c) { write(STDOUT, &c, 1); }

// Print a string to the terminal
void print_s(const char *str) {
    size_t len = strlen(str);
    write(STDOUT, str, len);
}

// only support up to Hexadecimal
char radix_map(uint8_t radix) { return radix <= 9 ? '0' + radix : 'a' + (radix - 10); }

// print number respect to its radix
void print_r(uint64_t value, uint8_t radix) {
    uint64_t n = 1;

    // corner case
    if (value == 0) {
        print_c('0');
        return;
    }

    while ((n * radix <= value) && (n * radix > n)) {
        n *= radix;
    }

    while (n > 0) {
        print_c(radix_map(value / n));
        value %= n;
        n /= radix;
    }
}

// Print an unsigned 64-bit integer value to the terminal in decimal notation
// (no leading zeros please!)
void print_d(uint64_t value) { print_r(value, 10); }

// Print an unsigned 64-bit integer value to the terminal in lowercase
// hexadecimal notation (no leading zeros or “0x” please!)
void print_x(uint64_t value) { print_r(value, 16); }

// Print the value of a pointer to the terminal in lowercase hexadecimal with
// the prefix “0x”
void print_p(void *ptr) {
    print_s("0x");
    print_x((uint64_t)ptr);
}

void printf(const char *format, ...) {
    // Start processing variadic arguments
    va_list args;
    va_start(args, format);

    // Loop until we reach the end of the format string
    size_t index = 0;
    while (format[index] != '\0') {
        // Is the current charater a '%'?
        if (format[index] == '%') {
            // Yes, print the argument
            index++;
            switch (format[index]) {
                case '%':
                    print_c('%');
                    break;
                case 'c':
                    print_c(va_arg(args, int));
                    break;
                case 's':
                    print_s(va_arg(args, char *));
                    break;
                case 'd':
                    print_d(va_arg(args, uint64_t));
                    break;
                case 'x':
                    print_x(va_arg(args, int64_t));
                    break;
                case 'p':
                    print_p(va_arg(args, void *));
                    break;
                default:
                    print_s("<not supported>");
            }
        } else {
            // No, just a normal character. Print it.
            print_c(format[index]);
        }
        index++;
    }

    // Finish handling variadic arguments
    va_end(args);
}