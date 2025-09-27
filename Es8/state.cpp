#include <iostream>
#include <cmath> 
#include <armadillo>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions
#include "state.h"

using namespace std;
using namespace arma;

//costructor
state::state(double a, double b){
    mu = a;
    sigma = b;
}

//energy functions
double state::psi_2(double x){ // Wave function squared
    double norm = 2 * sigma * sqrt(M_PI) * (1 + exp(-(mu*mu) / (sigma*sigma)));
    double psi = exp(-(x-mu)*(x-mu)/(2*sigma*sigma)) + exp(-(x+mu)*(x+mu)/(2*sigma*sigma));
    return norm*psi*psi;
}

double state::V(double x){ // Potential function
    double V = pow(x, 4) - 5 * pow(x, 2)/2;
    return V;
}

double state::E_loc(double x){
    double sigma2 = sigma*sigma;
    double pref = 1.0/(2.0*sigma2);
    double xm = x-mu, xp = x+mu;
    double log_g1 = -xm*xm/(2*sigma2);
    double log_g2 = -xp*xp/(2*sigma2);
    // log-sum-exp
    double M = max(log_g1, log_g2);
    double log_sum = M + log(exp(log_g1-M) + exp(log_g2-M));

    double g1 = exp(log_g1), g2 = exp(log_g2);
    double denom = g1+g2;
    if (denom < 1e-300) return V(x);
    double num = (1 - xm*xm/sigma2)*g1 + (1 - xp*xp/sigma2)*g2;
    double kinetic = pref * num/denom;
    return kinetic + V(x);
}

vector <double> state::H_eval(Random &rnd){
    double x_n=0; //new position
    double x_c=x0; //current position
    accepted = 0;
    rejected = 0;

    vector <double> H; //vector to store the local energy
    for(int i=0; i<n_b; i++){
        double sum=0;
        int temp=0;
        for(int j=0; j<n_s; j++){
            x_n = x_c + l_s * rnd.Rannyu(-1,1); //new position
            double A = min(1, psi_2(x_n)/psi_2(x_c)); //acceptance ratio
            if(rnd.Rannyu() > A){
                rejected++;
            }else{
                sum += E_loc(x_n); //function to be integrated
                x_c = x_n; 
                temp++;
                accepted++;
            } 
        }
        H.push_back(sum/temp); //store the average of the local energy
    }
    return H;
}

pair <double, double> state::H_value(Random &r){
    vector <double> H_vec = H_eval(r);
    pair<double, double> H = block_ave(H_vec);
    return H;
}

void state::sample_psi2(Random &rnd, string filename){

    double x_n=0; //new position
    double x_c=x0; //current position
    int n_p = pow(10,6); //number of tries for metropolis

    ofstream out;
    out.open(filename);

    int temp=0;
    for(int j=0; j<n_p; j++){
        x_n = x_c + l_s * rnd.Rannyu(-1,1); //new position
        double A = min(1, psi_2(x_n)/psi_2(x_c)); //acceptance ratio
        if(rnd.Rannyu() > A){

        }else{
            x_c = x_n; 
            temp++;
            out << temp << " " << x_n <<endl;
        } 
    }
    out.close();
}

//function to determine the step
void state::thermalization(Random &rnd){
    double x_n=0; //new position
    double x_c=x0; //current position
    int t = 100;
    double ratio = 0.2;

    while(ratio < 0.45 || ratio > 0.60){
        if(ratio < 0.5){
            l_s = l_s * rnd.Rannyu();
        }else{
            l_s = l_s * (rnd.Rannyu() + 1);
        }
        int acc = 0;
        int rej = 0;
        for(int j=0; j<t; j++){
            x_n = x_c + l_s * rnd.Rannyu(-1,1); //new position
            double A = min(1, psi_2(x_n)/psi_2(x_c)); //acceptance ratio
            if(rnd.Rannyu() > A){
                rej++;
            }else{
                x_c = x_n; 
                acc++;
            } 
        }
        ratio = double(acc)/(acc+rej);
    }
}

//function to get acceptance ratio
double state::accep_ratio(){
    if(accepted == 0){
        return 0;
    }

    return double(accepted)/(accepted+rejected);
}