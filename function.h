#include <vector>

using namespace std;

double min(double x, double y);
double error(vector <double> AV, vector <double> AV2, int n);
double error(double ave, double ave2);
pair< vector<double>, vector <double> > media_block(vector<double> blocks);
pair<double, double> block_ave(vector<double> blocks);
void load_block_ave(vector<double>, string namefile);
void load_block_ave_it(vector<double>, string namefile);