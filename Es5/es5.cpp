#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <armadillo>
#include "../PRNG/random.h" // for random generetor
#include "../function.h" //to use general functions
#include "position.h"
#include "q_state.h"

using namespace std;

int main(){

    // Random number generator
    Random rnd;

    position x_i(0.2, 0, 0); //initial position 
    position x_far(100, 0, 0); //far initial position

    // ground state, uniform transition
    q_state q_gs(true, true, x_i);
    q_gs.thermalization(rnd);
    q_gs.eval_radius(rnd, "data/gs.dat", true, "data/gs.xyz");

    // 2p state, uniform transition
    q_state q_2p(false, true, x_i);
    q_2p.thermalization(rnd);
    q_2p.eval_radius(rnd, "data/2p.dat", true, "data/2p.xyz");

    // ground state, uniform transition, far initial position
    q_state q_gs_far(true, true, x_far);
    q_gs_far.thermalization(rnd);
    q_gs_far.eval_radius(rnd, "data/gs_far.dat", false, "data/gs_far.xyz");

    // 2p state, uniform transition, far initial position
    q_state q_2p_far(false, true, x_far);
    q_2p_far.thermalization(rnd);
    q_2p_far.eval_radius(rnd, "data/2p_far.dat", false, "data/2p_far.xyz");

    // ground state, Gaussian transition
    q_state q_gs_gauss(true, false, x_i);
    q_gs.thermalization(rnd);
    q_gs.eval_radius(rnd, "data/gs_gaussian.dat", false, "data/gs.xyz");

    // 2p state, Gaussian transition
    q_state q_2p_gauss(false, false, x_i);
    q_2p.thermalization(rnd);
    q_2p.eval_radius(rnd, "data/2p_gaussian.dat", false, "data/2p.xyz");


    return 0;
}
