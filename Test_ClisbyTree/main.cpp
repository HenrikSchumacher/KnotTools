#include <iostream>
#include <iterator>

// see https://www.jviotti.com/2022/02/21/emitting-signposts-to-instruments-on-macos-using-cpp.html


#define dump(x) TOOLS_DUMP(x);
#define mem_dump(x) TOOLS_MEM_DUMP(x);

#include "../KnotTools.hpp"
#include "../src/PolyFold.hpp"

using namespace KnotTools;
using namespace Tensors;
using namespace Tools;

//using Real  = Real32; // Never use this unless for producing edge cases.
using Real  = Real64;
//using BReal = Real;
using BReal = Real32;
using Int   = Int64;
//using Int   = Int32;
using LInt  = Int64;

using PolyFold_T = PolyFold<Real,Int,LInt,BReal>;

int main( int argc, char** argv )
{
    
    PolyFold_T polyfold ( argc, argv );

    return 0;
}
