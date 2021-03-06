#pragma once

#include <stddef.h>
#include <stdint.h>

// Every interrupt handler must specify a code selector. We'll use entry 5
// (5*8=0x28), which is where our bootloader set up a usable code selector for
// 64-bit mode.
#define IDT_CODE_SELECTOR 0x28

// IDT entry types
#define IDT_TYPE_INTERRUPT 0xE
#define IDT_TYPE_TRAP 0xF

// 64-bit IDT Gate Descriptor
typedef struct idt_entry {
    uint16_t offset_0;
    uint16_t selector;
    uint8_t ist : 3;
    uint8_t _unused_0 : 5;
    uint8_t type : 4;
    uint8_t _unused_1 : 1;
    uint8_t dpl : 2;
    uint8_t present : 1;
    uint16_t offset_1;
    uint32_t offset_2;
    uint32_t _unused_2;
} __attribute__((packed)) idt_entry_t;

typedef struct interrupt_context {
    uintptr_t ip;
    uint64_t cs;
    uint64_t flags;
    uintptr_t sp;
    uint64_t ss;
} __attribute__((packed)) interrupt_context_t;

typedef struct idt_record {
    uint16_t size;
    void *base;
} __attribute__((packed)) idt_record_t;

void idt_setup();

/**
 * @brief idt_se_handler adds a handler to idt
 *
 * @param index index of idt
 * @param fn the idt handler to add
 * @param type handler type
 */
void idt_set_handler(uint8_t index, void *fn, uint8_t type);