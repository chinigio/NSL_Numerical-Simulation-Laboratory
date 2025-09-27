#include <iostream> 
#include <cmath>
#include <fstream>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions
#include "position.h"
#include "q_state.h"

using namespace std; 

q_state::q_state(bool w, bool t, position x_0){
    w_type = w;
    trans_type = t;
    x_i = x_0;
}

q_state::q_state(bool w, bool t, int n_th, int n_bl, double l_s_, position x_0){
    w_type = t;
    n_t = n_th;
    n_b = n_bl;
    n_s = n_t/n_b;
    l_s = l_s_;
    x_i = x_0;
}

double q_state::prob_density(position p){
    if(w_type) //ground state
        return pow( exp(- p.distance())/pow(M_PI,0.5),2);
    else //2p state
        return pow(pow(2/M_PI, 0.5) * p.getZ() * exp(-p.distance()/2)/8, 2);
}

void q_state::thermalization(Random &rnd){
    position x_n = x_i; //new position
    position x_c = x_i; //current position
    int t = 150;
    double ratio = 0.2;

    while(ratio < 0.45 || ratio > 0.55){
        if(ratio < 0.5){
            l_s = l_s * rnd.Rannyu();
        }else{
            l_s = l_s * (rnd.Rannyu() + 1);
        }
        int acc = 0;
        int rej = 0;
        for(int j=0; j<t; j++){
            if(trans_type)
                x_n = x_c.new_pos(rnd, rnd.Rannyu()*l_s); //generation of new position with uniform transitrion probability
            else
                x_n = x_c.new_pos(rnd, rnd.Gauss(0, l_s)); //generation of new position with Gaussian transition probability
            double A = min(1, prob_density(x_n)/prob_density(x_c)); //acceptance ratio
            if(rnd.Rannyu() > A){
                rej++;
            }else{
                x_c = x_n; 
                acc++;
            } 
        }
        ratio = double(acc)/(acc+rej);
    }
}

void q_state::eval_radius(Random &rnd, string filename, bool print_xyz, string filename_xyz){
    position x_ch = x_i;
    position x_n = x_i;
    ofstream out;
    vector <double> r_gs;
    if(print_xyz)
        out.open(filename_xyz);
    for(int i=0; i<n_b; i++){
        double sum=0;
        for(int j=0; j<n_s; j++){
            if(trans_type)
                x_n = x_ch.new_pos(rnd, rnd.Rannyu()*l_s); //generation of new position with uniform transitrion probability
            else
                x_n = x_ch.new_pos(rnd, rnd.Gauss(0, l_s)); //generation of new position with Gaussian transition probability
            double A = min(1., prob_density(x_n)/prob_density(x_ch));
            double r = rnd.Rannyu();
            if(r > A){
                sum += x_ch.distance();
            }else{
                sum += x_n.distance();
                x_ch = x_n; 
            } 
            if(print_xyz && j % 10 == 0){
                out << x_ch.getX() << " " << x_ch.getY() << " " << x_ch.getZ() << endl;
            }
        }
        r_gs.push_back(sum/n_s);
    }
    if(print_xyz)
        out.close();
    // evaluation of comulative average and error
    load_block_ave_it(r_gs, filename);
}

