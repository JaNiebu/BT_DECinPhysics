#include "Integrators.h"

double precision = 10000.;
double LineIntegral( function< array<double,2>(array<double,2>,double)> field, double alpha , array<double,2> initial_point, array<double,2> final_point )
{
    double value = 0;
    array<double,2> edge = { final_point[0]-initial_point[0], final_point[1]-initial_point[1] };
    for (int i = 0; i < precision; i++)
    {
        array<double,2> position = { initial_point[0]+(i/precision)*edge[0], initial_point[1]+(i/precision)*edge[1] };
        array<double,2> FieldValue = field( position , alpha );
        value += (FieldValue[0]*edge[0] + FieldValue[1]*edge[1])/precision;
    }
    return value;
    
}

double LineFluxIntegral( function< array<double,2>(array<double,2>,double)> field , double alpha , array<double,2> initial_point, array<double,2> final_point )
{
    double value = 0;
    array<double,2> edge = { final_point[0]-initial_point[0], final_point[1]-initial_point[1] };
    array<double,2> edgenormal = { final_point[1]-initial_point[1], -final_point[0]+initial_point[0] };
    for (int i = 0; i < precision; i++)
    {
        array<double,2> position = { initial_point[0]+(i/precision)*edge[0], initial_point[1]+(i/precision)*edge[1] };

        value +=  (field( position , alpha )[0]*edgenormal[0] + field( position , alpha )[1]*edgenormal[1])/precision;
    }  
    return value;
    
}
