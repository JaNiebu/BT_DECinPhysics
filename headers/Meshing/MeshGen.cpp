#include "MeshGen.h"

void RandomNodes(int Nodesnumber , string adress, int seed )
{
    vector<array<double,2>> V;
    V.push_back( array<double,2> {0.,0.});
    V.push_back( array<double,2> {1.,0.});
    V.push_back( array<double,2> {0.,1.});
    V.push_back( array<double,2> {1.,1.});
    mt19937 gen( seed );
    uniform_real_distribution<double> distribution(.0,1.);
    for (int i = 0; i < Nodesnumber; i++)
    {
        double x = distribution(gen);
        double y = distribution(gen);
        V.push_back( array<double,2> {x,y} );
    }
    ofstream out{ adress };
    for (int i = 0; i < V.size(); i++)
    {
        out << V[i][0] << " " << V[i][1] << endl;
    }

}

void Mesh(int n , string adress ){

//defining the basics of the mesh generation
     //scale of system ie number of vertices per direction
double d = 1/(1.0*(n));     //grid constant
//vector<array<double,2>> V = {};         //container for all vertices
//ofstream Vertices{"SqVertices.txt"};
ofstream TriVert{ adress };
//ofstream Faces{"SqFaces.txt"};

//square lattice points
/*
for (int i = 0; i < n+1; i++)
{
    for (int j = 0; j < n+1; j++)
    {
        double x = j*d;
        double y = i*d;
        Vertices << x << " " << y << endl;
    }
    
}

for (int i = 0; i < n; i++)
{
    for (int j = 0; j < n; j++)
    {
        int k_0 = i*(n + 1) -1 + j+1;
        int k_1 = k_0+1;
        int k_2 = k_0 + n+2;
        int k_3 = k_0 + n+1;
        Faces << k_0 << " " << k_1 << " " << k_2 << " " << k_3 << endl;
    }
    
}
*/

//near-regular trianulation of unit square

for (int i = 0; i < n+1; i++)
{
    for (int j = 0; j < (n+2); j++)
    {
        if ( (i % 2 == 0)&&(j<(n+1)) )
        {
            double x = j*d;
            double y = i*d*0.5*pow(3,.5);
            double v[2] = {x,y};
            TriVert << v[0] << " " << v[1] << endl;
        }
        else if( (i%2==1) )
        {
            if ( j == 0 )
            {
                double x = 0.;
                double y = d*0.5*pow(3,.5)*(i);
                double v [2] = {x,y};
                TriVert << v[0] << " " << v[1] << endl; 
            }
            else if ( j == (n+1) )
            {
                double x = 1;
                double y = d*0.5*pow(3,.5)*(i);
                double v [2] = {x,y};
                TriVert << v[0] << " " << v[1] << endl;
            }
            else
            {
                double x = d*((j-1)+0.5);
                double y = d*0.5*pow(3,.5)*i;
                double v [2] = {x,y};
                TriVert << v[0] << " " << v[1] << endl;
            }
             
        }     
         
    }

}
}