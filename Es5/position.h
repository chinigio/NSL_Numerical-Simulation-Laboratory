#ifndef __position_h__
#define __position_h__


using namespace std;
#include <armadillo>
using namespace arma;

class position {
private:
    vec pos;

public:
    // costructor
    position();
    position(double x_val, double y_val, double z_val);


    void setX(double x_val);
    void setY(double y_val);
    void setZ(double z_val);

    void sferichex(double r, double theta, double fi);
    void sferichey(double r, double theta, double fi);
    void sferichez(double r, double theta, double fi);


    double getX() const;
    double getY() const;
    double getZ() const;


    double distance() const;


    void print() const;

    void next_s(Random &r, double l_s);
    position new_pos(Random &r, double l_s);
    void next_s_discrete(Random &r, double l_s);
};

#endif // position_h
