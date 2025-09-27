#include "travel.h"

using namespace std;

// Initialize function
void travel::initialize(int n) {
    n_cities = n;
    o_c = linspace<vec>(0, n-1, n);
    vec rest = o_c.subvec(1, o_c.n_elem - 1);
    o_c.subvec(1, o_c.n_elem - 1) = rest;
}

void travel::init_rnd(int n) {
    n_cities = n;
    o_c = linspace<vec>(0, n-1, n);
    vec rest = o_c.subvec(1, o_c.n_elem - 1);
    rest = arma::shuffle(rest);
    o_c.subvec(1, o_c.n_elem - 1) = rest;
}

// Parameter and order functions
int travel::get_n_cities() {
    return n_cities;
}

int travel::get_order(int i) {
    if (i < 0 || i >= n_cities) {
        cout << "Error: index out of bounds." << endl;
        return -1;
    }
    return o_c(i);
}

void travel::set_order(int i, int j) {
    if (i < 0 || i >= n_cities) {
        cout << "Error: index out of bounds." << endl;
        return;
    }
    o_c(i) = j;
}

vec travel::get_order() {
    return o_c;
}

void travel::set_fitness(double f) {
    fitness = f;
}

double travel::get_fitness() {
    return fitness;
}


void travel::print() {
    cout << "Order of cities: " << o_c.t() << endl;
}

// genetic_mutation operators
void travel::permutation(int i, int j) {
    if (i < 1 || i >= n_cities || j < 1 || j >= n_cities) {
        return;
    }
    o_c.swap_rows(i, j);
}

void travel::rnd_perm(Random &r) {
    int i = r.uniform_int(1, n_cities-1);
    int j = r.uniform_int(1, n_cities-1);
    permutation(i, j);
}

void travel::shift(int n, int i, int m) {
    if (i < 1 || i > n_cities-2 || m < 1 || m > (n_cities-i)/2 || n < 1 || n > m) {
        cout << "Error: invalid parameters for shift." << endl;
        cout << "i: " << i << ", m: " << m << ", n: " << n << endl;
        return;
    }
    vec tail = arma::shift(o_c.subvec(i, i+m+n-1), n);
    o_c.subvec(i, i+m+n-1) = tail;
}

void travel::rnd_shift(Random &r) {
    int i = r.uniform_int(1, n_cities-2);
    int m = r.uniform_int(1, (n_cities-i)/2);
    int n = r.uniform_int(1, m);
    shift(n, i, m);
}

void travel::multi_permutation(int n, int i, int j) {
    if (i < 1 || i > n_cities - n || j < i+n || j > n_cities-n+1 || n < 1 || n > (n_cities-1)/2) {
        cout << "Error: invalid parameters for multi_permutation." << endl;
        cout << "i: " << i << ", j: " << j << ", n: " << n << endl;
        return;
    }
    vec temp = o_c.subvec(i, i+n-1);
    o_c.subvec(i, i+n-1) = o_c.subvec(j, j+n-1);
    o_c.subvec(j, j+n-1) = temp;
}

void travel::rnd_multi_perm(Random &r) {
    int n_2 = (n_cities-1)/2;
    int n = r.uniform_int(1, n_2);
    int i = r.uniform_int(1, n_cities-2*n-1);
    int j = r.uniform_int(i+n, n_cities-n);
    multi_permutation(n, i, j);
}

void travel::inversion(int i, int m) {
    if (i < 1 || i >= n_cities || m < 1 || m > n_cities-i) {
        cout << "Error: invalid parameters for inversion." << endl;
        return;
    }
    vec temp = o_c.subvec(i, i+m-1);
    o_c.subvec(i, i+m-1) = flipud(temp);
}

void travel::rnd_inver(Random &r) {
    int i = r.uniform_int(1, n_cities-1);
    int m = r.uniform_int(1, n_cities-i);
    inversion(i, m);
}

void travel::cross(travel& t, int i) {
    if (i < 1 || i >= n_cities) {
        cout << "Error: index out of bounds." << endl;
        return;
    }
    vec temp = o_c.subvec(i, n_cities-1);
    for(int j = i; j < n_cities; j++) {
        o_c(j) = t.get_order(j);
        o_c(j) = t.get_order(j);
    }

    o_c.subvec(i, n_cities-1) = t.get_order().subvec(i, n_cities-1);
    t.o_c.subvec(i, n_cities-1) = temp;
}

void travel::rnd_mut(Random &r) {
    double prop = 0.2;
    double p = r.Rannyu();

    if(p<prop){
        rnd_perm(r);
    }
    else if(p<2*prop){
        rnd_shift(r);
    }
    else if(p<3*prop){
        rnd_multi_perm(r);
    }
    else if(p<4*prop){
        rnd_inver(r);
    }
}

// genetic mutation creating a new travel object
travel travel::mutation(Random &r) {
    travel copy = *this;
    copy.rnd_mut(r);
    return copy;
}

