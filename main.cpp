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
    double r = distance(position, array<double,2> {0.5,0.5});
    double sigma = 1.;
    double F_x;
    double F_y;
    double lambda = 1.;
    //double epsilon_0 = 8.854e-12;

    if ( position[0]==0.5 && position[1]==0.5 )
    {
        F_x = 0.;
        F_y = 0.;
        cout << "singular point" << endl;
    }
    else
    {   
        F_x = 2.*lambda*(position[0]-.5)/(sigma*pow(r,2.));
        F_y = 2.*lambda*(position[1]-.5)/(sigma*pow(r,2.));
    } 
    array<double,2> Value = { F_x, F_y };
    //array<double,2> Value = { pow(position[0],alpha) , position[1] };
    return Value;
    
}

double SourceField( array<double,2> position )
{
    double lambda = 1.;
    double x = position[0];
    double y = position[1];
    double Val;
    if ( x == 0.5 && y == 0.5 )
    {
        Val = 4.*M_PI*lambda*1250.;
    }
    else
    {
        Val = 0.;
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

array<double,2> Field2( array<double,2> position , double alpha )
{
    double r = distance(position, array<double,2> {0.5,0.5});
    double sigma = 10.;
    double R = 0.2;
    double F_x = 2.*sigma*exp(-1.*sigma*pow(r,2.))*(position[0]-.5);
    double F_y = 2.*sigma*exp(-1.*sigma*pow(r,2.))*(position[1]-.5);
    return array<double,2> {F_x , F_y};
    
    
}

double SourceField2( array<double,2> position )
{
    double r = distance(position, array<double,2> {0.5,0.5});
    double sigma = 10.;
    double Value = 4.*sigma*exp(-1.*sigma*pow(r,2.))*(1.-1.*sigma*pow(r,2.));
    return Value;
}

array<double,2> Field3( array<double,2> position , double alpha )
{
    double r = distance(position, array<double,2> {0.5,0.5});
    double sigma = 10.;
    double R = 0.2;
    double F_x = -2.*sigma*(position[0]-.5);
    double F_y = -2.*sigma*(position[1]-.5);
    return array<double,2> {F_x , F_y};
    
    
}

double SourceField3( array<double,2> position )
{
    double r = distance(position, array<double,2> {0.5,0.5});
    double sigma = 10.;
    double Value = -4.*sigma;
    return Value;
}

array<double,2> Field4( array<double,2> position , double alpha )
{
    double r = distance(position, array<double,2> {0.5,0.5});
    double sigma = 10.;
    double F_x = -3.*sigma*r*(position[0]-.5);
    double F_y = -3.*sigma*r*(position[1]-.5);
    return array<double,2> {F_x , F_y};
    
    
}

double SourceField4( array<double,2> position )
{
    double r = distance(position, array<double,2> {0.5,0.5});
    double sigma = 10.;
    double Value = -9.*sigma*r;
    return Value;
}

array<double,2> Field5( array<double,2> position , double alpha )
{
    double r = distance(position, array<double,2> {0.65,0.65});
    double F_x;
    double F_y;
    if ( r<=0.15 )
    {
        F_x = 0.;
        F_y = 0.;
    }
    
    else
    {
        F_x = 0.15*pow(r,-2.)*(position[0]-0.65);
        F_y = 0.15*pow(r,-2.)*(position[1]-0.65);
    }
    return array<double,2> {F_x, F_y};
}

double SourceField5( array<double,2> position )
{
    double r = distance(position, array<double,2> {0.65,0.65});
    double sigma = 1/(2.*M_PI);
    double Value;
    if ( r<=0.15 )
    {
        Value = sigma;
    }
    else
    {
        Value = 0.;
    }
    return Value;

}

array<double,2> Field6( array<double,2> position , double alpha )
{
    double r = distance(position, array<double,2> {0.7,0.7});
    double sigma = 10.;
    double R = 0.2;
    double F_x = 2.*sigma*exp(-1.*sigma*pow(r,2.))*(position[0]-.7);
    double F_y = 2.*sigma*exp(-1.*sigma*pow(r,2.))*(position[1]-.7);
    return array<double,2> {F_x , F_y};
    
    
}

double SourceField6( array<double,2> position )
{
    double r = distance(position, array<double,2> {0.7,0.7});
    double sigma = 10.;
    double Value = 4.*sigma*exp(-1.*sigma*pow(r,2.))*(1.-1.*sigma*pow(r,2.));
    return Value;
}

int main()
{
    vector<int> RingNumber = {8};                       //different parameters for meshresolution
    vector<double> alpha_param = {3.};                  //different parameters for field exponent
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

    //Full Laplace
    Sparse Laplace = SparseInvMM( Hodge0 , SparseMM( MinusPrimalBoundary1 , SparseMMT( Hodge1 , PrimalBoundary1,E.size(),E.size() ), V.size(), E.size() ) , V.size() , V.size() );
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
    vector<double> FluxCorrection = GetFluxCorrection( BoundaryNodesIndices , BoundaryEdges , V , Field6 , alpha );
    
    
    //discretizing the forms
    /*vector<double> cochain;
    for (int j = 0; j < E.size(); j++)
    {   
        array<double,2> initial_point = V[E[j][0]];
        array<double,2> final_point = V[E[j][1]];
        cochain.push_back( LineIntegral( Field , alpha , initial_point , final_point ));
        if ( j==0 )
        {
        cout << "initial point " << initial_point[0] << " " << initial_point[1] << endl;
        cout << "final point " << final_point[0] << " " << final_point[1] << endl;
        cout << "integral value " << cochain[j] << endl;
        }
    }*/
    vector<double> ModifiedSourceTerm;
    for (int j = 0; j < V.size(); j++)
    {
        ModifiedSourceTerm.push_back( FindEntryij(j,j,Hodge0)*SourceField6(V[j]) );
    }
    for (int j = 0; j < FluxCorrection.size(); j++)
    {
        ModifiedSourceTerm[BoundaryNodesIndices[j]] += -1.*FluxCorrection[j];
    }
    
    

    //discretizing the normal flux
    vector<double> StarCochain;
    for (int j = 0; j < E.size(); j++)
    {
        array<double,2> initial_point = V[E[j][0]];
        array<double,2> final_point = V[E[j][1]];
        StarCochain.push_back( LineFluxIntegral( Field6 , alpha , initial_point , final_point ));
    }
    

    /*
    4. Solving the System
        includes:
        - vector algebra in own sparse format
        - converting own format to alglib format
        - choose according solver function and run
    */
    //Poisson problem -- dirichlet
    alglib::sparsematrix AlgLibL;
    int NumberOfNodes = V.size();
    alglib::sparsecreate(V.size(),V.size(), AlgLibL) ;
    for (int i = 0; i < L.column.size(); i++)
    {
        alglib::sparseset( AlgLibL , L.rows[i] , L.column[i] , -1.*L.values[i] );
    }
    alglib::sparseconverttocrs(AlgLibL);

    alglib::real_1d_array b;
    b.setlength(V.size());
    for (int i = 0; i < ModifiedSourceTerm.size(); i++)
    {
        b(i) = ModifiedSourceTerm[i];
    }
   
    alglib::linlsqrstate s;
    alglib::linlsqrreport rep;
    alglib::real_1d_array x;
    alglib::linlsqrcreate(V.size(), V.size(), s);
    alglib::linlsqrsolvesparse(s, AlgLibL, b);
    alglib::linlsqrresults(s, x, rep);

    printf("%d\n", int(rep.terminationtype));

    ofstream Poisson{"Solutions/PoissonAsymSolutionRing"+to_string(n_max)+".txt"};
    for (int i = 0; i < V.size(); i++)
    {
        Poisson << x(i) << endl;
    }
    /*
    * Rep.TerminationType completetion code:
                    *  1    ||Rk||<=EpsB*||B||
                    *  4    ||A^T*Rk||/(||A||*||Rk||)<=EpsA
                    *  5    MaxIts steps was taken
                    *  7    rounding errors prevent further progress,
                            X contains best point found so far.
                            (sometimes returned on singular systems)
                    *  8    user requested termination via calling
                            linlsqrrequesttermination()
    */
  



    /*vector<double> Complex;
    Complex.push_back(1.);
    for (int  i= 1; i < V.size(); i++)
    {
        Complex.push_back(0.);
    }
    
    vector<double> A = SparseVecMR(SparseMM(Hodge1,MinusPrimalBoundary1,E.size(),V.size()), Complex, V.size()); 
    double Val = 0;
    for (int i = 0; i < A.size(); i++)
    {
        Val += A[i]*cochain[i];
    }
    cout << Val << endl;
    vector<double> LhsDivTheoPointwise = SparseVecMR( ModDiv , cochain , V.size() );
    double Lhs = 0;
    for (int i = 0; i < LhsDivTheoPointwise.size(); i++)
    {
        Lhs += LhsDivTheoPointwise[i];
    }
    //Lhs = Lhs*3.*pow(3.,0.5)/8;
    vector<double> StarF = SparseVecMR( Hodge1 , cochain , E.size() );
    vector<double> RhsDivTheoPointwise;
    for (int i = 0; i < V.size(); i++)
    {
        //get i-th row of Bound1 <=> i-th column of d^0
        vector<double> Row_i = GetRow( i , PrimalBoundary1 , E.size() );
        double Entry_i;
        for (int j = 0; j < E.size(); j++)
        {
            if ( Row_i[j]!=0 )
            {
                Entry_i += Row_i[j]*StarF[j];
            }
            
        }
        RhsDivTheoPointwise.push_back( Entry_i );

    }*/
    
    /*for (int i = 0; i < BoundaryNodesIndices.size(); i++)
    {
        int Index = BoundaryNodesIndices[i];
        RhsDivTheoPointwise[Index] += FluxCorrection[i];
    }*/
    

    /*vector<double> NewLhs = SparseVecMR( MinusPrimalBoundary1 , StarCochain , V.size() );
    vector<double> NewRhs;
    for (int i = 0; i < V.size(); i++)
    {
        //get i-th row of Bound1 <=> i-th column of d^0
        vector<double> Row_i = GetRow( i , PrimalBoundary1 , E.size() );
        double Entry_i;
        for (int j = 0; j < E.size(); j++)
        {
            if ( Row_i[j]!=0 )
            {
                Entry_i += Row_i[j]*StarCochain[j];
            }
            
        }
        NewRhs.push_back( Entry_i );

    }*/
    

    /*double Rhs = 0;
    for (int i = 0; i < FluxCorrection.size(); i++)
    {
        Rhs += FluxCorrection[i];
    }
    cout << Rhs << " " << Lhs << " " << Rhs - Lhs << endl;
    */



    /*
    5. Writing to file
    */
    /*ofstream DivTheoErr{"Solutions/TheoremError.txt"};
    for (int i = 0; i < RhsDivTheoPointwise.size(); i++)
    {
        DivTheoErr << RhsDivTheoPointwise[i]-LhsDivTheoPointwise[i] << endl;
    }
    ofstream NewErr{"Solutions/NewTheoremError.txt"};
    for (int i = 0; i < NewRhs.size(); i++)
    {
        NewErr << NewRhs[i]-NewLhs[i] << endl;
    }*/
    
    cout << n+1 << " of " << RingNumber.size() << endl;
    }


}