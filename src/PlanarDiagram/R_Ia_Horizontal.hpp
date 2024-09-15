private:

bool Reidemeister_Ia_Horizontal( const Int c_0, const Int c_1, const bool side )
{
    PD_PRINT("\nReidemeister_Ia_Horizontal  ( "+CrossingString(c_0)+", "+CrossingString(c_1)+" )");
    
    // c_0 == c_1  should be made impossible by the way we call this function.
    PD_ASSERT( c_0 != c_1 );
    
    auto C_0 = Crossing( c_0 );
    auto C_1 = Crossing( c_1 );
    
    // This are some central assumption here. (C_0 is the bottom crossing.)
    PD_ASSERT( SameHandednessQ(C_0,C_1) );
    PD_ASSERT( C_0(Out,side) == C_1(In ,side) );
    PD_ASSERT( C_0(In ,side) == C_1(Out,side) );
    
// We assume this horizontal alignment in the case of side==Right.
// Both crossing's handedness can also be opposite (at the same time!).
//
//                  C_0(Out,side) = B = C_1(In ,side)
//
//           O----<----O       O---->----O       O----<----O
//               E_3    ^     ^     B     \     /    E_2
//                       \   /             \   /
//                        \ /               \ /
//                     C_0 /                 / C_1
//                        / \               / \
//                       /   \             /   \
//               E_0    /     \     A     v     v    E_1
//           O---->----O       O----<----O       O---->----O
//
//                  C_0(In ,side) = A = C_0(Out,side)
//
// In the case side == Left, we just flip everything around.
    
    
    // Since the two-crossing configuration of c_0 and c_1 is not destroyed, we can test both sides.
    const bool outQ = Reidemeister_Ia_Horizontal_impl(C_0,C_1,Out,side);
    
    R_Ia_counter += outQ;
    
    const bool inQ = Reidemeister_Ia_Horizontal_impl(C_0,C_1,In,side);
    
    R_Ia_counter += inQ;
    
    return (outQ || inQ);
    
} // Reidemeister_Ia_Horizontal


bool Reidemeister_Ia_Horizontal_impl( CrossingView & C_0, CrossingView & C_1, const bool io, const bool side )
{
    auto E_0 = Arc( C_0( io,!side) );
    auto E_1 = Arc( C_1(!io,!side) );
    
    PD_ASSERT( E_0( io == Out ? Tail : Head ) == C_0 );
    PD_ASSERT( E_1( io == Out ? Head : Tail ) == C_1 );

    auto C_2 = Crossing( E_0( io == Out ? Head : Tail ) );
    auto C_3 = Crossing( E_1( io == Out ? Tail : Head ) );
    
    if( C_2 == C_3 )
    {
        PD_ASSERT( C_0 != C_2 );
        PD_ASSERT( C_1 != C_2 );
        
        if( C_2(!io, side) == E_0 )
        {
            if( SameHandednessQ(C_0,C_2) )
            {
                PD_PRINT("Reidemeister_Ia_Horizontal_impl - normal move!");
                
                PD_VALPRINT( "io", io == In ? "In" : "Out" );
                PD_VALPRINT( "side", side == Left ? "Left" : "Right" );

                
// We have this horizontal alignment in the case of io = Out and side == Right.
// (In the case side == Left or io == In, we just have to reflect everything accordingly.)
// All crossing's handedness can also be switched (at the same time!).
//
//                         F_0  O       O  F_1
//                               \     ^
//                                \   /
//                                 \ /
//                                  \ C_2
//                                 / \
//                                /   \
//                               /     v
//                              O       O
//                             ^         \
//                            /           \
//                           /             \
//                          /               \
//                    E_0  /                 \  E_1
//                        /                   \
//                       /                     \
//                      /                       v
//                     O       O---->----O       O
//                      ^     ^     B     \     /
//                       \   /             \   /
//                        \ /               \ /
//                     C_0 /                 / C_1
//                        / \               / \
//                       /   \             /   \
//                      /     \     A     v     v
//           O---->----O       O----<----O       O---->----O
//
                
//                PD_ASSERT( io   == Out   );
//                PD_ASSERT( side == Right );
                
                auto A   = Arc( C_0(!io, side) );
                auto F_0 = Arc( C_2(!io,!side) );
                auto F_1 = Arc( C_2( io,!side) );
                
                
                PD_PRINT("incoming data");
                PD_VALPRINT("C_0",C_0);
                PD_VALPRINT("C_1",C_1);
                PD_VALPRINT("C_2",C_2);

                PD_VALPRINT("E_0",E_0);
                PD_VALPRINT("E_1",E_1);

                PD_VALPRINT("F_0",F_0);
                PD_VALPRINT("F_1",F_1);

                PD_VALPRINT("A",A);
                
                PD_ASSERT( C_0 != C_1 );
                PD_ASSERT( C_0 != C_2 );
                PD_ASSERT( C_1 != C_2 );
                PD_ASSERT( SameHandednessQ(C_0,C_1) );
                PD_ASSERT( SameHandednessQ(C_0,C_2) );
                PD_ASSERT( SameHandednessQ(C_1,C_2) );
                
                PD_ASSERT( E_0 != E_1 );
                PD_ASSERT( F_0 != F_1 );
                PD_ASSERT( F_0 != E_0 );
                PD_ASSERT( F_0 != E_1 );
                PD_ASSERT( F_1 != E_0 );
                PD_ASSERT( F_1 != E_1 );

                
//                PD_ASSERT(A_cross(e_0,Head) == c_2); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(e_0,Tail) == c_0); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(e_1,Head) == c_1); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(e_1,Tail) == c_2); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(f_0,Head) == c_2); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(f_1,Tail) == c_2); // Only for io == Out and side == Right.
//                
//                PD_ASSERT(A_cross(a  ,Head) == c_0); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(a  ,Tail) == c_1); // Only for io == Out and side == Right.
//
//                PD_ASSERT(C_arcs(c_2,In ,Right) == e_0 ); // Only for io == Out and side == Right.
//                PD_ASSERT(C_arcs(c_2,Out,Right) == e_1 ); // Only for io == Out and side == Right.
//                PD_ASSERT(C_arcs(c_2,In ,Left ) == f_0 ); // Only for io == Out and side == Right.
//                PD_ASSERT(C_arcs(c_2,Out,Left ) == f_1 ); // Only for io == Out and side == Right.

//
// In the case side == Left or io == In, we just have to reflect everything accordingly.
//
//
// Change to this:
//
//                              |       ^
//                              |       |
//                              |       |
//                              |       |
//                              |       |
//                              |       |
//                              v       |
//                              +       O
//                             /         ^
//                            /           \
//                           /             \
//                          /               \
//                    F_0  /                 \  F_1
//                        /                   \
//                       /                     \
//                      v                       \
//                     O       O---->----O       O
//                      \     ^     B     \     ^
//                       \   /             \   /
//                        \ /               \ /
//                     C_0 \                 \ C_1
//                        / \               / \
//                       /   \             /   \
//                      /     v     A     /     v
//           O---->----O       O---->----O       O---->----O
                
                
                // Reverse a.
                std::swap( A(Tail), A(Head) );
                
                // Reconnect arc f_0 (the Reconnect routine does not work!)
                PD_ASSERT( F_0( io == Out ? Head : Tail ) == C_2 );
                F_0(io == Out ? Head : Tail) = C_0.Idx();
                DeactivateArc(E_0.Idx());
                
                // Reconnect arc F_1 (the Reconnect routine does not work!)
                PD_ASSERT( F_1( io == Out ? Tail : Head) == C_2 );
                F_1( io == Out ? Tail : Head ) = C_1.Idx();
                DeactivateArc(E_1.Idx());
                
                // Modify crossing C_0.
                C_0( io,!side) = F_0.Idx();
                RotateCrossing(C_0,io == Out ? side : !side); // ??
                
                // Modify crossing C_1.
                C_1(!io,!side) = F_1.Idx();
                RotateCrossing(C_1,io == Out ? !side : side); // ??
                
                // Finally, we can remove the crossing.
                DeactivateCrossing(C_2.Idx());
                
                PD_PRINT("changed data");
                PD_VALPRINT("C_0",C_0);
                PD_VALPRINT("C_1",C_1);
                PD_VALPRINT("F_0",F_0);
                PD_VALPRINT("F_1",F_1);
                PD_VALPRINT("A  ",A  );
                
                PD_ASSERT( C_0.ActiveQ() );
                PD_ASSERT( C_1.ActiveQ() );
                PD_ASSERT(!C_2.ActiveQ() );
                
                PD_ASSERT(!E_0.ActiveQ() );
                PD_ASSERT(!E_1.ActiveQ() );
                PD_ASSERT( F_0.ActiveQ() );
                PD_ASSERT( F_1.ActiveQ() );
                PD_ASSERT( A.ActiveQ() );
                
                PD_ASSERT( CheckCrossing(C_0.Idx()) );
                PD_ASSERT( CheckCrossing(C_1.Idx()) );
                PD_ASSERT( CheckArc(F_0.Idx()) );
                PD_ASSERT( CheckArc(F_1.Idx()) );
                PD_ASSERT( CheckArc(A.Idx()) );
                

//                PD_ASSERT(A_cross(f_0,Head) == c_0); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(f_1,Tail) == c_1); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(a  ,Head) == c_1); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(a  ,Tail) == c_0); // Only for io == Out and side == Right.
                
                PD_PRINT("\nReidemeister_Ia_Horizontal_impl  ( "+ToString(C_0)+", "+ToString(C_1)+"," + ToString(io) + "," + ToString(side) + " )");
                
                ++R_Ia_horizontal_counter;
                
                return true;
            }
            else
            {

// We have this horizontal alignment in the case of io = Out and side == Right.
// (In the case side == Left or io == In, we just have to reflect everything accordingly.)
// All crossing's handedness can also be switched (at the same time!).
//
//                         F_0  O       O  F_1
//                               \     ^
//                                \   /
//                                 \ /
//                                  / C_2
//                                 / \
//                                /   \
//                               /     v
//                              O       O
//                             ^         \
//                            /           \
//                           /             \
//                          /               \
//                    E_0  /                 \  E_1
//                        /                   \
//                       /                     \
//                      /                       v
//                     O       O---->----O       O
//                      ^     ^     B     \     /
//                       \   /             \   /
//                        \ /               \ /
//                     C_0 /                 / C_1
//                        / \               / \
//                       /   \             /   \
//                      /     \     A     v     v
//           O---->----O       O----<----O       O---->----O

// So we cannot do anything meaningful.
                
            }
        }
        else
        {
            // This is a very seldom (impossible?) case.
            // TODO: Actually, this is an indicator for a connect sum.
            
            PD_PRINT("Reidemeister_Ia_Horizontal_impl - twist move!");

            PD_VALPRINT( "io", io == In ? "In" : "Out" );
            PD_VALPRINT( "side", side == Left ? "Left" : "Right" );
            
            
//            ArcView F_0;
//            ArcView F_1;
//
//            if( io == Out )
//            {
//                if( side == Left )
//                {
//// We have this horizontal alignment in the case of io = Out and side == Left.
//// All crossing's handedness can also be switched (at the same time!).
////
////                              O       O
////                             / ^     / ^
////                            /   \   /   \
////                           /     \ /     \
////                          /       X C_2   \
////                    E_1  /       / \       \  E_0
////                        /       /   \       \
////                       /       v     \       \
////                      v       O       O       \
////                     +        |       ^        +
////                     |    F_0 v       | F_1    ^
////                     |      +-----------+      |
////                     |      |  Tangle   |      |
////                     |      +-----------+      |
////                     v                         |
////                     O       O----<----O       O
////                      \     /           ^     ^
////                       \   /             \   /
////                        \ /               \ /
////                     C_1 /                 / C_0
////                        / \               / \
////                       /   \             /   \
////                      v     v           /     \
////           O----<----O       O---->----O       O----<----O
////
//                    F_0 = Arc( C_2( Out, Left ) );
//                    F_1 = Arc( C_2( In , Left ) );
//                }
//                else // if( side == Right )
//                {
//// We have this horizontal alignment in the case of io = Out and side == Right.
//// (In the case side == Left or io == In, we just have to reflect everything accordingly.)
//// All crossing's handedness can also be switched (at the same time!).
////
////                              O       O
////                             ^ \     ^ \
////                            /   \   /   \
////                           /     \ /     \
////                          /       X C_2   \
////                    E_0  /       / \       \  E_1
////                        /       /   \       \
////                       /       /     v       \
////                      /       O       O       v
////                     +        ^       |        +
////                     ^    F_1 |       v F_0    |
////                     |      +-----------+      |
////                     |      |  Tangle   |      |
////                     |      +-----------+      |
////                     |                         v
////                     O       O---->----O       O
////                      ^     ^           \     /
////                       \   /             \   /
////                        \ /               \ /
////                     C_0 /                 / C_1
////                        / \               / \
////                       /   \             /   \
////                      /     \           v     v
////           O---->----O       O----<----O       O---->----O
////
////
//                    F_0 = Arc( C_2( Out, Right) );
//                    F_1 = Arc( C_2( In , Right) );
//                }
//            }
//            else // if( io == In )
//            {
//                if( side == Left )
//                {
//// We have this horizontal alignment in the case of io = In and side == Left.
//// All crossing's handedness can also be switched (at the same time!).
////
////           O---->----O       O----<----O       O---->----O
////                      \     /           ^     ^
////                       \   /             \   /
////                        \ /               \ /
////                     C_1 /                 / C_0
////                        / \               / \
////                       /   \             /   \
////                      v     v           /     \
////                     O       O---->----O       O
////                     |                         ^
////                     |      +-----------+      |
////                     |      |  Tangle   |      |
////                     |      +-----------+      |
////                     v    F_0 |       ^ F_1    |
////                     +        v       |        +
////                      \       O       O       ^
////                       \       \     ^       /
////                        \       \   /       /
////                    E_1  \       \ /       /  E_0
////                          \       X C_2   /
////                           \     / \     /
////                            \   /   \   /
////                             v /     v /
////                              O       O
//                    
//                    F_0 = Arc( C_2( In , Left ) );
//                    F_1 = Arc( C_2( Out, Left ) );
//                }
//                else // if( side == Right )
//                {
//// We have this horizontal alignment in the case of io = In and side == Right.
//// All crossing's handedness can also be switched (at the same time!).
////
////           O---->----O       O---->----O       O---->----O
////                      ^     ^           \     /
////                       \   /             \   /
////                        \ /               \ /
////                     C_0 /                 / C_1
////                        / \               / \
////                       /   \             /   \
////                      /     \           v     v
////                     O       O----<----O       O
////                     ^                         |
////                     |      +-----------+      |
////                     |      |  Tangle   |      |
////                     |      +-----------+      |
////                     |    F_1 ^       | F_0    v
////                     +        |       v        +
////                      ^       O       O       /
////                       \       ^     /       /
////                        \       \   /       /
////                    E_0  \       \ /       /  E_1
////                          \       X C_2   /
////                           \     / \     /
////                            \   /   \   /
////                             \ v     \ v
////                              O       O
//                    
//                    F_0 = Arc( C_2( In , Right) );
//                    F_1 = Arc( C_2( Out, Right) );
//                }
//            }
            
            auto F_0 = Arc( C_2( io, side ) );
            auto F_1 = Arc( C_2(!io, side ) );

            PD_ASSERT( E_0 != F_1 );
            PD_ASSERT( E_0 != F_0 );
            PD_ASSERT( E_1 != F_1 );
            PD_ASSERT( E_1 != F_0 );

            PD_PRINT("Incoming data.");
            
            PD_VALPRINT("C_0",C_0);
            PD_VALPRINT("C_1",C_1);
            PD_VALPRINT("C_2",C_2);
            
            PD_VALPRINT("E_0",E_0);
            PD_VALPRINT("E_1",E_1);
            PD_VALPRINT("F_0",F_0);
            PD_VALPRINT("F_0",F_0);

            // DEBUGGING
            wprint(ClassName()+"::Reidemeister_Ia_Horizontal_impl - twist move!");
            //TODO: Test this.
                
            PD_ASSERT( F_0( io == Out ? Tail : Head ) == C_2 );
            PD_ASSERT( F_1( io == Out ? Head : Tail ) == C_2 );

            Reconnect( F_0, io == Out ? Tail : Head, E_0 );
            Reconnect( F_1, io == Out ? Head : Tail, E_1 );
            DeactivateCrossing(C_2.Idx());
            
            ++R_Ia_horizontal_counter;
            
            PD_PRINT("Changed data.");
            
            PD_VALPRINT("C_0",C_0);
            PD_VALPRINT("C_1",C_1);
            PD_VALPRINT("F_0",F_0);
            PD_VALPRINT("F_1",F_1);

            PD_PRINT("Horizontal reroute checks.");
            
            PD_ASSERT( C_0.ActiveQ() );
            PD_ASSERT( C_1.ActiveQ() );
            PD_ASSERT(!C_2.ActiveQ() );
            
            PD_ASSERT( CheckCrossing(C_0.Idx()) );
            PD_ASSERT( CheckCrossing(C_1.Idx()) );

            
            PD_ASSERT(!E_0.ActiveQ() );
            PD_ASSERT(!E_1.ActiveQ() );
            PD_ASSERT( F_0.ActiveQ() );
            PD_ASSERT( F_1.ActiveQ() );
            PD_ASSERT( CheckArc(F_0.Idx()) );
            PD_ASSERT( CheckArc(F_1.Idx()) );
            

            PD_PRINT("Horizontal reroute checks done.");
            
            return true;
            
//            wprint(ClassName()+"::Reidemeister_Ia_Horizontal_impl: Twist move possible.");
//
//            return false;
        }
    }
    
    return false;
}




bool Reidemeister_Ia_Horizontal_B( const Int c_0, const Int c_1, const bool side )
{
    PD_PRINT("\nReidemeister_Ia_Horizontal  ( "+CrossingString(c_0)+", "+CrossingString(c_1)+" )");
    
    // c_0 == c_1  should be made impossible by the way we call this function.
    PD_ASSERT( c_0 != c_1 );
    
    // This are some central assumption here. (c_0 is the bottom crossing.)
    PD_ASSERT( SameHandednessQ(c_0,c_1) );
    PD_ASSERT( C_arcs(c_0,Out,side) == C_arcs(c_1,In ,side) );
    PD_ASSERT( C_arcs(c_0,In ,side) == C_arcs(c_1,Out,side) );
    
// We assume this horizontal alignment in the case of side==Right.
// Both crossing's handedness can also be opposite (at the same time!).
//
//           C_arcs(c_0,Out,side) = b = C_arcs(c_1,In ,side)
//
//           O----<----O       O---->----O       O----<----O
//               e_3    ^     ^     b     \     /    e_2
//                       \   /             \   /
//                        \ /               \ /
//                     c_0 /                 / c_1
//                        / \               / \
//                       /   \             /   \
//               e_0    /     \     a     v     v    e_1
//           O---->----O       O----<----O       O---->----O
//
//           C_arcs(c_0,In ,side) = a = C_arcs(c_0,Out,side)
//
// In the case side == Left, we just flip everything around.
    
    
    // Since the two-crossing configuration of c_0 and c_1 is not destroyed, we can test both sides.
    const bool outQ = Reidemeister_Ia_Horizontal_impl(c_0,c_1,Out,side);
    
    R_Ia_counter += outQ;
    
    const bool inQ = Reidemeister_Ia_Horizontal_impl(c_0,c_1,In,side);
    
    R_Ia_counter += inQ;
    
    return (outQ || inQ);
    
} // Reidemeister_Ia_Horizontal


bool Reidemeister_Ia_Horizontal_impl_B( const Int c_0, const Int c_1, const bool io, const bool side )
{
    const Int e_0 = C_arcs( c_0,  io, !side );
    const Int e_1 = C_arcs( c_1, !io, !side );
        
    PD_ASSERT( A_cross( e_0, io == Out ? Tail : Head ) == c_0 );
    PD_ASSERT( A_cross( e_1, io == Out ? Head : Tail ) == c_1 );

    const Int c_2 = A_cross( e_0, io == Out ? Head : Tail );
    const Int c_3 = A_cross( e_1, io == Out ? Tail : Head );
    
    if( c_2 == c_3 )
    {
        PD_ASSERT( CrossingActiveQ(c_2) );
        
        PD_ASSERT( c_0 != c_2 );
        PD_ASSERT( c_1 != c_2 );
        
        if( C_arcs( c_2, !io, side ) == e_0 )
        {
            if( SameHandednessQ(c_0,c_2) )
            {
                PD_PRINT("Reidemeister_Ia_Horizontal_impl - normal move!");
                
                PD_VALPRINT( "io", io == In ? "In" : "Out" );
                PD_VALPRINT( "side", side == Left ? "Left" : "Right" );

                
// We have this horizontal alignment in the case of io = Out and side == Right.
// (In the case side == Left or io == In, we just have to reflect everything accordingly.)
// All crossing's handedness can also be switched (at the same time!).
//
//                         f_0  O       O  f_1
//                               \     ^
//                                \   /
//                                 \ /
//                                  \ c_2
//                                 / \
//                                /   \
//                               /     v
//                              O       O
//                             ^         \
//                            /           \
//                           /             \
//                          /               \
//                    e_0  /                 \  e_1
//                        /                   \
//                       /                     \
//                      /                       v
//                     O       O---->----O       O
//                      ^     ^     b     \     /
//                       \   /             \   /
//                        \ /               \ /
//                     c_0 /                 / c_1
//                        / \               / \
//                       /   \             /   \
//                      /     \     a     v     v
//           O---->----O       O----<----O       O---->----O
//
                
//                PD_ASSERT( io   == Out   );
//                PD_ASSERT( side == Right );
                
                const Int a   = C_arcs(c_0,!io,side);
                const Int f_0 = C_arcs(c_2,!io,!side);
                const Int f_1 = C_arcs(c_2, io,!side);
                
                
                PD_PRINT("incoming data");
                PD_VALPRINT("c_0",CrossingString(c_0));
                PD_VALPRINT("c_1",CrossingString(c_1));
                PD_VALPRINT("c_2",CrossingString(c_2));

                PD_VALPRINT("e_0",ArcString(e_0));
                PD_VALPRINT("e_1",ArcString(e_1));

                PD_VALPRINT("f_0",ArcString(f_0));
                PD_VALPRINT("f_1",ArcString(f_1));

                PD_VALPRINT("a",ArcString(a));
                
                PD_ASSERT( c_0 != c_1 );
                PD_ASSERT( c_0 != c_2 );
                PD_ASSERT( c_1 != c_2 );
                PD_ASSERT( SameHandednessQ(c_0,c_1) );
                PD_ASSERT( SameHandednessQ(c_0,c_2) );
                PD_ASSERT( SameHandednessQ(c_1,c_2) );
                
                PD_ASSERT( CrossingActiveQ(c_0) );
                PD_ASSERT( CrossingActiveQ(c_1) );
                PD_ASSERT( CrossingActiveQ(c_2) );
                PD_ASSERT( ArcActiveQ(a  ) );
                PD_ASSERT( ArcActiveQ(e_0) );
                PD_ASSERT( ArcActiveQ(e_1) );
                PD_ASSERT( ArcActiveQ(f_0) );
                PD_ASSERT( ArcActiveQ(f_1) );
                
                PD_ASSERT( e_0 != e_1 );
                PD_ASSERT( f_0 != f_1 );
                PD_ASSERT( f_0 != e_0 );
                PD_ASSERT( f_0 != e_1 );
                PD_ASSERT( f_1 != e_0 );
                PD_ASSERT( f_1 != e_1 );
                
                PD_ASSERT( CheckCrossing(c_0) );
                PD_ASSERT( CheckCrossing(c_1) );
                PD_ASSERT( CheckCrossing(c_2) );
                PD_ASSERT( CheckArc(a  ) );
                PD_ASSERT( CheckArc(e_0) );
                PD_ASSERT( CheckArc(e_1) );
                PD_ASSERT( CheckArc(f_0) );
                PD_ASSERT( CheckArc(f_1) );

                
//                PD_ASSERT(A_cross(e_0,Head) == c_2); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(e_0,Tail) == c_0); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(e_1,Head) == c_1); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(e_1,Tail) == c_2); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(f_0,Head) == c_2); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(f_1,Tail) == c_2); // Only for io == Out and side == Right.
//
//                PD_ASSERT(A_cross(a  ,Head) == c_0); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(a  ,Tail) == c_1); // Only for io == Out and side == Right.
//
//                PD_ASSERT(C_arcs(c_2,In ,Right) == e_0 ); // Only for io == Out and side == Right.
//                PD_ASSERT(C_arcs(c_2,Out,Right) == e_1 ); // Only for io == Out and side == Right.
//                PD_ASSERT(C_arcs(c_2,In ,Left ) == f_0 ); // Only for io == Out and side == Right.
//                PD_ASSERT(C_arcs(c_2,Out,Left ) == f_1 ); // Only for io == Out and side == Right.

//
// In the case side == Left or io == In, we just have to reflect everything accordingly.
//
//
// Change to this:
//
//                              |       ^
//                              |       |
//                              |       |
//                              |       |
//                              |       |
//                              |       |
//                              v       |
//                              +       O
//                             /         ^
//                            /           \
//                           /             \
//                          /               \
//                    f_0  /                 \  f_1
//                        /                   \
//                       /                     \
//                      v                       \
//                     O       O---->----O       O
//                      \     ^     b     \     ^
//                       \   /             \   /
//                        \ /               \ /
//                     c_0 \                 \ c_1
//                        / \               / \
//                       /   \             /   \
//                      /     v     a     /     v
//           O---->----O       O---->----O       O---->----O
                
                
                // Reverse a.
                std::swap( A_cross(a,Tail), A_cross(a,Head) );
                
                // Reconnect arc f_0 (the Reconnect routine does not work!)
                PD_ASSERT(A_cross(f_0, io == Out ? Head : Tail ) == c_2);
                A_cross(f_0,io == Out ? Head : Tail) = c_0;
                DeactivateArc(e_0);
                
                // Reconnect arc f_1 (the Reconnect routine does not work!)
                PD_ASSERT(A_cross(f_1, io == Out ? Tail : Head) == c_2);
                A_cross(f_1, io == Out ? Tail : Head ) = c_1;
                DeactivateArc(e_1);
                
                // Modify crossing c_0.
                C_arcs(c_0, io,!side) = f_0;
                RotateCrossing(c_0,io == Out ? side : !side); // ??
                
                // Modify crossing c_1.
                C_arcs(c_1,!io,!side) = f_1;
                RotateCrossing(c_1,io == Out ? !side : side); // ??
                
                // Finally, we can remove the crossing.
                DeactivateCrossing(c_2);
                
                PD_PRINT("changed data");
                PD_VALPRINT("c_0",CrossingString(c_0));
                PD_VALPRINT("c_1",CrossingString(c_1));
                PD_VALPRINT("f_0",ArcString(f_0));
                PD_VALPRINT("f_1",ArcString(f_1));
                PD_VALPRINT("a  ",ArcString(a));
                
                PD_ASSERT( CrossingActiveQ(c_0) );
                PD_ASSERT( CrossingActiveQ(c_1) );
                PD_ASSERT(!CrossingActiveQ(c_2) );
                
                PD_ASSERT(!ArcActiveQ(e_0) );
                PD_ASSERT(!ArcActiveQ(e_1) );
                PD_ASSERT( ArcActiveQ(f_0) );
                PD_ASSERT( ArcActiveQ(f_1) );
                PD_ASSERT( ArcActiveQ(a  ) );
                
                PD_ASSERT( CheckCrossing(c_0) );
                PD_ASSERT( CheckCrossing(c_1) );
                PD_ASSERT( CheckArc(f_0) );
                PD_ASSERT( CheckArc(f_1) );
                PD_ASSERT( CheckArc(a  ) );
                

//                PD_ASSERT(A_cross(f_0,Head) == c_0); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(f_1,Tail) == c_1); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(a  ,Head) == c_1); // Only for io == Out and side == Right.
//                PD_ASSERT(A_cross(a  ,Tail) == c_0); // Only for io == Out and side == Right.
                
                PD_PRINT("\nReidemeister_Ia_Horizontal_impl  ( "+CrossingString(c_0)+", "+CrossingString(c_1)+"," + ToString(io) + "," + ToString(side) + " )");
                
                ++R_Ia_horizontal_counter;
                
                return true;
            }
            else
            {

// We have this horizontal alignment in the case of io = Out and side == Right.
// (In the case side == Left or io == In, we just have to reflect everything accordingly.)
// All crossing's handedness can also be switched (at the same time!).
//
//                         f_0  O       O  f_1
//                               \     ^
//                                \   /
//                                 \ /
//                                  / c_2
//                                 / \
//                                /   \
//                               /     v
//                              O       O
//                             ^         \
//                            /           \
//                           /             \
//                          /               \
//                    e_0  /                 \  e_1
//                        /                   \
//                       /                     \
//                      /                       v
//                     O       O---->----O       O
//                      ^     ^     b     \     /
//                       \   /             \   /
//                        \ /               \ /
//                     c_0 /                 / c_1
//                        / \               / \
//                       /   \             /   \
//                      /     \     a     v     v
//           O---->----O       O----<----O       O---->----O

// So we cannot do anything meaningful.
                
            }
        }
        else
        {
            // This is a very seldom (impossible?) case.
            // TODO: Actually, this is an indicator for a connect sum.
            
//            logprint("Reidemeister_Ia_Horizontal_impl - twist move!");
//
//            logvalprint( "io", io == In ? "In" : "Out" );
//            logvalprint( "side", side == Left ? "Left" : "Right" );

//            Int f_0;
//            Int f_1;
//            if( io == Out )
//            {
//                if( side == Left )
//                {
//// We have this horizontal alignment in the case of io = Out and side == Left.
//// All crossing's handedness can also be switched (at the same time!).
////
////                              O       O
////                             / ^     / ^
////                            /   \   /   \
////                           /     \ /     \
////                          /       X c_2   \
////                    e_1  /       / \       \  e_0
////                        /       /   \       \
////                       /       v     \       \
////                      v       O       O       \
////                     +        |       ^        +
////                     |    f_0 v       | f_1    ^
////                     |      +-----------+      |
////                     |      |  Tangle   |      |
////                     |      +-----------+      |
////                     v                         |
////                     O       O----<----O       O
////                      \     /           ^     ^
////                       \   /             \   /
////                        \ /               \ /
////                     c_1 /                 / c_0
////                        / \               / \
////                       /   \             /   \
////                      v     v           /     \
////           O----<----O       O---->----O       O----<----O
////
//                    f_0 = C_arcs( c_2, Out, Left );
//                    f_1 = C_arcs( c_2, In , Left );
//                }
//                else
//                {
//// We have this horizontal alignment in the case of io = Out and side == Right.
//// (In the case side == Left or io == In, we just have to reflect everything accordingly.)
//// All crossing's handedness can also be switched (at the same time!).
////
////                              O       O
////                             ^ \     ^ \
////                            /   \   /   \
////                           /     v /     \
////                          /       X c_2   \
////                    e_0  /       ^ \       \  e_1
////                        /       /   \       \
////                       /       /     v       \
////                      /       O       O       v
////                     +        ^       |        +
////                     ^    f_1 |       v f_0    |
////                     |      +-----------+      |
////                     |      |  Tangle   |      |
////                     |      +-----------+      |
////                     |                         v
////                     O       O---->----O       O
////                      ^     ^           \     /
////                       \   /             \   /
////                        \ /               \ /
////                     c_0 /                 / c_1
////                        / \               / \
////                       /   \             /   \
////                      /     \           v     v
////           O---->----O       O----<----O       O---->----O
////
////
//                    f_0 = C_arcs( c_2, Out, Right );
//                    f_1 = C_arcs( c_2, In , Right );
//                }
//            }
//            else
//            {
//                if( side == Left )
//                {
//// We have this horizontal alignment in the case of io = In and side == Left.
//// All crossing's handedness can also be switched (at the same time!).
////
////           O---->----O       O----<----O       O---->----O
////                      \     /           ^     ^
////                       \   /             \   /
////                        \ /               \ /
////                     c_1 /                 / c_0
////                        / \               / \
////                       /   \             /   \
////                      v     v           /     \
////                     O       O---->----O       O
////                     |                         ^
////                     |      +-----------+      |
////                     |      |  Tangle   |      |
////                     |      +-----------+      |
////                     v    f_0 |       ^ f_1    |
////                     +        v       |        +
////                      \       O       O       ^
////                       \       \     ^       /
////                        \       \   /       /
////                    e_1  \       \ /       /  e_0
////                          \       X c_2   /
////                           \     / \     /
////                            \   /   \   /
////                             v /     v /
////                              O       O
//
//                    f_0 = C_arcs( c_2, In , Left );
//                    f_1 = C_arcs( c_2, Out, Left );
//                }
//                else
//                {
//// We have this horizontal alignment in the case of io = In and side == Right.
//// All crossing's handedness can also be switched (at the same time!).
////
////           O---->----O       O---->----O       O---->----O
////                      ^     ^           \     /
////                       \   /             \   /
////                        \ /               \ /
////                     c_0 /                 / c_1
////                        / \               / \
////                       /   \             /   \
////                      /     \           v     v
////                     O       O----<----O       O
////                     ^                         |
////                     |      +-----------+      |
////                     |      |  Tangle   |      |
////                     |      +-----------+      |
////                     |    f_1 ^       | f_0    v
////                     +        |       v        +
////                      ^       O       O       /
////                       \       ^     /       /
////                        \       \   /       /
////                    e_0  \       \ /       /  e_1
////                          \       X c_2   /
////                           \     / \     /
////                            \   /   \   /
////                             \ v     \ v
////                              O       O
//
//                    f_0 = C_arcs( c_2, In , Right );
//                    f_1 = C_arcs( c_2, Out, Right );
//                }
//            }
            
//            const Int f_0 = C_arcs( c_2,  io, side );
//            const Int f_1 = C_arcs( c_2, !io, side );
//
//            logvalprint("c_0",CrossingString(c_0));
//            logvalprint("c_1",CrossingString(c_1));
//            logvalprint("c_2",CrossingString(c_2));
//
//            logvalprint("e_0",ArcString(e_0));
//            logvalprint("e_1",ArcString(e_1));
//
//
//            logvalprint("f_0",ArcString(f_0));
//            logvalprint("f_1",ArcString(f_1));
//
//
//            PD_ASSERT( CheckArc(f_0) );
//            PD_ASSERT( CheckArc(f_1) );
//
//            PD_ASSERT( A_cross( f_0, io == Out ? Tail : Head ) == c_2 );
//            PD_ASSERT( A_cross( f_1, io == Out ? Head : Tail ) == c_2 );
//
//            PD_ASSERT( e_0 != f_1 );
//            PD_ASSERT( e_0 != f_0 );
//            PD_ASSERT( e_1 != f_1 );
//            PD_ASSERT( e_1 != f_0 );
//
//            Reconnect(f_0, io == Out ? Tail : Head ,e_0);
//            PD_ASSERT( CheckArc(f_0) );
//            logvalprint("f_0",ArcString(f_0));
//
//            Reconnect(f_1, io == Out ? Head : Tail ,e_1);
//            PD_ASSERT( CheckArc(f_1) );
//            logvalprint("f_1",ArcString(f_1));
//
//
//            DeactivateCrossing(c_2);
//
//            logprint("Horizontal reroute checks.");
//
//
//            PD_ASSERT( CheckCrossing(c_0) );
//            PD_ASSERT( CheckCrossing(c_1) );
//            logvalprint("f_0",ArcString(f_0));
//            logvalprint("f_1",ArcString(f_1));
//
//
//            logprint("Horizontal reroute checks done.");
//
//            return true;
            
            return false;

        }
    }
    
    return false;
}
