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

    int d = 1; // distance between lines
    double L = 0.5; // needle lenght
    int T = 10000; // tables length
    int N_t = 1000000; // Number of throws
    int N_b = 100; // Number of blocks
    double y1=1; // it' no relevant the choice for y1 for the simmetry of the problem

    // evaluation of N_hits for every block
    vector <double> N_h; 
    for (int i=0; i < N_b; i++){
        int cont = 0;
        for (int j=0; j< N_t; j++){
            double x1 = rnd.Rannyu(0,T);
            double T = rnd.CauchyLorentz(0, y1);
            double x2 = x1 + (T*L)/sqrt(pow(y1,2) + pow(T,2));
            if(int(x1) != int(x2)){
                cont++;
            }               
        }
        N_h.push_back(2*L*static_cast<double>(N_t)/(cont*d));
    }

    // evaluation of comulative average and error
    load_block_ave_it(N_h, "data/pi.dat");

    return 0;
}