#ifndef __SA_state_h__
#define __SA_state_h__

# include <iostream>
# include <armadillo>
# include <cmath>
# include <fstream>
# include "../PRNG/random.h" // for random generetor
# include "../function.h" //to use general functions
# include "state.h"

using namespace std;

class SA_state {
private:
    Random rnd; //random generator

    int n_t = 100; //number of temperature value
    int n_c = 15; //number of throws for each temperature
    double beta_i = 0.1; //initial beta

    state current_state;
    double H_val;
    double mu_c;
    double sigma_c;

    vector<double> H_storage;
    vector<double> mu_storage;
    vector<double> sigma_storage;

public:
    //constructor
    SA_state(double mu, double sigma);

    //parameter function
    double get_H();
    double get_mu();
    double get_sigma();
    state get_state();
    vector<double> get_H_storage();
    vector<double> get_mu_storage();
    vector<double> get_sigma_storage();

    void print_H_storage(string filename);
    void print_mu_storage(string filename);
    void print_sigma_storage(string filename);
    void print_H_block(string filename);
    void print_psi2_points(string filename);

    void search_ground();
};

#endif // SA_state_h