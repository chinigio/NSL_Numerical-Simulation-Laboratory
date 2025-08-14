#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions

#include <vector>

using namespace std;

double max(double a, double b){
    if(a > b){
        return a;
    } else return b;
}

double S_it(double t, int n_t, double S_i, Random &rnd, double mu, double sigma){
    double step = t/n_t;
    double S=0;
    double temp=S_i;
    for(int i=0; i<n_t; i++){
        double z = rnd.Gauss(0,1);
        S = temp * exp((mu - 0.5 * pow(sigma,2))* step + sigma * z * sqrt(step));
        temp = S; 
    }
    return S;
}

double S_fin(double t, double S0, Random &rnd, double r, double sigma){
    double W = rnd.Gauss(0,t);
    double S = S0 * exp((r - 0.5 * pow(sigma,2))*t + sigma * W);
    return S;
}

double C(double r, double T, double S, double K){
    double C = exp(-r * T) * max(0, S-K);
    return C;
}

double P(double r, double T, double S, double K){
    double P = exp(-r * T) * max(0, K-S);
    return P;
}

int main(){
    // Random number generator
    Random rnd;
    int seed[4]= {0, 0, 0, 1}; // seed for the random number generator
    rnd.SetRandom(seed, 2892, 2707);

    // Simulation parameters
    int n_b = 100; // number of blocks
    int n_t = 100000; // number of throws 
    int N = n_t/n_b; // number of throws for each block 

    // Finance parameters
    double S0 = 100; // asset price at t=0
    double T = 1; // delivery time
    double K = 100; // strike price
    double r = 0.1; // risk-free interest rate
    double sig = 0.25; // volatility
    double n_i = 100;

    //evaluating the value for each block
    vector <double> block_c;
    vector <double> block_p;
    for(int i=0; i<n_b; i++){
        double sum_c=0;
        double sum_p=0;
        for(int j=0; j<N; j++){
            double S = S_fin(T, S0, rnd, r, sig);
            sum_c += C(r, T, S, K);
            sum_p += P(r, T, S, K);
        }
        block_c.push_back(sum_c/N);
        block_p.push_back(sum_p/N);
    }

    vector <double> c_it;
    vector <double> p_it;
    for(int i=0; i<n_b; i++){
        double sum_c=0;
        double sum_p=0;
        for(int j=0; j<N; j++){
            double S = S_it(T, n_i, S0, rnd, r, sig);
            sum_c += C(r, T, S, K);
            sum_p += P(r, T, S, K);
        }
        c_it.push_back(sum_c/N);
        p_it.push_back(sum_p/N);
    }

    // evaluation of comulative average and error
    load_block_ave_it(block_c, "data/es3_c.dat");
    load_block_ave_it(block_p, "data/es3_p.dat");
    load_block_ave_it(c_it, "data/es3_c_it.dat");
    load_block_ave_it(p_it, "data/es3_p_it.dat");

    return 0;
}