#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "emulator.h"
#include <functional>
#include <set>
#include <cctype>
#include <sstream>
#include <initializer_list>
#include <cerrno>

class Executor {
    public:
        Executor();
        void free_memory();

        int run_file();
        void execute_line(string line);
        void read_line(string line);
    private:
        Emulator e;

        bool is_ALU(string line);
        bool is_compare(string line);
        bool is_load_or_store(string line);
        bool is_function_caller(string line);
        bool is_my_fn(string key_word);

        void alu(vector<string> words);
        void compare(vector<string> words);
        void load_or_store(vector<string> words);
        void func(vector<string> words);

        int extract_register_num(string r);
        int extact_addr(string s);
        vector<string> get_words(string line);
};

#endif