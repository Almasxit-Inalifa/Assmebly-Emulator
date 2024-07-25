#include "main.h"

int main() {
    int answers[NUM_TESTS];
    string prints[NUM_TESTS];

    calculate_answers_in_c(answers, prints);

    for (int i = 0; i < NUM_TESTS; i++) {
        string file_name = TESTS_FOLDER_NAME + "test" + to_string(i + 1) + ".txt";
        Executor e;
        read_file(file_name, e);

        int emulatorRes = e.run_file();

        if (emulatorRes == answers[i]) {
            cout << "TEST " << i + 1 << " SUCCESS";
            if (prints[i] != "") {
                cout << " (SHOULD PRINT " << prints[i] << ")";
            }
            cout << endl;
        } else {
            cout << "TEST " << i + 1 << " FAIL" << endl;
            cout << "EXPECTED " << answers[i] << " but got " << emulatorRes << endl;
        }

        e.free_memory();
    }
}

void read_file(string file_name, Executor& e) {
    string line;
    ifstream file(file_name);
    if (file.is_open()) {
        while (getline(file, line)) {
            transform(line.begin(), line.end(), line.begin(), ::tolower);
            e.read_line(line);
        }
        file.close();
    }
}


void calculate_answers_in_c(int answers[], string prints[]) {
    vector<pair<int, string> > answers_and_prints(NUM_TESTS);

    answers_and_prints[0] = test_one();
    answers_and_prints[1] = test_two();
    answers_and_prints[2] = test_three();
    answers_and_prints[3] = test_four();
    answers_and_prints[4] = test_five();
    answers_and_prints[5] = test_six();
    answers_and_prints[6] = test_seven();
    answers_and_prints[7] = test_eight();
    answers_and_prints[8] = test_nine();
    answers_and_prints[9] = test_ten();

    for (int i = 0; i < NUM_TESTS; i++) {
        answers[i] = answers_and_prints[i].first;
        prints[i] = answers_and_prints[i].second;
    }
}

