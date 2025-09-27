#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <armadillo>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions
#include "position.h"

using namespace std;

//ground state probability in natural unit
double ground_state(position p){
    return pow( exp(- p.distance())/pow(M_PI,0.5),2);
}

//2p state probability in natural unit
double state_2p(position p){
    return pow(pow(2/M_PI, 0.5) * p.getZ() * exp(-p.distance()/2)/8, 2);
}

double min(double a, double b){
    if(a>b){
        return b;
    }else
        return a;
}


int main(){

    // Random number generator
    Random rnd;

    //simulation parameters 
    int n_t = 1000000; //number of throws
    int n_b = 100; //number of blocks
    int n_s = n_t/n_b; //number of throws in each blocks
    double l_s = 2.5; //step lenght
    double accepted = 0;
    double rejected = 0;
    position x_i(0.2,0,0); //initial position

    //metropolis for ground state
    vector <double> r_gs;
    position x_ch; 
    x_ch = x_i;
    ofstream out;
    out.open("data/gs_xyz.xyz");
    //thermalization of the process
    for (int i=0; i<5000; i++) {
        position x_n = x_ch.new_pos(rnd, rnd.Rannyu()*l_s);
        double A = min(1.0, ground_state(x_n)/ground_state(x_ch));
        if(rnd.Rannyu() < A) x_ch = x_n;
    }
    for(int i=0; i<n_b; i++){
        double sum=0;
        for(int j=0; j<n_s; j++){
            position x_n = x_ch.new_pos(rnd, rnd.Rannyu()*l_s); //generation of new position 
            double A = min(1., ground_state(x_n)/ground_state(x_ch));
            double r = rnd.Rannyu();
            if(r > A){
                sum += x_ch.distance();
                rejected++;
            }else{
                sum += x_n.distance();
                x_ch = x_n; 
                accepted++;
            } 
            if(j % 10 == 0){
                out << x_ch.getX() << " " << x_ch.getY() << " " << x_ch.getZ() << endl;
            }
        }
        r_gs.push_back(sum/n_s);
    }
    out.close();
    // evaluation of comulative average and error
    load_block_ave_it(r_gs, "data/gs.dat");
    cout << "Acceptance rate ground-state distribution: " << accepted/(accepted+rejected) << endl;


    //metropolis for 2p state
    vector <double> r_2p;
    x_ch = x_i;
    out.open("data/2p_xyz.xyz");
    //thermalization of the process
    for (int i=0; i<5000; i++) {
        position x_n = x_ch.new_pos(rnd, rnd.Rannyu()*l_s);
        double A = min(1.0, state_2p(x_n)/state_2p(x_ch));
        if(rnd.Rannyu() < A) x_ch = x_n;
    }
    accepted = 0;
    rejected = 0;
    l_s = 5; //step lenght
    for(int i=0; i<n_b; i++){
        double sum=0;
        for(int j=0; j<n_s; j++){
            position x_n = x_ch.new_pos(rnd, rnd.Rannyu()*l_s); //generation of new position 
            double A = min(1., state_2p(x_n)/state_2p(x_ch));
            double r = rnd.Rannyu();
            if(r > A){
                sum += x_ch.distance();
                rejected++;
            }else{
                sum += x_n.distance();
                x_ch = x_n; 
                accepted++;
            } 
            if(j % 10 == 0){
                out << x_ch.getX() << " " << x_ch.getY() << " " << x_ch.getZ() << endl;
            }
        }
        r_2p.push_back(sum/n_s);
    }
    out.close();
    // evaluation of comulative average and error
    load_block_ave_it(r_2p, "data/2p.dat");
    cout << "Acceptance rate 2p-state distribution: " << accepted/(accepted+rejected) << endl;
    
    return 0;
}
