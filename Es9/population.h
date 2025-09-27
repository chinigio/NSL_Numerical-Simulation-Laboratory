#ifndef __population_h__
#define __population_h__

# include <iostream>
# include <armadillo>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions
#include "travel.h"


using namespace std;
using namespace arma;


class population{
private:
    Random r; // random number generator

    int n_cities; // number of cities
    int n_pop; // number of travel in population
    field <travel> pop; // population of travel objects
    double tot_fitness; // total fitness of population

    field <vec> pos_c; // position of cities
    mat dist; // distance matrix

    int n_per=0;
    int n_shift=0;
    int n_multi_perm=0;
    int n_inver=0;
    int n_cross=0;

public:
    //functions to initialize the popolation
    void load_c(field <vec> cities);
    void pop_init(int n_p);

    //function to manage the population
    void eval_fitness(int i);
    void eval_fitness(travel &t);
    travel get_travel(int i);
    void sort_pop(); // sort population by fitness
    void rm_trav(int i); // remove travel from population
    void add_sort(travel t); // insert a new travel in the population
    void sort_el(int i);
    void print_best(string filename);

    //mutation function
    void rnd_mut(int k);
    void general_mut();
    void cross(int i, int j); // cross two travels
    void rnd_cross();
    pair<travel, travel> birth(int i, int j);
    void evolution();

    void print_n_ev();

};

#endif