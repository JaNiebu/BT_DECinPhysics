#include "headers/headers.h"
#include "headers/Meshing/MeshGen.h"
#include "headers/Meshing/ReadingMesh.h"
#include "headers/Meshing/TriangulationFunc.h"

array<double,2> Field( array<double,2> position )
{
    array<double,2> Value = { position[0], position[1] };
    return Value;
}

int main()
{
    Mesh(10);
    
    vector<array<double,2>> V = ReadVertices("TriVertices.txt");

    vector<array<int,3>> F = BowyerWatson(V);

    WriteFaces(F, "Faces.txt");


    
    

}