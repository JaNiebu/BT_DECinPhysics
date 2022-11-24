#include<iostream>
#include<fstream>
#include<vector>
#include<array>
#include<map>
#include<iterator>
#include<algorithm>
#include<set>
#include<random>
#include<math.h>
#include<string>
#include "../Headers/TriangulationFunc.h"
#include "../Headers/ReadingMesh.h"
#include "../Headers/Signum.h"
#include "../Headers/DiagonalHodge.h"
//#include "../alglib-cpp/src/linalg.h"
#include "../alglib-cpp/src/solvers.h"
#include "../alglib-cpp/src/stdafx.h"
#include "../Headers/sparseMMT.h"
#include "../Headers/sparseMM.h"

using namespace std; 

double boundaryCondition( array<double,2> X )
{
    double x = X[0];
    double y = X[1];
    if ( x==0 )
    {
        return 10.;
    }
    else if ( x == 1 )
    {
        return 0.;
    }
    else
    {
        return 10*(1-x);
    }
    
    
    
}

    
int main(){

vector<array<double,2>> V = ReadVertices( "/home/s-janieb/BaThesisv2/Triangulation/Vertices.txt" );
vector<array<int,3>> F = ReadFaces( "/home/s-janieb/BaThesisv2/Triangulation/Triangulation.txt" );

vector<array<int,2>> E;
for (int i = 0; i < F.size(); i++)
{
    array<int,3> t = F[i];
    vector<array<int,2>> E_t = getEdges( t );
    for (int j = 0; j < E_t.size(); j++)
    {
        set<int> e = {E_t[j][0], E_t[j][1]};
        bool status = false;
        for (int k = 0; k < E.size(); k++)
        {
            set<int> e_prime = {E[k][0], E[k][1]};
            if ( e == e_prime )
            {
                status = true;
                break;
            }
            
        }
        if ( status == false )
        {
            E.push_back(array<int,2> {E_t[j][0] , E_t[j][1]});
        }
    }
}
ofstream edges{"Edges.txt"};
for (int i = 0; i < E.size(); i++)
{
    edges << E[i][0] << " " << E[i][1] << endl;
}


vector<int> V_int;
vector<int> V_bound;
for (int i = 0; i < V.size(); i++)
{
    if ( (0<V[i][0]<1)&&(0<V[i][1]<1) )
    {
        V_int.push_back(i);
    }
    else
    {
        V_bound.push_back(i);
    }

}
map<int,double> g;
for (int i = 0; i < V_bound.size(); i++)
{
    int j = V_bound[i];
    //g.push_back( boundaryCondition( V[j] ) );
    g[j] = 0.;
}


//map<array<int,2> , double> PBound2;
Sparse PBound1;
Sparse MinusPBound1;
for (int i = 0; i < E.size(); i++)
{
    int i_0 = E[i][0];
    int i_1 = E[i][1];

    PBound1.rows.push_back(i_0);
    PBound1.column.push_back(i);
    PBound1.values.push_back(-1.);

    PBound1.rows.push_back(i_1);
    PBound1.column.push_back(i);
    PBound1.values.push_back(1.);

    MinusPBound1.rows.push_back(i_0);
    MinusPBound1.column.push_back(i);
    MinusPBound1.values.push_back(1.);

    MinusPBound1.rows.push_back(i_1);
    MinusPBound1.column.push_back(i);
    MinusPBound1.values.push_back(-1.);
}

//create diagonal (circumcentric) Hodge operator for dimension 1
Sparse H1D = DiagHodge1D( V , E , F );

Sparse A_2 = SparseMM(MinusPBound1, SparseMMT(H1D,MinusPBound1,E.size(),E.size(),V.size()), V.size(), E.size(), V.size() );

vector<double> rhsFromBc;
for (int i = 0; i < V.size(); i++)
{
    rhsFromBc.push_back(0);
}

int i=0;
while( i < A_2.column.size() )
{
    int col = A_2.column[i];
    for (int j = 0; j < V_bound.size(); j++)
    {
        if ( V_bound[j]==col )
        {
            int row = A_2.rows[i];
            rhsFromBc[row] += -1*A_2.values[i]*g[j];
            
            A_2.rows.erase(A_2.rows.begin()+i);
            A_2.column.erase(A_2.column.begin()+i);
            A_2.values.erase(A_2.values.begin()+i);

        }
        else
        {
            i++;
        }
        
        
    }
    
}



//creating the matrix for the discrete laplacian

alglib::sparsematrix A;
alglib::sparsecreate( V.size() , V_int.size() , A );
for (int i = 0; i < A_2.column.size(); i++)
{
    alglib::sparseset(A, A_2.rows[i], A_2.column[i], A_2.values[i]);
}

alglib::sparseconverttocrs(A);
/* output: discrete operator A = d*d
for (int i = 0; i < V.size(); i++)
{
    for (int j = 0; j < V.size(); j++)
    {
        cout << alglib::sparseget(A,i,j) << " ";
    }
    cout << endl;

}
*/


//rigth side
alglib::real_1d_array b;

b.setlength( V.size() );
for (int i = 0; i < V.size(); i++)
{  
    b(i) = rhsFromBc[i];
    if ( distance(V[i],array<double,2> {.5,.5})<0.2 )
    {
        b(i) += 100.;
    }
    else
    {
        b(i) += 0.;
    } 
    
}
/*
for (int i = 0; i < b.length(); i++)
{
    cout << b[i] << endl;
}
*/


alglib::linlsqrstate s;
alglib::linlsqrreport rep;
alglib::real_1d_array x;
alglib::linlsqrcreate( V.size() , V.size() , s);
alglib::linlsqrsolvesparse( s , A , b);
alglib::linlsqrresults( s , x , rep );

ofstream sol{"solution.txt"};
for (int i = 0; i < x.length(); i++)
{
    sol << x[i] << endl;
}

//printf("%d\n", int(rep.terminationtype));
//printf("%s\n", x.tostring(V.size()).c_str());


}