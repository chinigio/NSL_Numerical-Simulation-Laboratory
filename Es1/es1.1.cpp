#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions

#include <vector>

using namespace std;

int main (){
    // Random number generator  initialization
    Random rnd;
    
    int M = 100000;            // Total number of throws
    int N = 100;                 // Number of blocks
    int L =int(M/N);            // Number of throws in each block, please use for M a multiple of N

    // evaluation of random number
    vector <double> ave;
    vector <double> aveb;
    for (int i=0; i<N; i++){
        double sum1=0;
        double sumb=0;
        for(int j=0; j<L; j++){
            double ap = rnd.Rannyu(); 
            sum1 += ap;
            sumb += pow(ap-0.5,2);
        }
        ave.push_back(sum1/L);  
        aveb.push_back(sumb/L);  
    }

    // evaluation of comulative average and error
    load_block_ave_it(ave, "data/1.1_a.dat");
    load_block_ave_it(aveb, "data/1.1_b.dat");

    //Chi test 
    int s = 100; // Number of sub-intervals
    int n = 10000; // Number of random numbers 
    int rip = 100; // Number of repetitions
    vector <double> chi;

    for(int i=0; i<rip; i++){
        vector <int> count(s,0);
        for(int j=0; j<n; j++){
            int k = int(rnd.Rannyu()*s);
            count[k]++;
        }
        double sum = 0;
        for(int j=0; j<s; j++){
            sum += pow(count[j]-n/s,2)/(n/s);
        }
        chi.push_back(sum);
    }

    // Save data in a file
    ofstream out;
    out.open("data/chi.dat");
    for(int i=0; i<rip; i++){
        out << i << " " << chi[i] << endl;
    }
    out.close();

    //Chi test n2 
    s = 100; // Number of sub-intervals
    n = 10000; // Number of random numbers 
    rip = 10000; // Number of repetitions
    vector <double> chi_2;

    for(int i=0; i<rip; i++){
        vector <int> count(s,0);
        for(int j=0; j<n; j++){
            int k = int(rnd.Rannyu()*s);
            count[k]++;
        }
        double sum = 0;
        for(int j=0; j<s; j++){
            sum += pow(count[j]-n/s,2)/(n/s);
        }
        chi_2.push_back(sum);
    }

    // Save data in a file
    out.open("data/chi2.dat");
    for(int i=0; i<rip; i++){
        out << i << " " << chi_2[i] << endl;
    }
    out.close();

    return 0;
}