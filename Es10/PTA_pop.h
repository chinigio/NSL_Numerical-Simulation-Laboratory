#ifndef __PTA_pop_h__
#define __PTA_pop_h__

# include <iostream>
# include <armadillo>
#include <cmath>
#include <fstream>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions
#include "travel.h" // to use travel class
#include "mpi.h" // for MPI

using namespace std;
using namespace arma;

class PTA_pop {
private:
    Random r; // random number generator

    int n_cities=0; // number of cities

    int rank, size; // MPI rank and size

    vec beta; // vector of beta values for the problem

    field <vec> pos_c; // position of cities
    mat dist; // distance matrix

    travel elem; // travel objects
public:

    //inizializing functions
    void load_c(field <vec> cities);
    void init_elem();
    void initialize();

    // variable functions
    void eval_fitness();
    void eval_fitness(travel &t);
    int get_rank();
    int get_size();
    travel get_elem();

    // mutation functions
    void metro_mut();
    void change_val();

    // printing functions
    void save_best(string filename);
};

#endif