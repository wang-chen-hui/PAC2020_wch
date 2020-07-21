/* 
 * logDataVSPrior is a function to calculate 
 * the accumulation from ABS of two groups of complex data
 * *************************************************************************/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include"check_result.h"
using namespace std;

typedef chrono::high_resolution_clock Clock;

const int m=1638400;	// DO NOT CHANGE!!
const int K=5000;	// DO NOT CHANGE!!
struct COMPLEX
{
    double *re = new double[m];
    double *unre = new double[m];
};

double logDataVSPrior(const COMPLEX* dat, const COMPLEX* pri , const double* ctf, const double* sigRcp,int num,const double disturb0);
void checkfunction(const int K,const double* res);
int main ( int argc, char *argv[] )
{ 
    COMPLEX *dat = new COMPLEX ();
    COMPLEX *pri = new COMPLEX ();
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
        dat->re[i] = dat0;
        dat->unre[i] = dat1;
        pri->re[i] = pri0;
        pri->unre[i] = pri1;
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
        double result = logDataVSPrior(dat,pri,ctf,sigRcp,m,disturb[t]);
        results[t] = result;
        fout << t+1 << ": " << result << endl;
    }
    fout.close();

    auto endTime = Clock::now(); 

    auto compTime = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

    checkfunction(K,results);

    cout << "Computing time=" << compTime.count() << " microseconds" << endl;

    delete[] dat;
    delete[] pri;

    delete[] ctf;
    delete[] sigRcp;
    delete[] disturb;
    delete[] results;
    return EXIT_SUCCESS;
}

double logDataVSPrior(const COMPLEX* dat, const COMPLEX* pri, const double* ctf, const double* sigRcp,int num,const double disturb0)
{
    double result = 0.0;
    for (int i = 0; i <num; i++)
    {
        double retmp = dat->re[i] - disturb0 * ctf[i] * pri->re[i];
        double unretmp = dat->unre[i] - disturb0 * ctf[i] * pri->unre[i];
        result += ( (retmp * retmp + unretmp * unretmp ) * sigRcp[i] );
    }
    return result;
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
