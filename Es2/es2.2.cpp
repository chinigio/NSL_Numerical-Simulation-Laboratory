#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>  // for transform() function
#include <armadillo> // for arma::vec and arma::norm
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions
#include "position.h" // for Posizione class

#include <vector>

using namespace std;

int main(){
    // Random number generator
    Random rnd;

    int n_b = 100; // number of blocks
    int n_rw = 10000; // number of random walks
    int n_s = 100; // number of steps in each random walk
    int N = n_rw/n_b; // number of step in each block

    double l_s = 1.0; // step length

    // generation of data
    vector<vector<double>> blocks_d(n_s, vector<double>(n_b, 0));
    vector<vector<double>> blocks_c(n_s, vector<double>(n_b, 0));
    position p_d, p_c;

    for(int i=0; i<n_b; i++){
        for(int j=0; j<N; j++){
            p_d = position(0, 0, 0); // reset position for each block
            p_c = position(0, 0, 0); // reset position for each block
            for(int k=0; k<n_s; k++){
                p_d.next_s_discrete(rnd, l_s); // take a step in discrete space
                blocks_d[k][i] += pow(p_d.distance(), 2); // accumulate squared distance
                p_c.next_s(rnd, l_s); // take a step in continuous space
                blocks_c[k][i] += pow(p_c.distance(), 2); // accumulate squared distance
            }
        }
    }

    for(int i=0; i<n_s; i++){
        for(int j=0; j<n_b; j++){
            blocks_d[i][j] = sqrt(blocks_d[i][j]/(double)N);
        }
    }

    for(int i=0; i<n_s; i++){
        for(int j=0; j<n_b; j++){
            blocks_c[i][j] = sqrt(blocks_c[i][j]/(double)N);
        }
    }

    // evaluation of comulative average and error
    vector<double> sum_prog_d(n_s, 0);
    vector<double> err_prog_d(n_s, 0);
    vector<double> sum_prog_c(n_s, 0);
    vector<double> err_prog_c(n_s, 0);
    for(int i=0; i<n_s; i++){
        vector<double> sqrt_vec(n_b, 0);
        sum_prog_d[i] = block_ave(blocks_d[i]).first;
        err_prog_d[i] = block_ave(blocks_d[i]).second;
        sum_prog_c[i] = block_ave(blocks_c[i]).first;
        err_prog_c[i] = block_ave(blocks_c[i]).second;
    }

    // Writing results on file
    ofstream out;
    out.open("data/es2.2_d.dat");
    for(int i=0; i<n_s; i++){
        out << i+1 << " " << sum_prog_d[i] << " " << err_prog_d[i] << endl;
    }
    out.close();

    out.open("data/es2.2_c.dat");
    for(int i=0; i<n_s; i++){
        out << i+1 << " " << sum_prog_c[i] << " " << err_prog_c[i] << endl;
    }
    out.close();

    return 0;
}
