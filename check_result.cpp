#include <stdio.h>
#include <iostream>
#include <chrono>
#include <omp.h>
#include "check_result.h"
#include <cmath>
#include<iomanip>
using namespace std;

void check_result(const double* check, const double* result, const int K)
{
    int i = 0;
    double EPS;
    EPS = 1.0/100000.0*1.0e+15;
    for(i;i < K; i++)
    {
        if(fabs(check[i]-result[i])<=EPS)
        {
            continue;
        }
        else
        {
            cout<<"The result is wrong" << endl;
            break;
        }
    }
    if(i == K)
        cout<<"The result is right" << endl;
    return;
}