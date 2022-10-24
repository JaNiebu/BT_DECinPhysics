#include "headers/own/headers.h"
#include "headers/own/MeshGen.h"
#include "headers/own/ReadingMesh.h"
#include "headers/own/TriangulationFunc.h"

using namespace std;

int main()
{
    Mesh(10);
    
    vector<array<double,2>> V = ReadVertices("TriVertices.txt");

    vector<array<int,3>> F = BowyerWatson(V);

    WriteFaces(F, "Faces.txt");

}