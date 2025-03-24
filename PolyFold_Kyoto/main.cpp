#include "../Knoodle.hpp"
#include "../src/PolyFold.hpp"

using Real  = Knoodle::Real64; // scalar type used for positions of polygon
using BReal = Knoodle::Real32; // scalar type used for bounding boxes
using Int   = Knoodle::Int64;  // integer type used, e.g., for indices in PlanarDiagram etc.
using LInt  = Knoodle::Int64;  // integer type for counting objects

using PolyFold_T = Knoodle::PolyFold<Real,Int,LInt,BReal>;

int main( int argc, char** argv )
{
    PolyFold_T polyfold ( argc, argv );
    
    return 0;
}
