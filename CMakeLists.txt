
project(DECCALC)

add_executable( bin_main main.cpp 
headers/Meshing/MeshGen.cpp 
headers/Meshing/ReadingMesh.cpp 
headers/Meshing/TriangulationFunc.cpp 
headers/GeoFunc/Circumcenter.cpp
)