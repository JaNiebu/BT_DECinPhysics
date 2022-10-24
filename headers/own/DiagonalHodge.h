#ifndef DiagonalHodge
#define DiagonalHodge
#include<map>
#include<array>
#include<vector>
#include<cmath>
#include "Signum.h"
#include "TriangulationFunc.h"
#include "Circumcenter.h"
#include "sparse.h"


using namespace std;

double TriArea( array<double,2> A , array<double,2> B , array<double,2> C );
double CircDualArea( int v , vector<array<double,2>> V , vector<array<int,3>> F );
double CircDualLength( array<int,2> e , vector<array<double,2>> V , vector<array<int,3>> F );
vector<double> DiagHodge0D( vector<array<double,2>> V , vector<array<int,3>> F );
Sparse DiagHodge1D( vector<array<double,2>> V , vector<array<int,2>> E , vector<array<int,3>> F );
vector<double> DiagHodge2D( vector<array<double,2>> V , vector<array<int,3>> F );

/*
double TriArea( array<double,2> A , array<double,2> B , array<double,2> C )
{
    double a = distance( B , C );
    double b = distance( A , C );
    double c = distance( A , B );

    double s = (a+b+c)/2;
    double Area = pow( (s*(s-a)*(s-b)*(s-c)), 0.5);
    return Area;
}

double CircDualArea( int v , vector<array<double,2>> V , vector<array<int,3>> F )
{
    double Area = 0;
    //Find all triangles containing v
    for (int i = 0; i < F.size(); i++)
    {
        array<int,3> t = F[i];
        if ( (t[0]==v)||(t[1]==v)||(t[2]==v) )
        {
            array<double,2> c_t = circumcenter2D( t , V );
            for (int j = 0; j < t.size(); j++)
            {
                if ( t[j]==v )
                {
                    continue;
                }
                else
                {
                    array<int,2> e = { v , t[j] };
                    array<double,2> c_e = CircCent1D( e , V );
                    Area += TriArea( c_t , c_e , V[v] );
                }
            
            }

        }
        
    }
    return Area;
    
}

double CircDualLength( array<int,2> e , vector<array<double,2>> V , vector<array<int,3>> F )
{
    double Length = 0;
    //Find all containing triangles
    for (int i = 0; i < F.size(); i++)
    {
        int count = 0;
        vector<array<int,2>> E_t = getEdges( F[i] );
        for (int j = 0; j < E_t.size(); j++)
        {
            array<int,2> e_2 = E_t[j];
            if ( (e_2[0]==e[0])&&(e_2[1]==e[1]) || (e_2[0]==e[1])&&(e_2[1]==e[0]) )
            {
                array<double,2> c_t = circumcenter2D( F[i] , V );
                array<double,2> c_e = CircCent1D( e , V );
                Length += distance( c_t , c_e );
            }
            
        }
        
    }
    return Length;

}

vector<double> DiagHodge0D( vector<array<double,2>> V , vector<array<int,3>> F )
{
    vector<double> DiagHodge0D;
    for (int i = 0; i < V.size(); i++)
    {
        DiagHodge0D.push_back( CircDualArea( i , V , F ) );
    }
    return DiagHodge0D;

}

Sparse DiagHodge1D( vector<array<double,2>> V , vector<array<int,2>> E , vector<array<int,3>> F )
{
    Sparse DiagHodge1D;
    for (int i = 0; i < E.size(); i++)
    {
        double ratio = CircDualLength( E[i] , V , F ) / distance( V[E[i][0]] , V[E[i][1]] );
        DiagHodge1D.column.push_back(i);
        DiagHodge1D.rows.push_back(i);
        DiagHodge1D.values.push_back( ratio );
    }
    return DiagHodge1D;

}

vector<double> DiagHodge2D( vector<array<double,2>> V , vector<array<int,3>> F )
{
    vector<double> DiagHodge2D;
    for (int i = 0; i < F.size(); i++)
    {
        int i_0 = F[i][0];
        int i_1 = F[i][1];
        int i_2 = F[i][2];
        double ratio = 1/TriArea( V[i_0] , V[i_1] , V[i_2] );
        DiagHodge2D.push_back( ratio );
    }
    return DiagHodge2D;
    
}
*/
#endif