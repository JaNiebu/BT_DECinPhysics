
project(DECCALC)

add_executable( bin_main main.cpp 

headers/NumCalc/sparseMM.cpp
headers/NumCalc/sparseMMT.cpp

headers/Meshing/MeshGen.cpp 
headers/Meshing/ReadingMesh.cpp 
headers/Meshing/TriangulationFunc.cpp
headers/Meshing/EdgesGen.cpp 

headers/GeoFunc/Circumcenter.cpp

headers/OpConstruct/BoundConstruct.cpp
headers/OpConstruct/DiagonalHodge.cpp
)