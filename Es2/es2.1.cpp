#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions

#include <vector>

using namespace std;

int main(){
    // Random number generator
    Random rnd;

    int N_b = 100; // number of blocks
    int n_t = 1000000; // number of throws in each block

    // vector for the calculation of the mean
    vector <double> ave;
    vector <double> aveb;

    // generation of data
    for (int i = 0; i < N_b; i++){
        double sum = 0;
        double sumb = 0;
        for (int j = 0; j < n_t; j++){
            sum += M_PI * 0.5 * cos(M_PI * rnd.Rannyu()*0.5);
            double x = 1 - sqrt(1 - rnd.Rannyu());
            sumb += M_PI * cos(M_PI * x *0.5)/(1-x) * 0.25;
        }
        ave.push_back(sum/n_t); 
        aveb.push_back(sumb/n_t);
    }

    // evaluation of comulative average and error
    load_block_ave_it(ave, "data/2.1a.dat");
    load_block_ave_it(aveb, "data/2.1b.dat");
    return 0;
}