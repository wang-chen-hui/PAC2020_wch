/* 
 * logDataVSPrior is a function to calculate 
 * the accumulation from ABS of two groups of complex data
 * *************************************************************************/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <complex>
#include <chrono>
#include <omp.h>
#include"check_result.h"
using namespace std;

typedef complex<double> Complex;
typedef chrono::high_resolution_clock Clock;

const int m=1638400;	// DO NOT CHANGE!!
const int K=5000;	// DO NOT CHANGE!!

double logDataVSPrior(const double* datre, const double* prire, const double* datunre, const double* priunre, const double* ctf, const double* sigRcp, const int num, const double disturb0);
void checkfunction(const int K,const double* res);
int main ( int argc, char *argv[] )
{ 
    double *datre = new double[m];
    double *prire = new double[m];
    double *datunre = new double[m];
    double *priunre = new double[m];
    double *ctf = new double[m];
    double *sigRcp = new double[m];
    double *disturb = new double[K];
    double *results = new double[K];
    double dat0, dat1, pri0, pri1, ctf0, sigRcp0;

    /***************************
     * Read data from input.dat
     * *************************/
    ifstream fin;

    fin.open("input.dat");
    if(!fin.is_open())
    {
        cout << "Error opening file input.dat" << endl;
        exit(1);
    }
    int i=0;
    while( !fin.eof() ) 
    {
        fin >> dat0 >> dat1 >> pri0 >> pri1 >> ctf0 >> sigRcp0;
        datre[i] = dat0;
        prire[i] = pri0;
        datunre[i] = dat1;
        priunre[i] = pri1;
        ctf[i] = ctf0;
        sigRcp[i] = sigRcp0;
        i++;
        if(i == m) break;
    }
    fin.close();

    fin.open("K.dat");
    if(!fin.is_open())
    {
	cout << "Error opening file K.dat" << endl;
	exit(1);
    }
    i=0;
    while( !fin.eof() )
    {
	fin >> disturb[i];
	i++;
	if(i == K) break;
    }
    fin.close();

    /***************************
     * main computation is here
     * ************************/
    auto startTime = Clock::now(); 

    ofstream fout;
    fout.open("result.dat");
    if(!fout.is_open())
    {
         cout << "Error opening file for result" << endl;
         exit(1);
    }

    for(unsigned int t = 0; t < K; t++)
    {
        double result = logDataVSPrior(datre, prire,datunre,priunre,ctf, sigRcp, m, disturb[t]);
        results[t] = result;
        fout << t+1 << ": " << result << endl;
    }
    fout.close();

    auto endTime = Clock::now(); 

    auto compTime = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

    checkfunction(K,results);

    cout << "Computing time=" << compTime.count() << " microseconds" << endl;

    delete[] datre;
    delete[] prire;
    delete[] datunre;
    delete[] priunre;


    delete[] ctf;
    delete[] sigRcp;
    delete[] disturb;
    delete[] results;
    return EXIT_SUCCESS;
}

double logDataVSPrior(const double* datre, const double* prire, const double* datunre, const double* priunre, const double* ctf, const double* sigRcp, const int num, const double disturb0)
{
    double result = 0.0;
    #pragma omp parallel for reduction(+:result),schedule(static,num/omp_get_num_threads())
    for (int i = 0; i < num; i++)
    {
        double retmp = datre[i] - ctf[i] * prire[i];
        double unretmp = datunre[i] - ctf[i] * priunre[i];
        result += ( (retmp * retmp + unretmp * unretmp ) * sigRcp[i] );
    }
    return result*disturb0;
}

void checkfunction(const int K,const double* res)
    {   
        ifstream fin;
        fin.open("check.dat");
        if(!fin.is_open())
        {
            cout << "Error opening file check.dat" << endl;
            exit(1);
        }
        int i=0;
        double *check = new double [K];
        double dat;
        int nouse;
        char a;
        while( !fin.eof() ) 
        {
            fin >> nouse >> a >> dat;
            check[i] = dat;
            i++;
            if(i == K) break;
        }
        fin.close();

        check_result(check,res,K);
        delete[] check;
        return;
    }
