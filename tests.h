#ifndef TESTS_H
#define TESTS_H

#include <fstream>
#include "executor.h"
#include <string>

// test 5
typedef struct {
    char c;
    short s[2];
    int i;
} Data;

typedef void (*PopulateFn)(int num, Data *d);

// test 10
typedef bool(*IsLower)(char);

typedef struct {
   int salary;  
   char* name;
} Employee;

// helper functions

int factorial(int n);
int fibonacci(int n);
void castFn (int num, Data* d);
int StructSum(int num, PopulateFn popFn);
bool lower(char c);
char* EmployeWithHighestSalary(int num_employees, Employee* employees, IsLower is_lower);
int nthPrime(int n);
void reverse(char *arr, int n, int elemSize);
int rev(char* letters);
char* rev_string(char* letters);

pair<int, string> test_one();
pair<int, string> test_two();
pair<int, string> test_three();
pair<int, string> test_four();
pair<int, string> test_five();
pair<int, string> test_six();
pair<int, string> test_seven();
pair<int, string> test_eight();
pair<int, string> test_nine();
pair<int, string> test_ten();

#endif

