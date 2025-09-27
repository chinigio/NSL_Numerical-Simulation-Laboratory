#include "PTA_pop.h"

using namespace std;
using namespace arma;

//inizializing functions
void PTA_pop::load_c(field <vec> cities){
    n_cities = cities.n_elem;
    pos_c = cities;
    dist.set_size(n_cities, n_cities);
    for (int i = 0; i < n_cities; i++){
        for (int j = 0; j < n_cities; j++){
            dist(i, j) = norm(pos_c(i) - pos_c(j));
        }
    }
}

void PTA_pop::init_elem(){
    elem.init_rnd(n_cities);
    eval_fitness();
}

void PTA_pop::initialize(){
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Initialize the travel object
    init_elem();

    // Initialize beta values
    beta.set_size(size);
    for (int i = 0; i < size; i++){
        beta(i) = pow(i + 1,2);
    }
}

// variable functions
void PTA_pop::eval_fitness(){
    double f = 0;
    for (int j = 0; j < n_cities-1; j++){
        f += dist(elem.get_order(j), elem.get_order(j+1));
    }
    f += dist(elem.get_order(n_cities-1), elem.get_order(0));
    elem.set_fitness(f);
}

void PTA_pop::eval_fitness(travel &t){
    if(t.get_n_cities() != n_cities){
        cout << "Error: travel object has different number of cities." << endl;
        return;
    }
    double f = 0;
    for (int j = 0; j < n_cities-1; j++){
        f += dist(t.get_order(j), t.get_order(j+1));
    }
    f += dist(t.get_order(n_cities-1), t.get_order(0));
    t.set_fitness(f);
}

int PTA_pop::get_rank(){
    return rank;
}

int PTA_pop::get_size(){
    return size;
}

travel PTA_pop::get_elem(){
    return elem;
}

// mutation functions
void PTA_pop::metro_mut(){
    travel t = elem.mutation(r);
    eval_fitness(t);
    double delta_f = t.get_fitness() - elem.get_fitness();
    if(delta_f < 0){
        elem = t;
    } else {
        double p = exp(-beta(rank) * delta_f);
        double x = r.Rannyu();
        if(x < p){
            elem = t;
        }
    }
}

void PTA_pop::change_val(){

    if(rank != size - 1){
        // Send the fitness value and the order of the cities to the next rank
        double f_send = elem.get_fitness();
        MPI_Send(&f_send, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(elem.get_order().memptr(), n_cities, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
    }

    if(rank != 0){
        // Receive the fitness value and the order of the cities from the previous rank
        double f_recv;
        MPI_Recv(&f_recv, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        vec order_recv(n_cities);
        MPI_Recv(order_recv.memptr(), n_cities, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Create a new travel object with the received order and fitness
        travel t_recv;
        t_recv.initialize(n_cities);
        for(int i = 0; i < n_cities; i++){
            t_recv.set_order(i, order_recv(i));
        }
        t_recv.set_fitness(f_recv);

        // Consider the received travel object and decide whether to replace the current one based on the Metropolis criterion
        double delta = (beta(rank) - beta(rank - 1)) * (t_recv.get_fitness() - elem.get_fitness());
        if(delta < 0){
            elem = t_recv;
        } else {
            double p = exp(-delta);
            if(r.Rannyu() < p){
                elem = t_recv;
            } 
        }
    }
}

// printing functions
void PTA_pop::save_best(string filename){
    if(rank == size - 1){
        ofstream out(filename, ios::app);
        if(!out.is_open()){
            cout << "Error: could not open file " << filename << endl;
            return;
        }
        out << elem.get_fitness() << endl;
        out.close();
    }
}

// end