#include "population.h"

using namespace std;

//functions to initialize the popolation
void population::load_c(field <vec> cities){
    n_cities = cities.n_elem;
    pos_c = cities;
    dist.set_size(n_cities, n_cities);
    for (int i = 0; i < n_cities; i++){
        for (int j = 0; j < n_cities; j++){
            dist(i, j) = norm(pos_c(i) - pos_c(j));
        }
    }
}

void population::pop_init(int n_p){
    n_pop = n_p;
    pop.set_size(n_pop);
    for (int i = 0; i < n_pop; i++){
        pop(i).init_rnd(n_cities);
        eval_fitness(i);
        tot_fitness += pop(i).get_fitness();
    }
    sort_pop();
}

//function to manage the population
travel population::get_travel(int i){
    return pop(i);
}

void population::eval_fitness(int i){
    double f = 0;
    for (int j = 0; j < n_cities-1; j++){
        f += dist(pop(i).get_order(j), pop(i).get_order(j+1));
    }
    f += dist(pop(i).get_order(n_cities-1), pop(i).get_order(0));
    pop(i).set_fitness(f);
}

void population::eval_fitness(travel &t){
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

void population::sort_pop(){
    for (int i = 0; i < n_pop-1; i++){
        for (int j = i+1; j < n_pop; j++){
            if (pop(i).get_fitness() > pop(j).get_fitness()){
                travel temp = pop(i);
                pop(i) = pop(j);
                pop(j) = temp;
            }
        }
    }
}

void population::rm_trav(int i){
    tot_fitness -= pop(i).get_fitness();
    field<travel> temp(pop.n_elem - 1);
    for (int j = 0; j < i; ++j){
        temp(j) = pop(j);
    }
    for (int j = i+1; j < pop.n_elem; ++j){
        temp(j-1) = pop(j);
    }
    pop = temp;
}

void population::add_sort(travel t){
    tot_fitness += t.get_fitness();

    // find the position to insert the new travel
    int left = 0;
    int right = pop.n_elem;
    while (left < right) {
        int mid = (left + right) / 2;
        if (pop(mid).get_fitness() < t.get_fitness())
            left = mid + 1;
        else
            right = mid;
    }
    // build a new field with the new travel inserted
    field<travel> temp(pop.n_elem + 1);
    for (int i = 0; i < left; ++i){
        temp(i) = pop(i);
    }
    temp(left) = t;
    for (int i = left; i < pop.n_elem; ++i){
        temp(i + 1) = pop(i);
    }
    // update the population
    pop = temp;

}

void population::sort_el(int i){
    travel t = pop(i); 
    rm_trav(i);

    // find the position to insert the new travel
    int left = 0;
    int right = pop.n_elem;
    while (left < right) {
        int mid = (left + right) / 2;
        if (pop(mid).get_fitness() < t.get_fitness())
            left = mid + 1;
        else
            right = mid;
    }

    for (int i = left; i < pop.n_elem; ++i){
        travel temp = pop(i);
        pop(i) = t;
        t = temp;
    }
}

void population::print_best(string filename){
    ofstream out;
    out.open(filename);
    for(int i = 0; i < n_cities; i++){
        int idx = pop(0).get_order(i); 
        out << pos_c(idx)(0) << " " << pos_c(idx)(1) << endl; 
    }
    out.close();
}

//mutation function
void population::rnd_mut(int i) {
    double prop = 0.1;
    double p = r.Rannyu();

    if(p<prop){
        pop(i).rnd_perm(r);
        eval_fitness(i);
        n_per++;
    }
    else if(p<2*prop){
        pop(i).rnd_shift(r);
        eval_fitness(i);
        n_shift++;
    }
    else if(p<3*prop){
        pop(i).rnd_multi_perm(r);
        eval_fitness(i);
        n_multi_perm++;
    }
    else if(p<4*prop){
        pop(i).rnd_inver(r);
        eval_fitness(i);
        n_inver++;
    }
}

void population::general_mut(){
    for(int i=0; i<n_pop; i++){
        rnd_mut(i);
    }
}

void population::cross(int i, int j) {
    int N = n_cities;
    int cut = r.Rannyu(1, N-1);  // cut point for crossover

    vec parent1 = pop(i).get_order();
    vec parent2 = pop(j).get_order();
    
    vec child1 = parent1;
    vec child2 = parent2;

    // Fill the second part of the children with the second part of the other parents
    int pos1 = cut;
    for (int k = 1; k < N; k++) {
        int city = parent2(k);
        bool already_in_child = false;
        for (int m = 1; m < cut; m++) {
            if (child1(m) == city) {
                already_in_child = true;
                break;
            }
        }
        if (!already_in_child) {
            child1(pos1) = city;
            pos1++;
        }
    }

    int pos2 = cut;
    for (int k = 0; k < N; k++) {
        int city = parent1(k);
        bool already_in_child = false;
        for (int m = 0; m < cut; m++) {
            if (child2(m) == city) {
                already_in_child = true;
                break;
            }
        }
        if (!already_in_child) {
            child2(pos2) = city;
            pos2++;
        }
    }

    // Remove the old individuals from the population and add the new ones
    for (int k = 0; k < N; ++k) {
        pop(i).set_order(k, child1(k));
        pop(j).set_order(k, child2(k));
    }
    eval_fitness(pop(i));
    eval_fitness(pop(j));
}

void population::rnd_cross(){
    double prop = 0.5;
    double p = r.Rannyu();
    sort_pop(); // sort the population by fitness
    int i = int(n_pop * pow(r.Rannyu(), 0.4));
    int j = int(n_pop * pow(r.Rannyu(), 0.4));

    if(p < prop){
        cross(i, j);
        n_cross++;
    }
}

pair<travel, travel> population::birth(int i, int j){
    int N = n_cities;
    int cut = r.Rannyu(1, N-1);  // cut point for crossover

    vec child1 = pop(i).get_order();
    vec child2 = pop(j).get_order();

    // Fill the second part of the children with the second part of the other parents
    int pos1 = cut;
    for (int k = 1; k < N; k++) {
        int city = pop(j).get_order(k);
        bool already_in_child = false;
        for (int m = 1; m < cut; m++) {
            if (child1(m) == city) {
                already_in_child = true;
                break;
            }
        }
        if (!already_in_child) {
            child1(pos1) = city;
            pos1++;
        }
    }

    int pos2 = cut;
    for (int k = 0; k < N; k++) {
        int city = pop(i).get_order(k);
        bool already_in_child = false;
        for (int m = 0; m < cut; m++) {
            if (child2(m) == city) {
                already_in_child = true;
                break;
            }
        }
        if (!already_in_child) {
            child2(pos2) = city;
            pos2++;
        }
    }

        travel c1;
    c1.initialize(N);
    travel c2;
    c2.initialize(N);

    // Remove the old individuals from the population and add the new ones
    for (int k = 0; k < N; ++k) {
        c1.set_order(k, child1(k));
        c2.set_order(k, child2(k));
    }
    eval_fitness(c1);
    eval_fitness(c2);
    return make_pair(c1, c2);
}

void population::evolution(){
    double index_prob = 2;
    double prob = 0.5;

    // building the new popolation
    field<travel> child;
    child.set_size(n_pop);

    int k=0;
    while(k < n_pop){
        int i = int(n_pop * pow(r.Rannyu(), index_prob));
        int j = int(n_pop * pow(r.Rannyu(), index_prob));
        if(r.Rannyu()<prob){
            pair<travel, travel> children = birth(i, j);
            child(k) = children.first;
            k++;
            child(k) = children.second;
            k++;
        }
    }

    pop = child;
    general_mut();
    sort_pop();

}


void population::print_n_ev(){
    cout << "Number of individuals in the population: " << n_pop << endl;
    cout << "Number of mutations: " << n_per << endl;
    cout << "Number of shifts: " << n_shift << endl;
    cout << "Number of multi permutations: " << n_multi_perm << endl;
    cout << "Number of inversions: " << n_inver << endl;
    cout << "Number of crossovers: " << n_cross << endl;
}