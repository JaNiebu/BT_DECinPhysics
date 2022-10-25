
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

headers/AlgLib/alglibinternal.cpp
headers/AlgLib/alglibmisc.cpp
headers/AlgLib/ap.cpp
headers/AlgLib/dataanalysis.cpp
headers/AlgLib/diffequations.cpp
headers/AlgLib/fasttransforms.cpp
headers/AlgLib/integration.cpp
headers/AlgLib/interpolation.cpp
headers/AlgLib/kernels_avx2.cpp
headers/AlgLib/kernels_fma.cpp
headers/AlgLib/kernels_sse2.cpp
headers/AlgLib/linalg.cpp
headers/AlgLib/optimization.cpp
headers/AlgLib/solvers.cpp
headers/AlgLib/specialfunctions.cpp
headers/AlgLib/statistics.cpp
)