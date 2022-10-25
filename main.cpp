#include "headers/headers.h"
//Preamble for numerical processing/calculation
#include "headers/NumCalc/sparseMM.h"
#include "headers/NumCalc/sparseMMT.h"
//Preamble for meshing process
#include "headers/Meshing/MeshGen.h"
#include "headers/Meshing/ReadingMesh.h"
#include "headers/Meshing/TriangulationFunc.h"
//Preamble for operator construction process
#include "headers/OpConstruct/BoundConstruct.h"
#include "headers/OpConstruct/DiagonalHodge.h"
#include "headers/Meshing/BoundaryElements.h"



array<double,2> Field( array<double,2> position )
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
    Mesh(10);
    
    vector<array<double,2>> V = ReadVertices("TriVertices.txt");

    vector<array<int,3>> F = BowyerWatson(V);

    WriteFaces(F, "Faces.txt");

    vector<array<int,2>> E = GetAllEdges(F);

    WriteEdges(E, "Edges.txt");


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
    Sparse L = sparseMM( MinusPrimalBoundary1 , sparseMMT( Hodge1 , PrimalBoundary1 ) );
    
    /*
    3. Problem input
        includes:
        - identifying the boundary elements (nodes/edges/faces)
        - generating and/or reading boundary conditions
        - generating and/or reading function data on interior
    */
    vector<int> BoundaryEdgesIndices = GetBoundaryEdgesIndices( E.size() , PrimalBoundary2 );


}