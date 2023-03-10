#include "Signum.h"

int Sgn2D( array<int,3> s_1 , array<int,3> s_2 , vector<array<double,2>> V )
{
    int Sgn_1 = Orient2D( s_1 , V );
    int Sgn_2 = Orient2D( s_2 , V );

    if ( Sgn_1 == Sgn_2 )
    {
        return 1;
    }
    else if ( Sgn_1 == -Sgn_2 )
    {
        return -1;
    }
    else
    {
        set<int> V_s = { s_1[0] , s_1[1] , s_1[2] , s_2[0] , s_2[1] , s_2[2] };
        cout << "error with Sgn2D, number of nodes is " << V_s.size() << endl;
        return 0;
    }
    
}

int Sgn1D( array<int,2> e_1 , array<int,2> e_2 , vector<array<double,2>> V )
{
    double dot = e_1[0]*e_2[0] + e_1[1]*e_2[1];
    int Sign;
    if ( dot<0 )
    {
        Sign = -1;
    }
    else if ( dot > 0 )
    {
        Sign = 1;
    }
    else
    {
        cout << "error, orthorgonal edges" << endl;
        Sign = 0;
    }        
    return Sign;
}

int StrictOrientationComparison( array<int,2> FirstEdge , array<int,2> SecondEdge )
{
    int Sign;
    if ( FirstEdge==SecondEdge )
    {
        Sign = 1;
    }
    else if ( FirstEdge[0]==SecondEdge[1]&&FirstEdge[1]==SecondEdge[0] )
    {
        Sign = -1;
    }
    else
    {
        Sign = 0;
    }
    return Sign;
    
}