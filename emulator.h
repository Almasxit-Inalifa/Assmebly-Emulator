#ifndef EMULATOR_H
#define EMULATOR_H

#include <iostream>
#include <functional>
#include <cstring>
#include <vector>
#include <map>
#include "assert.h"
#include <stdint.h>
#include <functional>
#include <cerrno>
#include "constants.h"

class Emulator {

typedef void (*FunctionPtr)(int, int, int);

public:
    Emulator();
    void free_memory();

    // ALU 
    void add(int dest, int r1, int r2);
    void addi(int dest, int r1, int num);
    void sub(int dest, int r1, int r2);
    void mul(int dest, int r1, int r2);
    void div(int dest, int r1, int r2);

    void my_and(int dest, int r1, int r2);
    void my_or(int dest, int r1, int r2);

    // branch instructions
    void beq(int r1, int r2, int jump_to);
    void bne(int r1, int r2, int jump_to);
    void blt(int r1, int r2, int jump_to);
    void bgt(int r1, int r2, int jump_to);
    void ble(int r1, int r2, int jump_to);
    void bge(int r1, int r2, int jump_to);


    // load and store 
    void lw(int r, int addr);
    void lh(int r, int addr);
    void lb(int r, int addr);
    void li(int r, int num);
    void sw(int r, int addr);
    void sh(int r, int addr);
    void sb(int r, int addr);
    void move(int dest, int r);

    // function callers
    void call(string function_name);
    void jump(int jump_to);
    void jalr(int addr);
    void jr(int r);

    // my custom functions
    void my_print();
    void my_strlen();
    void my_malloc();
    void my_realloc();
    void my_free();

    // additional functions
    int load_cur_line();
    void store_cur_line(int new_cur_line);

    int load_pc();
    void store_pc(int new_ra);

    int get_register(int r);
    int get_function_address(string func_name);
    string get_line(int line_num);
    void add_func_addr(string line);
    void add_line(string line);

    map<string, void (Emulator::*)(int, int, int)> get_alu_and_branch_function_map();
    map<string, void (Emulator::*)(int, int)> get_load_and_store_function_map();

private:
    // Helper functions
    void fill_fn_maps();

    // global variables

    int cur_line;
    int memory_p;   

    // data

    int* registers; 
    unsigned char* memory;

    map<string, int> function_addresses;
    map<int, int> allocated_ptr_sizes;
    vector<string> lines;             

    map<string, void (Emulator::*)(int, int, int)> alu_and_branch_function_map;
    map<string, void (Emulator::*)(int, int)> load_and_store_function_map;
};

#endif 
