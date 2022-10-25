#include "BoundaryElements.h"

vector<int> GetBoundaryEdgesIndices( int Edgesnumber, Sparse Boundary2 )
{
    vector<int> BoundaryEdgesIndices;
    for (int i = 0; i < Edgesnumber; i++)
    {
        //finding all entries in the i-th column
        int count = 0;
        for (int j = 0; j < Boundary2.rows.size(); j++)
        {
            if ( Boundary2.column[j]==i )
            {
                count++;
            }
            
        }
        if ( count==1 )
        {
            BoundaryEdgesIndices.push_back( i );
        }
        
    }
    return BoundaryEdgesIndices;

}

vector<int> GetBoundaryNodesIndices( int Nodesnumber , vector<array<int,2>> BoundaryEdges )
{
    vector<int> BoundaryNodesIndices;
    for (int i = 0; i < Nodesnumber; i++)
    {
        int count = 0;
        for (int j = 0; j < BoundaryEdges.size(); j++)
        {
            if ( (BoundaryEdges[j][0]==i) || (BoundaryEdges[j][1]==i) )
            {
                count++;
            }
            
        }
        if ( count == 2 )
        {
            BoundaryNodesIndices.push_back( i );
        }
        
    }
    return BoundaryNodesIndices;
    
}

vector<int> GetBoundaryFacesIndices( vector<array<int,3>> Faces , vector<int> BoundaryNodesIndices )
{
    vector<int> BoundaryFacesIndices;
    for (int i = 0; i < Faces.size(); i++)
    {
        int count = 0;
        for (int j = 0; j < BoundaryNodesIndices.size(); j++)
        {
            if ( ((Faces[i][0]==j)||(Faces[i][1]==j)||(Faces[i][2]==j)) )
            {
                count++;
            }
            
        }
        if ( count >= 2 )
        {
            BoundaryFacesIndices.push_back( i );
        }
        
    }
    return BoundaryFacesIndices;
    
}


