#include "headers/headers.h"
//Preamble for numerical processing/calculation
#include "headers/NumCalc/sparseMM.h"
#include "headers/NumCalc/sparseMMT.h"
#include "headers/NumCalc/Integrators.h"
//Preamble for meshing process
#include "headers/Meshing/MeshGen.h"
#include "headers/Meshing/ReadingMesh.h"
#include "headers/Meshing/TriangulationFunc.h"
//Preamble for operator construction process
#include "headers/OpConstruct/BoundConstruct.h"
#include "headers/OpConstruct/DiagonalHodge.h"
#include "headers/Meshing/BoundaryElements.h"



array<double,2> Field1( array<double,2> position )
{
    array<double,2> Value = { position[0]*position[0], position[1] };
    return Value;
}

array<double,2> Field2( array<double,2> position )
{
    array<double,2> Value = { position[0], position[1] };
    return Value;
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
    Mesh(10 , "Meshdata/TriVertices.txt");
    
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

    Sparse MinusPrimalBoundary1 = ConstructBound1( E , true );

    Sparse PrimalBoundary2 = ConstructBound2( F , E , false );

    Sparse Hodge0 = DiagHodge0D( V , F );

    Sparse Hodge1 = DiagHodge1D( V , E , F );

    //modified laplace operator
    Sparse L = SparseMM( MinusPrimalBoundary1 , SparseMMT( Hodge1 , PrimalBoundary1,E.size(),E.size(),V.size()), V.size(), E.size(), V.size() );

    Sparse DIV = SparseInvMM( Hodge0 , SparseMM( MinusPrimalBoundary1 , Hodge1 , V.size() , E.size() , E.size()) , V.size() , E.size() , V.size() );

    
    /*
    3. Problem input
        includes:
        - identifying the boundary elements (nodes/edges/faces)
        - generating and/or reading boundary conditions
        - generating and/or reading function data on interior
    */
    vector<int> BoundaryEdgesIndices = GetBoundaryEdgesIndices( E.size() , PrimalBoundary2 );


    vector<double> Cochain1;
    for (int i = 0; i < E.size(); i++)
    {
        array<int,2> edge = E[i];
        Cochain1.push_back( LineIntegral( &Field1, V[edge[0]] , V[edge[1]] ) );
    }
    for (int i = 0; i < Cochain1.size(); i++)
    {
        cout << Cochain1[i] << endl;
    }
    cout << LineIntegral( Field2 , array<double,2> {0.,0.} , array<double,2> {1.,0.} ) << endl;
    

    vector<double> Cochain2;
    for (int i = 0; i < E.size(); i++)
    {
        array<int,2> edge = E[i];
        Cochain2.push_back( LineIntegral(Field2, V[edge[0]] , V[edge[1]] ) );
    }
    
    vector<double> divergence1 = SparseVecMR( DIV , Cochain1 , V.size() );

    vector<double> divergence2 = SparseVecMR( DIV , Cochain2 , V.size() );


    /*
    4. Solving the System
    */



   /*
   5. Writing to file
   */
  ofstream Div1{"Solutions/DivSol1.txt"};
  for (int  i = 0; i < divergence1.size(); i++)
  {
    Div1 << divergence1[i] << endl;
  }

  ofstream Div2{"Solutions/DivSol2.txt"};
  for (int  i = 0; i < divergence2.size(); i++)
  {
    Div2 << divergence2[i] << endl;
  }
  


}