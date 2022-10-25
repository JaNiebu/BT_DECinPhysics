
project(DECCALC)

add_executable( bin_main main.cpp 

headers/NumCalc/Integrators.cpp
headers/NumCalc/sparseMM.cpp
headers/NumCalc/sparseMMT.cpp

headers/Meshing/BoundaryElements.cpp
headers/Meshing/EdgesGen.cpp 
headers/Meshing/MeshGen.cpp 
headers/Meshing/ReadingMesh.cpp 
headers/Meshing/TriangulationFunc.cpp

headers/GeoFunc/Circumcenter.cpp
headers/GeoFunc/Signum.cpp

headers/OpConstruct/BoundConstruct.cpp
headers/OpConstruct/DiagonalHodge.cpp
)