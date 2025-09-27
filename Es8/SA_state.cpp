#include "SA_state.h" 

using namespace std; 

//constructor
SA_state::SA_state(double mu, double sigma)
    : current_state(mu, sigma), mu_c(mu), sigma_c(sigma)
{
    H_val = current_state.H_value(rnd).first;
}

//parameter function
double SA_state::get_H(){
    return H_val;
}

double SA_state::get_mu(){
    return mu_c;
}

double SA_state::get_sigma(){
    return sigma_c;
}

state SA_state::get_state(){
    return current_state;
}

vector<double> SA_state::get_H_storage(){
    return H_storage;
}

vector<double> SA_state::get_mu_storage(){
    return mu_storage;
}

vector<double> SA_state::get_sigma_storage(){
    return sigma_storage;
}

//printing function
void SA_state::print_H_storage(string filename){
    int k = H_storage.size();
    ofstream out;
    out.open(filename);
    for(int i=0; i < k; i++){
        out << i << " " << H_storage[i] << endl;
    }
    out.close();
}

void SA_state::print_mu_storage(string filename){
    ofstream out;
    out.open(filename);
    for(int i=0; i < mu_storage.size(); i++){
        out << i << " " << mu_storage[i] << endl;
    }
    out.close();
}

void SA_state::print_sigma_storage(string filename){
    ofstream out;
    out.open(filename);
    for(int i=0; i < sigma_storage.size(); i++){
        out << i << " " << sigma_storage[i] << endl;
    }
    out.close();
}

void SA_state::print_H_block(string filename){
    vector <double> H_vector = current_state.H_eval(rnd); 
    load_block_ave_it(H_vector, filename); // evaluation of comulative average and error
}

void SA_state::print_psi2_points(string filename){
    current_state.thermalization(rnd);
    current_state.sample_psi2(rnd, filename);
}


void SA_state::search_ground(){

    // variable for the metropolis algorithm
    double mu_s = 0.5; // step for mu
    double sigma_s = 0.5; // step for sigma

    pair<double, double> H_pair;

    for(int i=0; i<n_t; i++){
        double beta = beta_i * pow(i+1, 1.2);
        double T = 1/beta;
        for (int j=0; j<n_c; j++){
            double mu_n = mu_c + rnd.Rannyu(-1, 1) * mu_s;
            double sigma_n = sigma_c + rnd.Rannyu(-1, 1) * sigma_s;
            state new_state(mu_n, sigma_n);
            if (sigma_n <= 0.1) {
                continue;
            }
            new_state.thermalization(rnd);
            H_pair = new_state.H_value(rnd);
            double H_n = H_pair.first;

            if (std::isnan(H_n) || std::isinf(H_n)){ 
                continue;
            }
            
            double a = min(1, exp(- beta * (H_n - H_val)));
            if(rnd.Rannyu() > a){
            }else{
                mu_c = mu_n;
                sigma_c = sigma_n;
                H_val = H_n;
                H_storage.push_back(H_n);
                mu_storage.push_back(mu_n);
                sigma_storage.push_back(sigma_n);
                current_state = new_state;
            }
        }
    }
}