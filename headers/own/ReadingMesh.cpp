#include "ReadingMesh.h"


    vector<array<double,2>> ReadVertices( string path )
    {
        vector<array<double,2>> V;
        ifstream Vertices{ path };
        double x, y;
        while (Vertices >> x >> y)
        {
            array<double, 2> v = {x, y};
            V.push_back(v);
        }
        return V;
    }

    vector<array<int,3>> ReadFaces( string path )
    {
        vector<array<int, 3>> F;
        ifstream Faces{path};
        string line;
        int k;
        vector<int> f = {};
        while (Faces >> k)
        {   
            f.push_back(k);
            if ((Faces.peek() =='\n'))
            {
                F.push_back(array<int,3> {f[0],f[1],f[2]});
                f.clear();
            } 
        }
        return F;
    }

    void WriteVertices( vector<array<double,2>> V, string filename )
    {
        ofstream Vertexfile{filename};

        for (int i = 0; i < V.size(); i++)
        {
            Vertexfile << V[i][0] << " " << V[i][1] << endl;
        }
        
    }

    void WriteEdges( vector<array<int,2>> E, string filename )
    {
        ofstream Edgefile{filename};

        for (int i = 0; i < E.size(); i++)
        {
            Edgefile << E[i][0] << " " << E[i][1] << endl;
        }
        
    }

    void WriteFaces( vector<array<int,3>> F, string filename )
    {
        ofstream Facefile{filename};

        for (int i = 0; i < F.size(); i++)
        {
            Facefile << F[i][0] << " " << F[i][1] << " " << F[i][2] << endl;
        }
        
    }