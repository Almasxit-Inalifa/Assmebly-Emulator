#include "executor.h"

Executor::Executor() {

}

void Executor::free_memory() {
    e.free_memory();
}

int Executor::run_file() {
    e.store_cur_line(e.get_function_address("main") + 1);

    while(e.load_cur_line() != -1) {
        execute_line(e.get_line(e.load_cur_line()));
    }
    
    return e.get_register(RET_VAL_REG);;
}

void Executor::execute_line(string line) {
    e.store_cur_line(e.load_cur_line() + 1);
    int len = line.length();
    if (len == 0 || line[0] == '#' || line[0] == '.' || line[len - 1] == ':') return;

    if (line == "ret") {
        e.store_cur_line(e.load_pc());
        e.store_pc(-1);

        return;
    }

    vector<string> words = get_words(line);

    if (is_ALU(words[0])) {
        alu(words);
    } else if (is_compare(words[0])) {
        compare(words);
    } else if (is_load_or_store(words[0])) {
        load_or_store(words);
    } else if (is_function_caller(words[0])) {
        func(words);
    } else {
        cerr << "UNKNOWN INPUT. " << line << "function not found." << endl;
        std::exit(EXIT_FAILURE); 
    }
}


vector<string> Executor::get_words(string line) {
    vector<string> words;
    istringstream iss(line);
    string word;


    while (getline(iss, word, ',')) {
        stringstream sub_ss(word);
        string sub_token;
        
        while (std::getline(sub_ss, sub_token, ' ')) {
            if (!sub_token.empty()) {
                words.push_back(sub_token);
            }
        }
    }

    return words;
}

void Executor::alu(vector<string> words) {
    if (words[0] == "addi") {
        e.addi(extract_register_num(words[1]),
        extract_register_num(words[2]), 
        stoi(words[3]));
        return;
    }

    map<string, void (Emulator::*)(int, int, int)> function_map = e.get_alu_and_branch_function_map();
    (e.*(function_map.find(words[0])->second))
            (extract_register_num(words[1]),
             extract_register_num(words[2]), 
             extract_register_num(words[3]));
}


void Executor::compare(vector<string> words) {
    map<string, void (Emulator::*)(int, int, int)> function_map = e.get_alu_and_branch_function_map();
    (e.*(function_map.find(words[0])->second))
            (extract_register_num(words[1]),
             extract_register_num(words[2]), 
             e.get_function_address(words[3]));
}


void Executor::func(vector<string> words) {
    if (words[0] == "j") {
        e.jump(e.get_function_address(words[1]));
    } else if (words[0] == "call") {
        if (is_my_fn(words[1])) {
            if (words[1] == "strlen") {
                e.my_strlen();
            } else if (words[1] == "malloc") {
                e.my_malloc();
            } else if (words[1] == "realloc") {
                e.my_realloc();
            } else if (words[1] == "print") {
                e.my_print();
            } else if (words[1] == "free") {
                e.my_free();
            }
        } else {
            e.call(words[1]);
        }
    } else if (words[0] == "jalr") {
        e.jalr(extract_register_num(words[1]));
    } else if (words[0] == "jr") {
        e.jr(extract_register_num(words[1]));
    } else {
        cerr << "UNKNOWN INPUT" << endl;
        std::exit(EXIT_FAILURE);
    }
}

void Executor::load_or_store(vector<string> words) {
    if (words[0] == "li") {
        e.li(extract_register_num(words[1]), stoi(words[2]));
    } else if (words[0] == "mv") {
        e.move(extract_register_num(words[1]), extract_register_num(words[2]));
    } else if (words[0] == "la") {
        e.li(extract_register_num(words[1]), e.get_function_address(words[2]));
    } else {
        map<string, void (Emulator::*)(int, int)> function_map = e.get_load_and_store_function_map();
        (e.*(function_map.find(words[0])->second))
                (extract_register_num(words[1]), 
                 extact_addr(words[2]));
    }
}


int Executor::extract_register_num(string r) {
    if (r == "ra") return RA_REG; // ra == x1
    if (r == "sp") return SP_REG; // sp == x2

    if (r[0] == 't') {
        int num_part = stoi(r.substr(1, 2));
        
        if (num_part <= 2) {
            return 5 + num_part; // t0 = x5, t1 = x6, t2 = x7
        }

        return 25 + num_part; // t3 = x28, t4 = x29...
    }

    if (r[0] == 'a') return 10 + stoi(r.substr(1, 2)); // a0 = x10, a1 = x11...

    assert(r[0] == 'x');
    
    int res = stoi(r.substr(1, 2));

    assert(res < NUM_PUBLIC_REG);

    return res;
}

int Executor::extact_addr(string s) {
    if (s[0] == 'x' || s[0] == 'a') return extract_register_num(s);

    string offset;
    int brace_ind;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') {
            brace_ind = i;
            break;
        } 
        offset += s[i];
    }

    int regNum = extract_register_num(s.substr(brace_ind + 1, s.length() - 2 - brace_ind));
    assert(regNum < NUM_PUBLIC_REG);

    int res = e.get_register(regNum) + stoi(offset);

    assert(res >= 0 && res < MEMORY_SIZE);

    return res;
}

void Executor::read_line(string line) {
    int len = line.length();
    if (line[len - 1] == ':') {
        e.add_func_addr(line);
    }

    e.add_line(line);
}

bool Executor::is_ALU(string key_word) {
    string tmp[] = {"add", "addi", "sub", "mul", "div", "and", "or"};
    set<string> alus(tmp, tmp + sizeof(tmp) / sizeof(tmp[0]));
    return alus.find(key_word) != alus.end();
}

bool Executor::is_compare(string key_word) {
    string tmp[] = {"blt", "ble", "beq", "bne", "bgt", "bge"};
    set<string> compares(tmp, tmp + sizeof(tmp) / sizeof(tmp[0]));
    return compares.find(key_word) != compares.end();
}

bool Executor::is_load_or_store(string key_word) {
    string tmp[] = {"lw", "lh", "lb", "li", "sw", "sh", "sb", "mv", "la"};
    set<string> ls(tmp, tmp + sizeof(tmp) / sizeof(tmp[0]));
    return ls.find(key_word) != ls.end();
}

bool Executor::is_function_caller(string key_word) {
    string tmp[] = {"j", "call", "jalr", "jr"};
    set<string> calls(tmp, tmp + sizeof(tmp) / sizeof(tmp[0]));
    return calls.find(key_word) != calls.end();
}

bool Executor::is_my_fn(string key_word) {
    string tmp[] = {"strlen", "malloc", "realloc", "print", "free"};
    set<string> fns(tmp, tmp + sizeof(tmp) / sizeof(tmp[0]));
    return fns.find(key_word) != fns.end();
}

