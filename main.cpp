#include "headers/headers.h"
//Preamble for numerical processing/calculation
#include "headers/NumCalc/sparseMM.h"
#include "headers/NumCalc/sparseMMT.h"
#include "headers/NumCalc/Integrators.h"
#include "headers/OpConstruct/ReadWriteOp.h"
//Preamble for meshing process
#include "headers/Meshing/MeshGen.h"
#include "headers/Meshing/ReadingMesh.h"
#include "headers/Meshing/TriangulationFunc.h"
//Preamble for operator construction process
#include "headers/OpConstruct/BoundConstruct.h"
#include "headers/OpConstruct/DiagonalHodge.h"
#include "headers/Meshing/BoundaryElements.h"



array<double,2> Field( array<double,2> position , double alpha )
{
    array<double,2> Value = { -1.*pow( (position[0]-0.5) , alpha ), -1.*(position[1]-0.25) };
    return Value;
}

double SourceField( array<double,2> position )
{
    double x = position[0];
    double y = position[1];
    double Val;
    if ( distance(position, array<double,2> {.5,.5}) < 0.1 )
    {
        Val = 1;
    }
    else
    {
        Val = 0;
    }
    return Val;
}


int main()
{

    /*
    1. Meshing
        includes:
        - generation of nodes,
        - writing them to file,
        - reading them as input,
        - construct delaunay triangulation,
        - write triangulation to file
    */
    Mesh(2 , "Meshdata/TriVertices.txt");
    
    vector<array<double,2>> V = ReadVertices("Meshdata/TriVertices.txt");

    vector<array<int,3>> F = BowyerWatson(V);

    WriteFaces(F, "Meshdata/Faces.txt");

    vector<array<int,2>> E = GetAllEdges(F);

    WriteEdges(E, "Meshdata/Edges.txt");


    /*
    2. Operator Construction
        includes:
        - construction of boundary operators/ext. deriv.,
        - (diagonal) hodges
        - combined operator for specfic problem e.g. (modified) laplace
    */
    Sparse PrimalBoundary1 = ConstructBound1( E , false );
    WriteSparse( "Solutions/PrimalBoundary1Operator.txt" , PrimalBoundary1 , V.size() , E.size());

    Sparse MinusPrimalBoundary1 = ConstructBound1( E , true );
    WriteSparse( "Solutions/MinusPrimalBoundary1Operator.txt" , MinusPrimalBoundary1 , V.size() , E.size() );

    Sparse PrimalBoundary2 = ConstructBound2( F , E , false );
    WriteSparse( "Solutions/PrimalBoundary2Operator.txt" , PrimalBoundary2 , E.size() , F.size() );

    Sparse Hodge0 = DiagHodge0D( V , F );

    Sparse Hodge1 = DiagHodge1D( V , E , F );

    //modified laplace operator
    Sparse L = SparseMM( MinusPrimalBoundary1 , SparseMMT( Hodge1 , PrimalBoundary1,E.size(),E.size() ), V.size(), E.size());
    //normal divergence operator
    Sparse DIV = SparseInvMM( Hodge0 , SparseMM( MinusPrimalBoundary1 , Hodge1 , V.size() , E.size() ) , V.size() , E.size() );

    
    /*
    3. Problem input
        includes:
        - identifying the boundary elements (nodes/edges/faces)
        - generating and/or reading boundary conditions
        - generating and/or reading function data on interior
    */
    vector<int> BoundaryEdgesIndices = GetBoundaryEdgesIndices( E.size() , PrimalBoundary2 );
    vector<array<int,2>> BoundaryEdges;
    for (int i = 0; i < BoundaryEdgesIndices.size(); i++)
    {
        BoundaryEdges.push_back( E[BoundaryEdgesIndices[i]] );
    }
    cout << BoundaryEdgesIndices.size() << endl;
    vector<int> BoundaryNodesIndices = GetBoundaryNodesIndices( V.size() , BoundaryEdges );
    

    array<double,3> alpha_param = {1.,2.,3.};

    for (int i = 0; i < alpha_param.size(); i++)
    {
        vector<double> cochain;
        double alpha = alpha_param[i];
        for (int j = 0; j < E.size(); j++)
        {   
            array<double,2> initial_point = V[E[j][0]];
            array<double,2> final_point = V[E[j][1]];
            cochain.push_back( LineIntegral( Field , alpha , initial_point , final_point ));
        }

        vector<double> h_values;
        for (int j = 0; j < BoundaryNodesIndices.size(); j++)
        {   
            cout << j << endl;
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
        for (int j = 0; j < h_values.size(); j++)
        {
            cout << h_values[j] << endl;
        }
        
        
        
        vector<double> Solution = SparseVecMR( DIV , cochain , V.size() );
        for (int i = 0; i < BoundaryNodesIndices.size(); i++)
        {
            Solution[BoundaryNodesIndices[i]] += h_values[i];
        }
        string adress = "Solutions/SolFor" + to_string(alpha);
        ofstream out{adress};
        for (int j = 0; j < Solution.size(); j++)
        {
            out << Solution[j] << endl;
        }
        string Erradress = "Solutions/ErrorFor" + to_string(alpha);
        ofstream ErrOut{Erradress};
        for (int j = 0; j < Solution.size(); j++)
        {
            ErrOut << Solution[j] - (-1.*alpha*pow(V[j][0]-0.5,alpha-1.)-1) << endl;
        }
        
    }
    


    /*
    4. Solving the System
    */



    /*
    5. Writing to file
    */
  

}