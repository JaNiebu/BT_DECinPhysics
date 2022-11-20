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
    array<double,2> Value = { pow( (position[0]) , alpha ), position[1] };
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
    vector<int> RingNumber = {1,2,3,4,5,6,7};               //different parameters for meshresolution
    vector<double> alpha_param = {1.,2.,3.,4.,5.,6.,7.,8.};           //different parameters for field exponent
    for (int n = 0; n < RingNumber.size() ; n++)
    {
    int n_max = RingNumber[n];
    

    /*
    1. Meshing
        includes:
        - generation of nodes,
        - writing them to file,
        - reading them as input,
        - construct delaunay triangulation,
        - write triangulation to file
    */
    //RandomNodes( 10 , "Meshdata/RandomVertices.txt" , time(0) );
    //Mesh(5 , "Meshdata/TriVertices.txt");
    string Nodesadress = "Meshdata/VerticesRing" + to_string(n_max) + ".txt";
    string Facesadress = "Meshdata/FacesRing" + to_string(n_max) + ".txt";
    string Edgesadress = "Meshdata/EdgesRing" + to_string(n_max) + ".txt";
    HexAreaNodes(n_max, Nodesadress);
    
    vector<array<double,2>> V = ReadVertices( Nodesadress );

    vector<array<int,3>> F = BowyerWatson(V);

    WriteFaces(F, Facesadress);

    vector<array<int,2>> E = GetAllEdges(F);

    WriteEdges(E, Edgesadress);


    /*
    2. Operator Construction
        includes:
        - construction of boundary operators/ext. deriv.,
        - (diagonal) hodges
        - combined operator for specfic problem e.g. (modified) laplace
    */
    Sparse PrimalBoundary1 = ConstructBound1( E , false );
    //WriteSparse( "Solutions/PrimalBoundary1Operator.txt" , PrimalBoundary1 , V.size() , E.size());

    Sparse MinusPrimalBoundary1 = ConstructBound1( E , true );
    //WriteSparse( "Solutions/MinusPrimalBoundary1Operator.txt" , MinusPrimalBoundary1 , V.size() , E.size() );

    Sparse PrimalBoundary2 = ConstructBound2( F , E , false );
    //WriteSparse( "Solutions/PrimalBoundary2Operator.txt" , PrimalBoundary2 , E.size() , F.size() );

    Sparse Hodge0 = DiagHodge0D( V , F );
    //WriteSparse( "Solutions/DiagHodge0Operator.txt" , Hodge0 , V.size() , V.size() );

    Sparse Hodge1 = DiagHodge1D( V , E , F );
    //WriteSparse( "Solutions/DiagHodge1Operator.txt" , Hodge1 , E.size() , E.size() );

    //modified laplace operator
    Sparse L = SparseMM( MinusPrimalBoundary1 , SparseMMT( Hodge1 , PrimalBoundary1,E.size(),E.size() ), V.size(), E.size());
    //WriteSparse( "Solutions/ModifiedLaplaceOperator.txt" , L , V.size() , V.size() );
    //normal divergence operator
    Sparse DIV = SparseInvMM( Hodge0 , SparseMM( MinusPrimalBoundary1 , Hodge1 , V.size() , E.size() ) , V.size() , E.size() );
    //WriteSparse( "Solutions/DiscreteDivergenceOperator.txt" , DIV , V.size() , E.size() );
    //modified divergence operator
    Sparse ModDiv = SparseMM( MinusPrimalBoundary1 , Hodge1 , V.size() , E.size() );
    
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
    vector<int> BoundaryNodesIndices = GetBoundaryNodesIndices( V.size() , BoundaryEdges );
    

    //loop over different fields
    
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
        
        
        vector<double> Solution = SparseVecMR( ModDiv , cochain , V.size() );
        //Doing the flux correction over the boundary of the domain
        for (int i = 0; i < BoundaryNodesIndices.size(); i++)
        {
            double HodgeCorrection = 1;//(FindEntryij( i , i , Hodge0 ));
            Solution[BoundaryNodesIndices[i]] += h_values[i]*HodgeCorrection;
        }
        
        //writing the solution to file
        string adress = "Solutions/SolRing" + to_string(n_max)+ "Field" + to_string(int(alpha));
        ofstream out{adress};
        for (int j = 0; j < Solution.size(); j++)
        {
            out << Solution[j] << endl;
        }
        //calculate error to analytical divergence on the node and write to file
        string Erradress = "Solutions/ErrorRing" + to_string(n_max)+ "Field" + to_string(int(alpha));
        ofstream ErrOut{Erradress};
        for (int j = 0; j < Solution.size(); j++)
        {
            ErrOut << Solution[j] - (alpha*pow(V[j][0]-0.5,alpha-1.)+1.) << endl;
        }
        
    }
    }
    


    /*
    4. Solving the System
        includes:
        - converting own format to alglib format
        - choose according solver function and run
    */



    /*
    5. Writing to file
    */
}