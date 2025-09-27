#ifndef __state_h__
#define __state_h__

using namespace std;

class state {
private:

    double mu;
    double sigma;
    double x0 = 0; //initial position
    double hbar = 1; //hbar
    double mass = 1; //mass of the particle

    double n_b = 100; //number of blocks 
    double n_s = 10000; //number of step for block
    double l_s = 1;

    int accepted = 0;
    int rejected = 0;
public:
    state(double mu, double sigma);
    double get_mu(){ return mu; }
    double get_sigma(){ return sigma; }

    double psi_2(double x);
    double V(double x);
    double E_loc(double x);
    vector <double> H_eval(Random &rnd);
    pair <double, double> H_value(Random &rnd);
    void sample_psi2(Random &rnd, string filename);

    void thermalization(Random &rnd);

    double accep_ratio();
};

#endif // position_h