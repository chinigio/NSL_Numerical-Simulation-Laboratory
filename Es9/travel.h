#ifndef __travel_h__
#define __travel_h__

# include <iostream>
# include <armadillo>
#include <cmath>
#include <fstream>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions

using namespace std;
using namespace arma;


class travel {
private:
    int n_cities; // number of cities
    vec o_c; // order of cities
    double fitness; // fitness value
public:
    void initialize(int n_c);
    void init_rnd(int n_c);

    int get_n_cities();
    int get_order(int i);
    void set_order(int i, int j);
    vec get_order();
    void set_fitness(double f);
    double get_fitness();

    void print();

    // genetic_mutation operators
    void permutation(int i, int j);
    void rnd_perm(Random &r);
    void shift(int n, int i, int j);
    void rnd_shift(Random &r);
    void multi_permutation(int n, int i, int j);
    void rnd_multi_perm(Random &r);
    void inversion(int i, int m);
    void rnd_inver(Random &r);
    void cross(travel& t, int i);
    void rnd_mut(Random &r);

    travel mutation(Random &r);
};

#endif