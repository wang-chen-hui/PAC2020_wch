#include <stdio.h>
#include <iostream>
#include <chrono>
#include <omp.h>
using namespace std;

#ifndef _CHECK_RESULT_H
#define _CHECK_RESULT_H

void check_result(const double* check,const double* result,const int K);
#endif