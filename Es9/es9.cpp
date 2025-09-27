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

field <vec> rnd_circle(int n_cities, double radius, Random &r) {
    field <vec> cities;
    cities.set_size(n_cities);
    cities(0) = {radius, 0.0}; // first city at the origin
    for (int i = 1; i < n_cities; i++){
        double theta = 2 * M_PI * r.Rannyu();
        double x = radius * cos(theta);
        double y = radius * sin(theta);
        vec city = {x, y};
        cities(i) = city;
    }
    return cities;
}

field <vec> rnd_square(int n_cities, double L, Random &r) {
    field <vec> cities;
    cities.set_size(n_cities);
    for (int i = 0; i < n_cities; i++){
        double x = L * r.Rannyu();
        double y = L * r.Rannyu();
        vec city = {x, y};
        cities(i) = city;
    }
    return cities;
}

int main(){
    // general parameters and variables
    Random r; // random number generator
    int n_ev = 500; // number of evolution steps
    int n_pop = 200; // number of individuals in the population 
    int n_cities = 34;

    // generation of random cities along a circle
    field <vec> cities_circle;
    cities_circle = rnd_circle(n_cities, 1.0, r); // generate cities in a circle with radius 1.0

    // create a population of travel objects
    population pop_circle;
    pop_circle.load_c(cities_circle);
    pop_circle.pop_init(n_pop); 

    // evaluation of the best travel and the average of the firtst 50 individuals
    ofstream out;
    ofstream out2;
    out.open("data/circle_dist.dat");
    out2.open("data/circle_d_ave.dat");
    for(int i = 0; i < n_ev; i++){
        pop_circle.evolution();
        out << i << " " << pop_circle.get_travel(0).get_fitness() << endl; 
        double ave = 0.0;
        for(int j = 0; j < 50; j++){
            ave += pop_circle.get_travel(j).get_fitness();
        }
        out2 << i << " " << ave/50.0 << endl;
    }
    out.close();
    out2.close();  

    // print the best travel
    pop_circle.print_best("data/circle_coord.dat");


    //repeat the same process for the cities distribuited randomly in a square--------------------------------------------------

    // generation of random cities along a circle
    field <vec> cities_square;
    cities_square = rnd_square(n_cities, 1.0, r); // generate cities in a circle with radius 1.0

    // create a population of travel objects
    population pop_square;
    pop_square.load_c(cities_square);
    pop_square.pop_init(n_pop); // initialize population with 10 individuals

    // evaluation of the best travel and the average of the firtst 50 individuals
    out.open("data/square_dist.dat");
    out2.open("data/square_d_ave.dat");
    for(int i = 0; i < n_ev; i++){
        pop_square.evolution();
        out << i << " " << pop_square.get_travel(0).get_fitness() << endl; 
        double ave = 0.0;
        for(int j = 0; j < 50; j++){
            ave += pop_square.get_travel(j).get_fitness();
        }
        out2 << i << " " << ave/50.0 << endl;
    }
    out.close();
    out2.close();  

    // print the best travel
    pop_square.print_best("data/square_coord.dat");

    return 0;
}

