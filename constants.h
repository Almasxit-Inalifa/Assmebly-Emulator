#ifndef CONSTANTS_H
#define CONSTANTS_H

using namespace std;

static const int NUM_REG = 34;
static const int NUM_PUBLIC_REG = 32;
static const int PRIVATE_REG_FIRST = 32;
static const int PRIVATE_REG_SECOND = 33;

static const int MEMORY_SIZE = 4000; // first 2000 for stack and the second 2000 for allocated memory
static const int RA_REG = 1;
static const int SP_REG = 2;
static const int RET_VAL_REG = 10;


static const int NUM_TESTS = 10;
static const string TESTS_FOLDER_NAME = "tests/";

#endif