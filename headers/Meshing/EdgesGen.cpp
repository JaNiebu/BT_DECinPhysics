#include "EdgesGen.h"

vector< array<int, 2> > getEdges( array<int,3> t ){
    vector< array<int, 2> > E_t;
    for (int i = 0; i < 3; i++)
    {
        array<int, 2> e = { t[i], t[(i+1)%3] };
        E_t.push_back( e );
    }
    return E_t;
}


vector<array<int,2>> GetAllEdges( vector<array<int,3>> F )
{
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
    return E;
}



