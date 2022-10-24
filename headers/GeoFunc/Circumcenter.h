#ifndef Circumcenter
#define Circumcenter
#include "../headers.h"

using namespace std;

array<double,2> circumcenter2D( array<int,3> t , vector<array<double, 2>> V );
array<double,2> CircCent1D( array<int,2> e , vector<array<double,2>> V );


/*
    array<double,2> circumcenter2D( array<int,3> t , vector<array<double, 2>> V )
    {
        double A_x = V[t[0]][0];
        double A_y = V[t[0]][1];
        double B_x = V[t[1]][0];
        double B_y = V[t[1]][1];
        double C_x = V[t[2]][0];
        double C_y = V[t[2]][1];

        double D = 2*(A_x*(B_y-C_y) + B_x*(C_y-A_y) + C_x*(A_y-B_y));
        double U_x = ( (A_x*A_x+A_y*A_y)*(B_y-C_y) + (B_x*B_x+B_y*B_y)*(C_y-A_y) + (C_x*C_x+C_y*C_y)*(A_y-B_y))/D;
        double U_y = ( (A_x*A_x+A_y*A_y)*(C_x-B_x) + (B_x*B_x+B_y*B_y)*(A_x-C_x) + (C_x*C_x+C_y*C_y)*(B_x-A_x))/D;
        array<double,2> U ={ U_x, U_y};
        return U;

    }

    array<double,2> CircCent1D( array<int,2> e , vector<array<double,2>> V )
    {
        double x = (V[e[0]][0] + V[e[1]][0])/2;
        double y = (V[e[0]][1] + V[e[1]][1])/2;
        return array<double,2> { x , y };
    }
*/
#endif