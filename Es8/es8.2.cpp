# include <iostream>
# include <vector>
#include <cmath>
#include <fstream>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions
#include "state.h"
#include "SA_state.h"

using namespace std;

int main(){
    Random rnd; // Random number generator

    SA_state model(0.5, 1.0);

    model.search_ground();

    cout << "H: " << model.get_H() << " mu: " << model.get_mu() << " sigma: " << model.get_sigma() <<endl; 

    model.print_H_storage("data/H_storage.dat");
    model.print_mu_storage("data/mu.dat");
    model.print_sigma_storage("data/sigma.dat");
    model.print_H_block("data/H_ground.dat");
    model.print_psi2_points("data/psi2_points.dat");

    state final_state = model.get_state();
    return 0;
}
