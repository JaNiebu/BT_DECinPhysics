#ifndef TriangulationFunc
#define TriangulationFunc
#include "../headers.h"
#include "../GeoFunc/Circumcenter.h"
#include "EdgesGen.h"

using namespace std;

double distance(array<double, 2> x , array<double, 2> y);
double circumradius( array<int,3> t , vector<array<double, 2>> V );
bool Circ( array<int,3> t , array<double, 2> v , vector<array<double, 2>> V );
vector<array<int, 3>> sharet( array<int,3> t, array<int, 2> e , vector< array<int, 3>> T);
vector< array<int, 3>> BowyerWatson( vector<array<double, 2>> &vec );
int Orient2D( array<int, 3> t , vector<array<double, 2>> V);
array<double, 3> BarycentCoord( array<double, 2> v , array<int, 3> t , vector<array<double, 2>> V );
bool InsideTriCheck( array<double, 2> v , array<int, 3> t , vector<array<double, 2>> V );

    
//Defining functions
/*
    double distance(array<double, 2> x , array<double, 2> y){
        double arg = pow( (x[0]-y[0]), 2 ) + pow( (x[1]-y[1]), 2 );
        double dist = pow( arg , 0.5);
        return dist;
    }

    double circumradius( array<int,3> t , vector<array<double, 2>> V ){
        int i_0 = t[0];
        int i_1 = t[1];
        int i_2 = t[2];
        
        double a = distance( V[i_0] , V[i_1] );
        double b = distance( V[i_1] , V[i_2] );
        double c = distance( V[i_2] , V[i_0] );   

        double arg = ( pow(a,2.) * pow(b,2.) * pow(c,2.))/((a+b+c)*(-a+b+c)*(a-b+c)*(a+b-c));
        double CircRad = pow( arg, 0.5);
        return CircRad;     

    }

    bool Circ( array<int,3> t , array<double, 2> v , vector<array<double, 2>> V ){
        array<double, 2> U = circumcenter2D(t , V);
        double R = circumradius(t , V);
        double d = distance( v , U );
        if ( (d <= R) )
        {
            //cout << "Circ true" << endl;
            return true;
        }
        else
        {
            //cout << "Circ false" << endl;
            return false;
        }
        
        
    }

    vector< array<int, 2> > getEdges( array<int,3> t ){
        vector< array<int, 2> > E_t;
        for (int i = 0; i < 3; i++)
        {
            array<int, 2> e = { t[i], t[(i+1)%3] };
            E_t.push_back( e );
        }
        return E_t;
    }

    vector<array<int, 3>> sharet( array<int,3> t, array<int, 2> e , vector< array<int, 3>> T){
        vector<array<int, 3>> sharingTriangles;
        set<int> V_t = { t[0], t[1], t[2] };
        for (int i = 0; i < T.size(); i++)
        {
            array<int, 3> t_prime =  { T[i][0], T[i][1], T[i][2] } ;
            set<int> V_t_prime = { t_prime[0], t_prime[1], t_prime[2] };
            vector< array<int, 2> > E_t_prime = getEdges( t_prime );
            
            for (int j = 0; j < E_t_prime.size(); j++)
            {
                array<int, 2> e_prime =  { E_t_prime[j][0], E_t_prime[j][1] };
                if ( (e_prime[0] == e[0]) && (e_prime[1] == e[1]) && (V_t != V_t_prime) )
                {
                    sharingTriangles.push_back(t_prime);
                    continue;
                }
                else if ( (e_prime[0] == e[1]) && (e_prime[1] == e[0]) && (V_t != V_t_prime))
                {
                    sharingTriangles.push_back(t_prime);
                    continue;
                }
                
                
            }
            
        }
        return sharingTriangles;

    }

    vector< array<int, 3>> BowyerWatson( vector<array<double, 2>> &vec ){
        int N = vec.size();
        vector< array<int, 3>> T;
        vec.push_back(array<double, 2> {-49.5 , -10.});
        vec.push_back(array<double, 2> {50.5 , -10.});
        vec.push_back(array<double, 2> {0.5 , 50.});
        array<int, 3> SupTri={N,(N+1),(N+2)};
        T.push_back( SupTri );

        for (int i = 0; i < N; i++)
        {
            array<double, 2> v = vec[i];
            vector< array<int, 3>> badTriangles;                                                    //Routine for finding bad triangles
            for (int j = 0; j < T.size(); j++)
            {
                array<int, 3> t = { T[j][0], T[j][1], T[j][2] };
                if ( Circ( t , v , vec ) )                                                          //Check for Circumcircle Condition
                {
                    badTriangles.push_back( t );
                }

            }
            vector<array<int, 2>> polygon = {};                                                     //Finding the polygonal hole around v
            for (int j = 0; j < badTriangles.size(); j++)
            {
                array<int, 3> t = { badTriangles[j][0], badTriangles[j][1], badTriangles[j][2] };
                vector<array<int, 2>> E_t = getEdges( t );

                for (int k = 0; k < E_t.size(); k++)
                {
                    array<int, 2> e = { E_t[k][0], E_t[k][1] };
                    vector<array<int, 3>> sharingTriangles = sharet(t, e, badTriangles);
                    
                    if ( sharingTriangles.empty() )
                    {
                        polygon.push_back(e);
                    }

                }
            
            }
            for (int j = 0; j < badTriangles.size(); j++)
            {
                array<int, 3> t = { badTriangles[j][0], badTriangles[j][1], badTriangles[j][2] };
                T.erase(remove(T.begin(), T.end(), t), T.end());
            }
            for (int j = 0; j < polygon.size(); j++)
            {
                array<int, 3> t = { polygon[j][0], polygon[j][1], i};
                T.push_back( t );
            }
        
        }


        //Deleting all Triangles and nodes related to the Supertriangle
        int i = 0;
        while ( i < T.size() )
        {
            array<int, 3> t = { T[i][0], T[i][1], T[i][2] };
            if ( ((t[0] >= N) || (t[1] >= N) || (t[2] >= N)) )
            {
                T.erase( T.begin()+i );
                continue;
            }
            else
            {
                i++;
                continue;
            }
        }
        vec.pop_back();
        vec.pop_back();
        vec.pop_back();
        
        
        return T;
    }

    int Orient2D( array<int, 3> t , vector<array<double, 2>> V){
        array<double ,2> a = V[t[0]];
        array<double, 2> b = V[t[1]];
        array<double, 2> c = V[t[2]];

        double SignArea = (a[0]-c[0])*(b[1]-c[1]) - (b[0]-c[0])*(a[1]-c[1]);
        int s;
        if ( SignArea < 0 )
        {
            int s = -1;
        }
        else if ( SignArea > 0 )
        {
            int s = 1;
        }
        else if ( SignArea == 0)
        {
            int s = 0;
        }   
        return s;    
    }

    array<double, 3> BarycentCoord( array<double, 2> v , array<int, 3> t , vector<array<double, 2>> V ){
        array<double, 2> v_0 = V[t[0]];
        array<double, 2> v_1 = V[t[1]];
        array<double, 2> v_2 = V[t[2]];

        double lambda_1 = ( (v_1[1]-v_2[1])*(v[0]-v_2[0])+(v_2[0]-v_1[0])*(v[1]-v_2[1]) )/( (v_1[1]-v_2[1])*(v_0[0]-v_2[0])+(v_2[0]-v_1[0])*(v_0[1]-v_2[1]) );
        double lambda_2 = ( (v_2[1]-v_0[1])*(v[0]-v_2[0])+(v_0[0]-v_2[0])*(v[1]-v_2[1]) )/( (v_1[1]-v_2[1])*(v_0[0]-v_2[0])+(v_2[0]-v_1[0])*(v_0[1]-v_2[1]) );
        double lambda_3 = 1 - lambda_1 - lambda_2;

        array<double, 3> Lambda = { lambda_1, lambda_2 , lambda_3 };
        return Lambda;
    }

    bool InsideTriCheck( array<double, 2> v , array<int, 3> t , vector<array<double, 2>> V ){
        array<double, 3> v_t = BarycentCoord( v , t , V );
        //cout << v_t[0] << " " << v_t[1] << " " << v_t[2] << endl;
        if ( ( (v_t[0]<0) || (v_t[1]<0) || (v_t[2]<0) ) )
        {
            return true;
            cout << "not well centered" << endl;
        }
        else
        {
            return false;
        }
    }

    vector<array<int,3>> BarySubDiv( array<int,3> t , vector<array<double,2>> &V )
    {
        int n = V.size();
        array<double,2> A = V[t[0]];
        array<double,2> B = V[t[1]];
        array<double,2> C = V[t[2]];
        array<double,2> BarCent = { (A[0]+B[0]+C[0])/3 , (A[1]+B[1]+C[1])/3 };

        V.push_back( BarCent );
        vector<array<int,3>> SubDiv;
        for (int i = 0; i < t.size(); i++)
        {
            SubDiv.push_back( array<int,3> {t[i], t[(i+1)%3], n });
        }
        return SubDiv;
        
    }
*/
/*
    vector<array<int,3>> WellCenteredFix( vector<array<int,3>> &F , vector<array<double,2>> &V )
    {
        vector<array<int,3>> T_new;
        int i = 0;
        while ( i < F.size() )      
        {
            array<int,3> t = F[i];
            array<double,2> c_t = circumcenter2D( t , V );
            if ( InsideTriCheck( c_t , t , V ) )
            {
                int i_max;
                vector<array<int,2>> E_t = getEdges( t );
                double l_max = 0;
                for (int j = 0; j < E_t.size(); j++)
                {
                    double l = distance( V[E_t[j][0]] , V[E_t[j][1]] ) ;
                    if ( l > l_max )
                    {
                        l_max = l;
                        int i_max = j;
                    }
                    else if (l == l_max )
                    {
                        cout << "two equally long sides" << endl;
                    }
                    
                }
                array<double,2> v_new = CircCent1D( E_t[i_max] , V );
                V.push_back( v_new );

                vector<array<int,3>> T_new;
                for (int j = 0; j < ; j++)
                {
                
                }
                
                
                vector<array<int,3>> SubDiv = BarySubDiv( t , V );
                for (int j = 0; j < SubDiv.size(); j++)
                {
                    T_new.push_back( SubDiv[j] );
                }
                F.erase( F.begin()+i );

            }
            else
            {
                i++;
            }
            
            
        }
        return T_new;

    }*/

#endif