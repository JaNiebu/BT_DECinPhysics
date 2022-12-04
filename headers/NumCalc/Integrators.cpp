#include "Integrators.h"

double precision = 100.;
double LineIntegral( function< array<double,2>(array<double,2>,double)> field, double alpha , array<double,2> initial_point, array<double,2> final_point )
{
    double value = 0;
    array<double,2> edge = { final_point[0]-initial_point[0], final_point[1]-initial_point[1] };
    for (int i = 0; i < precision; i++)
    {
        array<double,2> position = { initial_point[0]+((i+0.5)/precision)*edge[0], initial_point[1]+((i+0.5)/precision)*edge[1] };
        array<double,2> FieldValue = field( position , alpha );
        value += (FieldValue[0]*edge[0] + FieldValue[1]*edge[1])/precision;
    }
    return value;
    
}

double LineFluxIntegral( function< array<double,2>(array<double,2>,double)> field , double alpha , array<double,2> initial_point, array<double,2> final_point )
{
    double value = 0;
    array<double,2> edge = { final_point[0]-initial_point[0], final_point[1]-initial_point[1] };
    array<double,2> edgenormal = { final_point[1]-initial_point[1], -final_point[0]+initial_point[0] };
    for (int i = 0; i < precision; i++)
    {
        array<double,2> position = { initial_point[0]+((i+.5)/precision)*edge[0], initial_point[1]+((i+.5)/precision)*edge[1] };

        value +=  (field( position , alpha )[0]*edgenormal[0] + field( position , alpha )[1]*edgenormal[1])/precision;
    }  
    return value;
    
}

vector<double> GetFluxCorrection( vector<int> BoundaryNodesIndices , vector<array<int,2>> BoundaryEdges , vector<array<double,2>> V , function< array<double,2>(array<double,2>,double)> Field , double alpha  )
{
    vector<double> h_values;
        for (int j = 0; j < BoundaryNodesIndices.size(); j++)
        {   
            int index = BoundaryNodesIndices[j];
            vector<array<double,2>> NodesOfBoundary;
            //find start point of boundary of dual cell of node "index"
            for (int  k = 0; k < BoundaryEdges.size(); k++)
            {
                if ( BoundaryEdges[k][1]==index )
                {
                    array<double,2> edge_vector = { V[index][0]-V[BoundaryEdges[k][0]][0] , V[index][1]-V[BoundaryEdges[k][0]][1] };
                    array<double,2> initial_point = { V[BoundaryEdges[k][0]][0]+edge_vector[0]/2 , V[BoundaryEdges[k][0]][1]+edge_vector[1]/2 };
                    NodesOfBoundary.push_back( initial_point );
                    break;
                } 
            }
            //find end point of boundary of dual cell of node "index"
            for (int k = 0; k < BoundaryEdges.size(); k++)
            {
                if ( BoundaryEdges[k][0]==index)
                {
                    array<double,2> edge_vector = { V[BoundaryEdges[k][1]][0]-V[index][0] , V[BoundaryEdges[k][1]][1]-V[index][1] };
                    array<double,2> final_point = { V[index][0]+edge_vector[0]/2 , V[index][1]+edge_vector[1]/2};
                    NodesOfBoundary.push_back( final_point );
                    break;
                }
            }
            //calculate h value
            h_values.push_back( LineFluxIntegral( Field , alpha , NodesOfBoundary[0] , V[index] ) + LineFluxIntegral( Field , alpha , V[index] , NodesOfBoundary[1] ));
            
        }
    return h_values;
}


