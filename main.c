#include <stdio.h>
#include <stdint.h>

#define VIRTUAL_MACHINE_CAPACITY 1024

typedef int64_t Word;

typedef struct {
    Word stack[VIRTUAL_MACHINE_CAPACITY];
    size_t stack_size;
} Virtual_Machine;

typedef enum {
    INTRUCTION_PUSH,
    INTRUCTION_PLUS,
} Intruction_Type;

typedef struct {
    Intruction_Type type;
    Word operand;
} Intruction;

Virtual_Machine virtual_machine = {0};

int main(void) {
    printf("%zu\n", virtual_machine.stack_size);
    return 0;
}
