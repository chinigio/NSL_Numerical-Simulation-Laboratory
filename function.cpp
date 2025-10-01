#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "function.h"

using namespace std;


double min(double a, double b){
    if(a>b){
        return b;
    }else
        return a;
}

// Function for statistical uncertainty estimation
double error(vector <double> AV, vector <double> AV2, int n){  
    if(n==0){
        return 0;
    }else
        return sqrt((AV2[n] - pow(AV[n],2))/(n));
};

double error(double ave, double ave2){
    return sqrt(ave2 - pow(ave,2));
}

// Function to evaluate the average and the error of a block of data
pair< vector<double>, vector <double> > media_block(vector<double> blocks){
    int N = blocks.size();
    vector <double> blocks2; 
    for(int i=0; i<N; i++){
        blocks2.push_back(pow(blocks[i],2));
    }
    // evaluation of comulative average and error
    vector <double> sum_prog(N,0);
    vector <double> su2_prog(N,0);
    vector <double> err_prog(N,0);
    for(int i=0; i<N; i++){
        for(int j=0; j<i+1; j++){
            sum_prog[i] += blocks[j];
            su2_prog[i] += blocks2[j];
        }
        sum_prog[i]/=(i+1); // Cumulative average
        su2_prog[i]/=(i+1); // Cumulative square average
        err_prog[i] = error(sum_prog, su2_prog,i); // Statistical uncertainty
    }
    return make_pair(sum_prog, err_prog);
}

// Function to evaluate the average and the error of a block of data
pair<double,double> block_ave(vector<double> blocks){
    int N = blocks.size();
    vector<double> v = media_block(blocks).first;
    vector<double> err = media_block(blocks).second;
    return make_pair(v[N-1], err[N-1]);
}


// Function to evaluate and load to datafile the average and the error of a block of data
void load_block_ave(vector<double> v, string namefile){
    pair<double, double> result = block_ave(v);
    double sum_prog = result.first;
    double err_prog = result.second;

    ofstream out;
    out.open(namefile);
    for(int i=0; i<v.size(); i++){
        out << i << " " << sum_prog << " " << err_prog << endl;
    }
    out.close();
}

// Function to evaluate and load to datafile the average and the error of a block of data
void load_block_ave_it(vector<double> v, string namefile){
    pair< vector<double>, vector<double> > result = media_block(v);
    vector<double> sum_prog = result.first;
    vector<double> err_prog = result.second;

    ofstream out;
    out.open(namefile);
    for(int i=0; i<v.size(); i++){
        out << i << " " << sum_prog[i] << " " << err_prog[i] << endl;
    }
    out.close();
}