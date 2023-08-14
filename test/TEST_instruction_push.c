void TEST_INSTRUCTION_PUSH_virtual_machine_execute_instruction(void) {
    virtual_machine_execute_instruction(&virtual_machine,
                                        MAKE_INSTRUCTION_PUSH(69));

    assert(69 == virtual_machine.stack[virtual_machine.stack_size - 1] &&
           "ERROR: TEST_virtual_machine_execute_instruction() -> "
           "INSTRUCTION_PUSH");
}
