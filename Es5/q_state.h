#ifndef __q_state_h__
#define __q_state_h__

using namespace std;

class q_state {

private:
    bool w_type;
    bool trans_type;
    int n_t = 2*pow(10,6);
    int n_b = 2*100;
    int n_s = n_t/n_b;
    double l_s = 2.5;
    position x_i; //initial position

public: 

    q_state(bool w, bool t, position x_0);
    q_state(bool w, bool t, int n_t, int n_b, double l_s_, position x_0);
    void set_position(position x_0) { x_i = x_0; }

    double prob_density(position p);

    void thermalization(Random &rnd);

    void eval_radius(Random &rnd, string filename, bool print_xyz, string filename_xyz);
        
};

#endif // q_state_h