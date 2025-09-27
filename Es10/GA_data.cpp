# include <iostream>
# include <vector>
#include <cmath>
#include <fstream>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions

#include <armadillo>
#include "travel.h"
#include "population.h"

using namespace std;
using namespace arma;

field<vec> load_cities(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    vector<vec> temp;
    double x, y;
    while (in >> x >> y) {
        vec city(2);
        city(0) = x;
        city(1) = y;
        temp.push_back(city);
    }
    in.close();

    field<vec> cities(temp.size());
    for (size_t i = 0; i < temp.size(); ++i) {
        cities(i) = temp[i];
    }

    return cities;
}

int main(){
    // general parameters and variables
    Random rnd; // random number generator
    int n_ev = pow(10, 6); // number of evolution steps
    int n_pop = 8; // number of individuals in the population 
    int n_cities = 34;

    // create a population of travel objects
    population GA_pop;
    field <vec> cities = load_cities("imput/cap_prov_ita.dat"); 
    GA_pop.load_c(cities);
    GA_pop.pop_init(n_pop);

    // let the population evolve
    ofstream out;
    out.open("data/GA.dat");
    for(int i = 0; i < n_ev; i++){
        GA_pop.evolution();
        if(i % 1000 == 0)
            out << i << " " << GA_pop.get_travel(0).get_fitness() << endl; 
    }
    out.close();

    // print the best travel
    GA_pop.print_best("data/GA_coord.dat");

    return 0;
}