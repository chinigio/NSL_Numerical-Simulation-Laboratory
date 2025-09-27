# include <iostream>
# include <vector>
#include <cmath>
#include <fstream>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions
#include "state.h"

using namespace std;

int main() {
    
    Random rnd; // Random number generator

    //ground state initialization
    state ground(0.0, 1.0); 
    ground.thermalization(rnd);

    // evaluation of the energy
    vector <double> H_ground = ground.H_eval(rnd); 
    load_block_ave_it(H_ground, "data/H_block.dat"); // evaluation of comulative average and error

    cout <<"Acceptance ratio: " << ground.accep_ratio() << endl;
    return 0;
}