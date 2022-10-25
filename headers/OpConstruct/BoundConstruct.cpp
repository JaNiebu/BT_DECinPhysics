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

Sparse ConstructBound2( vector<array<int,3>> Faces , vector<array<int,2>> Edges , bool SignChange )
{
    Sparse Boundary2;
    for (int i = 0; i < Faces.size(); i++)
    {
        array<int,3> t = Faces[i];
        vector<array<int,2>> EdgesOfFace = getEdges( t );
        for (int j = 0; j < Edges.size(); j++)
        {
            for (int k = 0; k < EdgesOfFace.size(); k++)
            {
                int OrientationSign = StrictOrientationComparison( Edges[j] , EdgesOfFace[k] );
                if ( OrientationSign == 0 )
                {
                    continue;
                }
                
                else if ( OrientationSign == 1 )
                {
                    Boundary2.column.push_back( i );
                    Boundary2.rows.push_back( j );
                    if ( SignChange == false )
                    {
                        Boundary2.values.push_back( 1. );
                    }
                    else if ( SignChange == true )
                    {
                        Boundary2.values.push_back( -1. );
                    } 
                    
                }
                else if ( OrientationSign == -1 )
                {
                    Boundary2.column.push_back( i );
                    Boundary2.rows.push_back( j );
                    if ( SignChange == false )
                    {
                        Boundary2.values.push_back( -1. );
                    }
                    else if ( SignChange == true )
                    {
                        Boundary2.values.push_back( 1. );
                    } 
                    
                }
                
                
            }
            
            
        }
        
    }
    return Boundary2;
    
}