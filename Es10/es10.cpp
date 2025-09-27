# include <iostream>
# include <armadillo>
#include <cmath>
#include <fstream>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions
#include "mpi.h" // for MPI
#include "travel.h" // to use travel class
#include "PTA_pop.h" // to use PTA_pop class

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


int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);  

    Random r;
    field <vec> cities = load_cities("imput/cap_prov_ita.dat"); // loading the cities

    // parameter for the problem
    int n_mut = pow(10, 6);  

    // Initialize the population
    PTA_pop pop;
    pop.load_c(cities);
    pop.initialize();

    // let the population evolve
    for(int i = 0; i < n_mut; i++){
        pop.metro_mut();
        if(i % 1000 == 0) {
            pop.save_best("data/best_travel.txt");
            pop.change_val();
        }
    }

    // print the best travel
    if(pop.get_rank() == pop.get_size() - 1){
        ofstream out;
        out.open("data/10_coord.dat");
        for(int i = 0; i < cities.n_elem; i++){
            out << cities(pop.get_elem().get_order(i))(0) << " " << cities(pop.get_elem().get_order(i))(1) << endl;
        }
        out.close();
    }

    MPI_Finalize();

    return 0;
}
