#include <iostream>
#include <cmath> 
#include <armadillo>
#include "../PRNG/random.h" // for random generetor
#include "position.h"

using namespace std;
using namespace arma;

// Costruttore di default
position::position(){
  pos = vec(3, fill::zeros); // initialize coordinate to zero
}


// Specific costructor with coordinates
position::position(double x_val, double y_val, double z_val) {
  pos = {x_val, y_val, z_val}; // Inizializza le coordinate con i valori specificati
}

// method to set the coordinates
void position::setX(double x_val) { pos(0) = x_val; }
void position::setY(double y_val) { pos(1) = y_val; }
void position::setZ(double z_val) { pos(2) = z_val; }


void position::sferichex(double r, double theta, double fi) { pos(0) = r * (sin(theta)* cos(fi));  }
void position::sferichey(double r, double theta, double fi) { pos(1) = r * (sin(theta)* sin(fi));  }
void position::sferichez(double r, double theta, double fi) { pos(2) = r * (cos(theta));           }


double position::getX() const { return pos(0); }
double position::getY() const { return pos(1); }
double position::getZ() const { return pos(2); }


double position::distance() const {
  double d = arma::norm(pos - vec({0.0, 0.0, 0.0})); 
  return d;
}

void position::print() const {
  cout << pos.t();
}

void position:: next_s(Random &r, double l_s){
  double step = l_s; //step length
  double theta = acos(2*r.Rannyu() - 1); 
  double phi = 2 * M_PI * r.Rannyu();
  vec a = {step*sin(theta)*cos(phi), step*sin(theta)*sin(phi), step*cos(theta)};
  pos += a; // Update position by adding the step vector
}

position position::new_pos(Random &r, double l_s){
  position new_pos = *this;
  new_pos.next_s(r, l_s);
  return new_pos;
}

void position::next_s_discrete(Random &r, double l_s) {
  int k = int(r.Rannyu(0,6));
  int axis = k / 2;       
  int direction = (k % 2 == 0) ? 1 : -1; 
  pos(axis) += direction * l_s; // Update position by adding or subtracting l_s along the chosen axis
}

