#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define VIRTUAL_MACHINE_CAPACITY 1024

typedef int64_t Word;

typedef enum {
    EXCEPTION_OK = 0,
    EXCEPTION_STACK_OVERFLOW,
    EXCEPTION_STACK_UNDERFLOW,
    EXCEPTION_ILLEGAL_INSTRUCTION,
} Exception;

const char *exception_dump(Exception exception) {
    switch (exception) {
    case EXCEPTION_OK:
        return "EXCEPTION_OK";
    case EXCEPTION_STACK_OVERFLOW:
        return "EXCEPTION_STACK_OVERFLOW";
    case EXCEPTION_STACK_UNDERFLOW:
        return "EXCEPTION_STACK_UNDERFLOW";
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        return "EXCEPTION_ILLEGAL_INSTRUCTION";
    default:
        assert(0 && "exception_dump(): Unreachable");
    }
}

typedef struct {
    Word stack[VIRTUAL_MACHINE_CAPACITY];
    size_t stack_size;
} Virtual_Machine;

typedef enum {
    INSTRUCTION_PUSH,
    INSTRUCTION_PLUS,
    INSTRUCTION_MULTIPLICATION,
    INSTRUCTION_DIVISION,
    INSTRUCTION_MINUS,
} Instruction_Type;

typedef struct {
    Instruction_Type type;
    Word operand;
} Instruction;

const char *instruction_type_string(Instruction instruction) {
    switch (instruction.type) {

    case INSTRUCTION_PUSH:
        return "PUSH";
    case INSTRUCTION_MINUS:
        return "MINUS";
    case INSTRUCTION_DIVISION:
        return "MINUS";
    case INSTRUCTION_MULTIPLICATION:
        return "MULTIPLICATION";
    case INSTRUCTION_PLUS:
        return "PLUS";
    default:
        assert(0 && "instruction_type_string(): Unreachable");
    }
}

Exception virtual_machine_execute_instruction(Virtual_Machine *virtual_machine,
                                              Instruction instruction) {
    printf("(%s) ", instruction_type_string(instruction));

    switch (instruction.type) {

    case INSTRUCTION_PUSH:
        if (virtual_machine->stack_size >= VIRTUAL_MACHINE_CAPACITY) {
            return EXCEPTION_STACK_OVERFLOW;
        }

        virtual_machine->stack[virtual_machine->stack_size] =
            instruction.operand;

        virtual_machine->stack_size++;
        break;

    case INSTRUCTION_PLUS:
        if (virtual_machine->stack_size < 2) {
            return EXCEPTION_STACK_UNDERFLOW;
        }

        virtual_machine->stack[virtual_machine->stack_size - 2] +=
            virtual_machine->stack[virtual_machine->stack_size - 1];

        virtual_machine->stack_size--;
        break;

    case INSTRUCTION_MINUS:
        if (virtual_machine->stack_size < 2) {
            return EXCEPTION_STACK_UNDERFLOW;
        }

        virtual_machine->stack[virtual_machine->stack_size - 2] -=
            virtual_machine->stack[virtual_machine->stack_size - 1];
        virtual_machine->stack_size--;
        break;

    case INSTRUCTION_MULTIPLICATION:
        if (virtual_machine->stack_size < 2) {
            return EXCEPTION_STACK_UNDERFLOW;
        }

        virtual_machine->stack[virtual_machine->stack_size - 2] *=
            virtual_machine->stack[virtual_machine->stack_size - 1];
        virtual_machine->stack_size--;
        break;

    case INSTRUCTION_DIVISION:
        if (virtual_machine->stack_size < 2) {
            return EXCEPTION_STACK_UNDERFLOW;
        }

        virtual_machine->stack[virtual_machine->stack_size - 2] /=
            virtual_machine->stack[virtual_machine->stack_size - 1];
        virtual_machine->stack_size--;
        break;

    default:
        return EXCEPTION_ILLEGAL_INSTRUCTION;
    }

    return EXCEPTION_OK;
}

void virtual_machine_dump(FILE *stream,
                          const Virtual_Machine *virtual_machine) {
    fprintf(stream, "Stack: \n");
    if (virtual_machine->stack_size > 0) {
        for (size_t i = 0; i < virtual_machine->stack_size; ++i) {
            fprintf(stream, "  %lld\n", virtual_machine->stack[i]);
        }
    } else {
        fprintf(stream, "  [empty]\n");
    }
}

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))
#define MAKE_INSTRUCTION_PUSH(value)                                           \
    (Instruction) { .type = INSTRUCTION_PUSH, .operand = value }
#define MAKE_INSTRUCTION_PLUS                                                  \
    (Instruction) { .type = INSTRUCTION_PLUS }
#define MAKE_INSTRUCTION_MINUS                                                 \
    (Instruction) { .type = INSTRUCTION_MINUS }
#define MAKE_INSTRUCTION_MULTIPLICATION                                        \
    (Instruction) { .type = INSTRUCTION_MINUS }
#define MAKE_INSTRUCTION_DIVISION                                              \
    (Instruction) { .type = INSTRUCTION_MINUS }

Virtual_Machine virtual_machine = {0};

void TEST_virtual_machine_execute_instruction() {
    Instruction program[] = {
        MAKE_INSTRUCTION_PUSH(69),
        // MAKE_INSTRUCTION_PUSH(420),
        // MAKE_INSTRUCTION_PLUS,
        // MAKE_INSTRUCTION_PUSH(42),
        // MAKE_INSTRUCTION_MINUS,
    };

    for (size_t i = 0; i < ARRAY_SIZE(program); ++i) {
        Exception exception =
            virtual_machine_execute_instruction(&virtual_machine, program[i]);
        if (exception != EXCEPTION_OK) {
            fprintf(stderr, "Exception activated: %s\n",
                    exception_dump(exception));
            virtual_machine_dump(stderr, &virtual_machine);
            exit(1);
        }
        virtual_machine_dump(stdout, &virtual_machine);
    }

    assert(69 ==
               virtual_machine.stack[virtual_machine.stack_size - 1] &&
           "ERROR: TEST_virtual_machine_execute_instruction() -> "
           "INSTRUCTION_PUSH");

}

int main(void) {
    virtual_machine_dump(stdout, &virtual_machine);
    TEST_virtual_machine_execute_instruction();
    return 0;
}
