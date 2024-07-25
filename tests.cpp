#include "tests.h"

pair<int, string> test_one() {
    int res = (int)((char)((short)-123643));
    string print_val = "";
    return make_pair(res, print_val);
}

pair<int, string> test_two() {
    int res = (16 + 34);
    string print_val = "";
    return make_pair(res, print_val);
}

int factorial(int n) {
    if (n <= 1)
        return 1;
    else
        return n * factorial(n - 1);
}

pair<int, string> test_three() {
    int num = 12;

    int res = factorial(num);
    string print_val = "";

    return make_pair(res, print_val);
}

int fibonacci(int n) {
    int i;
    int first = 0;
    int second = 1;
    int next;

    for (i = 0; i < n; ++i) {
        next = first + second;
        first = second;
        second = next;
    }

    return first;
}

pair<int, string> test_four() {
    int num = 19;
    int res = fibonacci(num);

    string print_val = "";

    return make_pair(res, print_val);
}

void castFn(int num, Data *d) {
    d->c = (char)num;
    d->s[0] = (short)num;
    d->s[1] = (short)num;
    d->i = num;
}

int StructSum(int num, PopulateFn popFn) {
    Data d;
    popFn(num, &d);
    return d.c + d.s[0] + d.s[0] + d.s[1] + d.i;
}

pair<int, string> test_five() {
    int num = 17246;
    int res = StructSum(num, castFn);

    string print_val = "";

    return make_pair(res, print_val);
}

int nthPrime(int n) {
    const int MAX_SIZE = 1000; // Adjust this as needed
    bool *isPrime = (bool *)malloc(MAX_SIZE);

    // Initialize all numbers as prime
    for (int i = 2; i < MAX_SIZE; ++i)
        isPrime[i] = true;

    // Apply Sieve of Eratosthenes
    for (int p = 2; p * p < MAX_SIZE; ++p) {
        if (isPrime[p]) {
            for (int i = p * p; i < MAX_SIZE; i += p)
                isPrime[i] = false;
        }
    }

    // Count prime numbers
    int count = 0, prime = 0;
    for (int i = 2; i < MAX_SIZE; ++i) {
        if (isPrime[i]) {
            count++;
            if (count == n) {
                prime = i;
                break;
            }
        }
    }

    free(isPrime);

    return prime;
}

pair<int, string> test_six() {
    int num = 17;
    int res = nthPrime(num);
    string print_val = "";
    return make_pair(res, print_val);
}

pair<int, string> test_seven() {
    char *s = (char *)malloc(4);
    s[0] = 'L';
    s[1] = 'E';
    s[2] = 'O';
    s[3] = '\0';

    s = (char *)realloc(s, 8);
    s[3] = 'N';
    s[4] = 'A';
    s[5] = 'R';
    s[6] = 'D';
    s[7] = '\0';

    s = (char *)realloc(s, 10);
    s[7] = 'O';
    s[8] = '\0';

    // print s
    int res = strlen(s);
    string print_val(s);
    free(s);
    return make_pair(res, print_val);
}

pair<int, string> test_eight() {
    char *letters = (char *)malloc(10);
    letters[0] = 'A';
    letters[1] = 'B';
    letters[2] = 'C';
    letters[3] = '\0';

    int res = strlen(letters);
    char *rev_str = rev_string(letters);
    string print_val(rev_str);
    free(letters);
    free(rev_str);

    return make_pair(res, print_val);
}

char *rev_string(char *letters) {
    char *our = (char *)malloc(4);
    our[0] = letters[2];
    our[1] = letters[1];
    our[2] = letters[0];
    our[3] = '\0';

    // print(our);
    int len = strlen(our);
    return our;
}

int rev(char *letters) {
    char *our = (char *)malloc(4);
    our[0] = letters[2];
    our[1] = letters[1];
    our[2] = letters[0];
    our[3] = '\0';

    // print(our);
    int len = strlen(our);
    free(our);
    return len;
}

pair<int, string> test_nine() {
    char *arr = (char *)malloc(8);
    arr[0] = '\0';
    arr[1] = 'e';
    arr[2] = 'd';
    arr[3] = 'c';
    arr[4] = 'b';
    arr[5] = 'a';

    int n = 6;
    int elemSize = 1;
    reverse(arr, n, elemSize);

    // print(arr);
    int res = strlen(arr);
    string print_val(arr);
    free(arr);
    return make_pair(res, print_val);
}

void reverse(char *arr, int n, int elemSize) {
    for (int i = 0; i < n / 2; i++) {
        int j = n - i - 1;
        char *to = arr + i * elemSize;
        char *from = arr + j * elemSize;
        for (int k = 0; k < elemSize; k++) {
            char c = from[k];
            arr[j] = to[k];
            arr[i] = c;
        }
    }
}

bool lower(char c) {
    return (c >= 'a' && c <= 'z');
}

char *EmployeWithHighestSalary(int num_employees, Employee *employees, IsLower is_lower) {
    int len = strlen(employees[num_employees - 1].name);
    char *upper = (char *)malloc(len * sizeof(char) + 1);
    for (int i = 0; i < len; ++i) {
        char ch = employees[num_employees - 1].name[i];
        if (is_lower(ch)) {
            upper[i] = 'A' + ch - 'a';
        }
    }
    upper[len] = '\0';
    return upper;
}

pair<int, string> test_ten() {
    int num_employees = 5;
    Employee *employees = (Employee *)malloc(sizeof(Employee) * num_employees);
    for (int i = 0; i < num_employees; i++) {
        employees[i].salary = i * 1200;
        employees[i].name = (char *)malloc(5);
        employees[i].name[0] = 'm';
        employees[i].name[1] = 'a';
        employees[i].name[2] = 'r';
        employees[i].name[3] = 'k';
        employees[i].name[4] = '\0';
    }

    char *ours = EmployeWithHighestSalary(num_employees, employees, lower);
    // print(ours)

    int res = MEMORY_SIZE - 71; // alloced memory should be 70
    string print_val(employees[0].name);
    transform(print_val.begin(), print_val.end(), print_val.begin(), ::toupper);

    for (int i = 0; i < num_employees; i++) {
        free(employees[i].name);
    }

    free(employees);
    free(ours);

    return make_pair(res, print_val);
}
