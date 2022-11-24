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
//Preamble for alglib library
#include "headers/AlgLib/solvers.h"
#include "headers/AlgLib/stdafx.h"
#include "headers/AlgLib/ap.h"
#include "headers/AlgLib/linalg.h"



array<double,2> Field( array<double,2> position , double alpha )
{
    /*double r = distance(position, array<double,2> {0.5,0.5});
    double F_x;
    double F_y;
    if ( position[0]==0.5 && position[1]==0.5 )
    {
        F_x = 10000.;
        F_y = 10000.;
    }
    else
    {   
        F_x = position[0]/(r*r);
        F_y = position[1]/(r*r);
    } 
    array<double,2> Value = { 2*M_PI*F_x, 2*M_PI*F_y };*/
    array<double,2> Value = { pow(position[0],alpha) , position[1] };
    return Value;
    
}

double SourceField( array<double,2> position )
{
    double x = position[0];
    double y = position[1];
    double Val;
    if ( x == 0.5 && y == 0.5 )
    {
        Val = 10;
    }
    else
    {
        Val = 0;
    }
    /*
    if ( distance(position, array<double,2> {.5,.5}) < 0.1 )
    {
        Val = 1;
    }
    else
    {
        Val = 0;
    }
    */
    return Val;
}


int main()
{
    vector<int> RingNumber = {1};               //different parameters for meshresolution
    vector<double> alpha_param = {3.};           //different parameters for field exponent
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

    double alpha = 2.;
    //getting all the boundary elements
    vector<int> BoundaryEdgesIndices = GetBoundaryEdgesIndices( E.size() , PrimalBoundary2 );
    vector<array<int,2>> BoundaryEdges;
    for (int i = 0; i < BoundaryEdgesIndices.size(); i++)
    {
        BoundaryEdges.push_back( E[BoundaryEdgesIndices[i]] );
    }
    vector<int> BoundaryNodesIndices = GetBoundaryNodesIndices( V.size() , BoundaryEdges );
    
    //getting the boundary condition (flux)
    vector<double> FluxCorrection = GetFluxCorrection( BoundaryNodesIndices , BoundaryEdges , V , Field , alpha );

    //discretizing the forms
    vector<double> cochain;
    for (int j = 0; j < E.size(); j++)
    {   
        array<double,2> initial_point = V[E[j][0]];
        array<double,2> final_point = V[E[j][1]];
        cochain.push_back( LineIntegral( Field , alpha , initial_point , final_point ));
    }

    /*
    4. Solving the System
        includes:
        - vector algebra in own sparse format
        - converting own format to alglib format
        - choose according solver function and run
    */
    vector<double> LhsDivTheoPointwise = SparseVecMR( ModDiv , cochain , V.size() );
    double Lhs = 0;
    for (int i = 0; i < LhsDivTheoPointwise.size(); i++)
    {
        Lhs += LhsDivTheoPointwise[i];
    }
    //Lhs = Lhs*3.*pow(3.,0.5)/8;
    
    double Rhs = 0;
    for (int i = 0; i < FluxCorrection.size(); i++)
    {
        Rhs += FluxCorrection[i];
    }
    cout << Rhs << " " << Lhs << " " << Rhs - Lhs << endl;
    



    /*
    5. Writing to file
    */
    
    
    }


}