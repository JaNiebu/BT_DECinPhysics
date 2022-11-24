#ifndef MeshGen
#define MeshGen
#include "../headers.h"
#include "../GeoFunc/Circumcenter.h"

void RandomNodes(int Nodesnumber , string adress, int seed );

void HexAreaNodes(int n_max , string adress );

void Mesh(int n , string adress);

#endif