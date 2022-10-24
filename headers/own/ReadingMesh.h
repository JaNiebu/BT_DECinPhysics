   #ifndef ReadingMesh
   #define ReadingMesh
   #include "headers.h"


    using namespace std;

    vector<array<double,2>> ReadVertices( string path );
    vector<array<int,3>> ReadFaces( string path );
    void WriteVertices( vector<array<double,2>> V );
    void WriteEdges( vector<array<int,2>> E, string filename );
    void WriteFaces( vector<array<int,3>> F, string filename );
    
    /*
    
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
    */

#endif