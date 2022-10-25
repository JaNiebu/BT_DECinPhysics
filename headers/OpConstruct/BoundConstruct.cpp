#include "BoundConstruct.h"


Sparse ConstructBound1( vector<array<int,2>> E , bool SignChange)
{
    Sparse PBound1;
    for (int i = 0; i < E.size(); i++)
    {
        int i_0 = E[i][0];
        int i_1 = E[i][1];

        if ( SignChange == false )
        {
            PBound1.rows.push_back(i_0);
            PBound1.column.push_back(i);
            PBound1.values.push_back(-1.);

            PBound1.rows.push_back(i_1);
            PBound1.column.push_back(i);
            PBound1.values.push_back(1.);
        }
        else if ( SignChange == true )
        {
            PBound1.rows.push_back(i_0);
            PBound1.column.push_back(i);
            PBound1.values.push_back(1.);

            PBound1.rows.push_back(i_1);
            PBound1.column.push_back(i);
            PBound1.values.push_back(-1.);
        }
        else
        {
            cout << "error with boolean in ConstructBound1" << endl;
        }
        
        
        
        

    }
    return PBound1;
}


/*

    Sparse MinusPBound1;

        MinusPBound1.rows.push_back(i_0);
        MinusPBound1.column.push_back(i);
        MinusPBound1.values.push_back(1.);

        MinusPBound1.rows.push_back(i_1);
        MinusPBound1.column.push_back(i);
        MinusPBound1.values.push_back(-1.);

*/