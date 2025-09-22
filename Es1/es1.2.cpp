#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../PRNG/random.h" // for random generetor

#include <vector>

using namespace std;

int main (){
    // Random number generator
    Random rnd;
    
    int r = 10000; // Number of realizations 
    vector <int> N = {1, 2, 10, 100}; // Number of data summed in each realization

    // Data vectors
    vector<vector<double>> uni(N.size(), vector<double>(r, 0));
    vector<vector<double>> exp(N.size(), vector<double>(r, 0));
    vector<vector<double>> cl(N.size(), vector<double>(r, 0));

    // generate data
    for(int i=0; i<r; i++){
        for(int j=0; j<4; j++){
            for(int k=0; k<N[j]; k++){
                uni[j][i] += rnd.Rannyu();
                exp[j][i] += rnd.Exp(1);
                cl[j][i] += rnd.CauchyLorentz(0,1);
            }
        }
    }

    // Write data to file
    vector<string> filenames = {"data/N1.dat", "data/N2.dat", "data/N10.dat", "data/N100.dat"};
    ofstream out;
    for(int j=0; j<4; j++){
        out.open(filenames[j]);
        for(int i=0; i<r; i++){
            out << i << " " << uni[j][i]/N[j] << " " << exp[j][i]/N[j] << " " << cl[j][i]/N[j] << endl;
        }
        out.close();  
    }

    return 0; 
}