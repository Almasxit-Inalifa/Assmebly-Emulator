#include "emulator.h"


Emulator::Emulator() {
    // the first half of the memory is for the stack, the second half - for the heap
    memory = (unsigned char*)malloc(sizeof(unsigned char) * MEMORY_SIZE);
    registers = (int*)malloc(sizeof(int) * NUM_REG);

    memory_p = MEMORY_SIZE - 1;

    registers[0] = 0; // x0 is always 0
    registers[RA_REG] = -1; // x1 is ra and is -1 at the start
    registers[SP_REG] = (MEMORY_SIZE / 2) - 1; // x2 is sp and in pointing at the beginning of the stack

    Emulator::fill_fn_maps();
}

void Emulator:: free_memory() {
    free(registers);
    free(memory);
}


// ALU
void Emulator::add(int dest, int r1, int r2) {
    registers[dest] = registers[r1] + registers[r2];
}

void Emulator::addi(int dest, int r1, int num) {
    registers[dest] = registers[r1] + num;
}

void Emulator::sub(int dest, int r1, int r2) {
    registers[dest] = registers[r1] - registers[r2];
}

void Emulator::mul(int dest, int r1, int r2) {
    registers[dest] = registers[r1] * registers[r2];
}

void Emulator::div(int dest, int r1, int r2) {
    registers[dest] = registers[r1] / registers[r2];
}

void Emulator::my_and(int dest, int r1, int r2) {
    registers[dest] = (registers[r1] != 0 && registers[r2] != 0);
}

void Emulator::my_or(int dest, int r1, int r2) {
    registers[dest] = (registers[r1] != 0 || registers[r2] != 0);
}



// Branch instructions
void Emulator::beq(int r1, int r2, int jump_to) {
    if (registers[r1] == registers[r2]) jump(jump_to);
}

void Emulator::bne(int r1, int r2, int jump_to) {
    if (registers[r1] != registers[r2]) jump(jump_to);
}

void Emulator::blt(int r1, int r2, int jump_to) {
    if (registers[r1] < registers[r2]) jump(jump_to);
}

void Emulator::bgt(int r1, int r2, int jump_to) {
    if (registers[r1] > registers[r2]) jump(jump_to);
}

void Emulator::ble(int r1, int r2, int jump_to) {
    if (registers[r1] <= registers[r2]) jump(jump_to);
}

void Emulator::bge(int r1, int r2, int jump_to) {
    if (registers[r1] >= registers[r2]) jump(jump_to);
}


// Function callers
void Emulator::jump(int jump_to) {
    cur_line = jump_to + 1;
}

    // Call Instruction
void Emulator::call(string function_name) {
    int address = function_addresses.find(function_name) -> second;
    registers[RA_REG] = cur_line;
    cur_line = address + 1;
}

void Emulator::jalr(int r) {
    registers[RA_REG] = cur_line;
    cur_line = registers[r] + 1;
}

void Emulator::jr(int r) {
    cur_line = registers[r];
}

// Load and store instructions

void Emulator::lw(int r, int addr) {
    unsigned char first_byte = memory[addr];
    unsigned char second_byte = memory[addr + 1];
    unsigned char third_byte = memory[addr + 2];
    unsigned char fourth_byte = memory[addr + 3];

    int res = (int)((fourth_byte << 24) | (third_byte << 16) | (second_byte << 8) | first_byte);

    registers[r] = res;
}

void Emulator::lh(int r, int addr) {
    unsigned char first_byte = memory[addr];
    unsigned char second_byte = memory[addr + 1];

    int res = (int)((second_byte << 8) | first_byte);

    registers[r] = res;
}

void Emulator::li(int r, int num) {
    registers[r] = num;
}

void Emulator::lb(int r, int addr) {
    unsigned char first_byte = memory[addr];
    registers[r] = (int)((char)first_byte);
}

void Emulator::sw(int r, int addr) {
    int num = registers[r];
    unsigned char first_byte = num & 0xFF;
    unsigned char second_byte = (num >> 8) & 0xFF;
    unsigned char third_byte = (num >> 16) & 0xFF;
    unsigned char fourth_byte = (num >> 24) & 0xFF;

    memory[addr] = first_byte;
    memory[addr + 1] = second_byte;
    memory[addr + 2] = third_byte;
    memory[addr + 3] = fourth_byte;
}

void Emulator::sh(int r, int addr) {
    int num = registers[r];
    unsigned char first_byte = num & 0xFF;
    unsigned char second_byte = (num >> 8) & 0xFF;

    memory[addr] = first_byte;
    memory[addr + 1] = second_byte;
}

void Emulator::sb(int r, int addr) {
    memory[addr] = (unsigned char)(registers[r] & 0xFF);
}

void Emulator::move(int dest, int r) {
    registers[dest] = registers[r];
}


// my functions
void Emulator:: my_print() {
    lw(PRIVATE_REG_FIRST, registers[SP_REG]);
    int char_ptr_addr = registers[PRIVATE_REG_FIRST];
    unsigned char cur = memory[char_ptr_addr];

    int count = 0;
    while(cur != '\0') {
        cout << cur;
        count++;
        cur = memory[char_ptr_addr + count];
    }

    cout << endl;
}

void Emulator::my_strlen() {
    lw(PRIVATE_REG_FIRST, registers[SP_REG]);
    int char_ptr_addr = registers[PRIVATE_REG_FIRST];
    unsigned char cur = memory[char_ptr_addr];

    int count = 0;
    while(cur != '\0') {
        count++;
        cur = memory[char_ptr_addr + count];
    }
    
    registers[RET_VAL_REG] = count;
}

void Emulator::my_malloc() {
    lw(PRIVATE_REG_FIRST, registers[SP_REG]);
    int size = registers[PRIVATE_REG_FIRST];

    if (memory_p - size >= MEMORY_SIZE / 2) {
        memory_p -= size;
        registers[RET_VAL_REG] = memory_p;
        allocated_ptr_sizes.insert(make_pair(memory_p, size));
    } else {
        cerr << "MEMORY OVERFLOW" << endl;
        std::exit(EXIT_FAILURE);
    }
}

void Emulator::my_realloc() {
    lw(PRIVATE_REG_FIRST, registers[SP_REG]);
    lw(PRIVATE_REG_SECOND, registers[SP_REG] + 4);

    int old_addr = registers[PRIVATE_REG_FIRST];
    int size = registers[PRIVATE_REG_SECOND];

    if (memory_p - size >= MEMORY_SIZE / 2) {
        memory_p -= size;
        registers[RET_VAL_REG] = memory_p;

        int old_size = allocated_ptr_sizes.find(old_addr) -> second;
        for (int i = 0; i < old_size; i++) {
            memory[memory_p + i] = memory[old_addr + i];
        }

        allocated_ptr_sizes.insert(make_pair(memory_p, size));
    } else {
        cerr << "MEMORY OVERFLOW" << endl;
        std::exit(EXIT_FAILURE);
    }

    Emulator::my_free();
}

void Emulator::my_free() {
    lw(PRIVATE_REG_FIRST, registers[SP_REG]);
    allocated_ptr_sizes.erase(registers[PRIVATE_REG_FIRST]);
}

int Emulator::load_cur_line() {
    return cur_line;
}

void Emulator::store_cur_line(int new_cur_line) {
    cur_line = new_cur_line;
}

int Emulator::load_pc() {
    return registers[RA_REG];
}

void Emulator::store_pc(int new_ra) {
    registers[RA_REG] = new_ra;
}


int Emulator::get_register(int r) {
    return registers[r];
}

int Emulator::get_function_address(string func_name) {
    return function_addresses.find(func_name) -> second;
}

string Emulator::get_line(int line_num) {
    return lines[line_num];
}

void Emulator::add_func_addr(string line) {
    function_addresses.insert(make_pair(line.substr(0, line.length() - 1), lines.size()));
}

void Emulator::add_line(string line) {
    lines.push_back(line);
}

map<string, void (Emulator::*)(int, int, int)> Emulator::get_alu_and_branch_function_map() {
    return alu_and_branch_function_map;
}

map<string, void (Emulator::*)(int, int)> Emulator::get_load_and_store_function_map() {
    return load_and_store_function_map;
}

void Emulator::fill_fn_maps() {
    alu_and_branch_function_map["blt"] = &Emulator::blt;
    alu_and_branch_function_map["bne"] = &Emulator::bne;
    alu_and_branch_function_map["bgt"] = &Emulator::bgt;
    alu_and_branch_function_map["beq"] = &Emulator::beq;
    alu_and_branch_function_map["bge"] = &Emulator::bge;
    alu_and_branch_function_map["ble"] = &Emulator::ble;

    alu_and_branch_function_map["add"] = &Emulator::add;
    alu_and_branch_function_map["sub"] = &Emulator::sub;
    alu_and_branch_function_map["div"] = &Emulator::div;
    alu_and_branch_function_map["mul"] = &Emulator::mul;

    alu_and_branch_function_map["and"] = &Emulator::my_and;
    alu_and_branch_function_map["or"] = &Emulator::my_or;

    load_and_store_function_map["lw"] = &Emulator::lw;
    load_and_store_function_map["lh"] = &Emulator::lh;
    load_and_store_function_map["lb"] = &Emulator::lb;
    load_and_store_function_map["sw"] = &Emulator::sw;
    load_and_store_function_map["sh"] = &Emulator::sh;
    load_and_store_function_map["sb"] = &Emulator::sb;
}