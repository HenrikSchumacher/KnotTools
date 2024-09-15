private:

/*! @brief Checks whether a Reidemeister I move can be made at crossing `c`,
 *  then applies it (if possible), and returns a Boolean that indicates whether
 *  a change has been made or not.
 */

bool Reidemeister_I( const Int c )
{
    PD_PRINT("\nReidemeister_I( c = "+CrossingString(c)+" )");

    //
    // This is precisely the case if two consecutive arcs coincide.
    
    if( !CrossingActiveQ(c) )
    {
        PD_PRINT("Crossing "+ToString(c)+" is not active. Skipping");

        return false;
    }
    
    // Let's see on which side the loop is.
    bool side;
    
    auto C = GetCrossing( c );
    
    if( C(Out,Left ) == C(In ,Left ) )
    {
        side = Left;
    }
    else if ( C(Out,Right) == C(In ,Right) )
    {
        side = Right;
    }
    else
    {
        return false;
    }
    
    // This is the central assumption here.
    PD_ASSERT( C(Out,side) == C(In ,side) );
    
    auto A = GetArc( C(In ,!side) );
    auto B = GetArc( C(Out,!side) );
    auto D = GetArc( C(Out, side) );   // This is the looping arc (see picture below).
    
    if( A != B )
    {
// Changing this (case side == Right )
//                                        B = C(Out,Right)
//                            +-<---O       O------->---O...
//                           /       ^     ^             w
//                          /         \   /
//        C(Out,Left )     /           \ /
//           = D =        |             /  c                v != w
//        C(In ,Left )     \           / \
//                          \         /   \
//                           \       /     \            v
//                            +--->-O       O---<-------O...
//                                        A = C(In ,Right)
//
// to that:
//
//                                        B = C(Out,Right)
//                            +-<---O       O------->---O...
//                           /       ^     ^          w ^
//                          /         \   /             |
//        C(Out,Left )     /           \ /              |
//           = D =        |             /  C            |
//        C(In ,Left )     \           / \              |  A = C(In ,Right)
//                          \         /   \             |
//                           \       /     \            |
//                            +--->-O       O         v O...
//
        
        // Make arc a point to where arc b pointed before.
        Reconnect(A,Head,B);
        
        DeactivateArc(D.Idx());
        
        DeactivateCrossing(C.Idx());
        
        goto exit;
    }
    else
    {
// Otherwise we have this case of an unlink.
//
//                            +-<---O       O--->-+
//                           /       ^     ^   B   \
//                          /         \   /         \
//       C(Out,Left )      /           \ /           \
//          = D =         |             /  C          |  A == B
//       C(In ,Left )      \           / \           /
//                          \         /   \         /
//                           \       /     \   A   /
//                            +--->-O       O-<---+
        ++unlink_count;
        
        DeactivateArc(A.Idx());
        DeactivateArc(D.Idx());
        
        DeactivateCrossing(C.Idx());
        
        goto exit;
    }
   
exit:
    
    ++R_I_counter;
    
    return true;
}


bool Reidemeister_I_B( const Int c )
{
    PD_PRINT("\nReidemeister_I( c = "+CrossingString(c)+" )");

    //
    // This is precisely the case if two consecutive arcs coincide.
    
    if( !CrossingActiveQ(c) )
    {
        PD_PRINT("Crossing "+ToString(c)+" is not active. Skipping");

        return false;
    }
    
    // Let's see on which side the loop is.
    bool side;
    
    Int A [2][2];
    copy_buffer<4>( C_arcs.data(c), &A[0][0] );
    
    if( A[Out][Left ] == A[In ][Left ] )
    {
        side = Left;
    }
    else if ( A[Out][Right] == A[In ][Right] )
    {
        side = Right;
    }
    else
    {
        return false;
    }
    
    // This is the central assumption here.
    PD_ASSERT( A[Out][side] == A[In ][side] );
    
    const Int a = A[In ][!side];
    const Int b = A[Out][!side];
    const Int d = A[Out][ side];   // This is the looping arc (see picture below).
    
    if( a != b )
    {
// Changing this (case side == Right )
//                                        b = A[Out][Right]
//                            +-<---O       O------->---O...
//                           /       ^     ^             w
//                          /         \   /
//        A[Out][Left]     /           \ /
//           = d =        |             /  c                v != w
//        A[In ][Left]     \           / \
//                          \         /   \
//                           \       /     \            v
//                            +--->-O       O---<-------O...
//                                        a = A[In][Right]
//
// to that:
//
//                                        b = A[Out][Right]
//                            +-<---O       O------->---O...
//                           /       ^     ^          w ^
//                          /         \   /             |
//        A[Out][Left]     /           \ /              |
//           = d =        |             /  c            |
//        A[In ][Left]     \           / \              |  a = A[In][Right]
//                          \         /   \             |
//                           \       /     \            |
//                            +--->-O       O         v O...
//
        
        // Make arc a point to where arc b pointed before.
        Reconnect(a,Head,b);
        
        DeactivateArc(d);
        
        DeactivateCrossing(c);
        
        goto exit;
    }
    else
    {
// Otherwise we have this case of an unlink.
//
//                            +-<---O       O--->-+
//                           /       ^     ^   b   \
//                          /         \   /         \
//       A[Out][Left]      /           \ /           \
//          = d =         |             /  c          |  a == b
//       A[In ][Left]      \           / \           /
//                          \         /   \         /
//                           \       /     \   a   /
//                            +--->-O       O-<---+
        ++unlink_count;
        
        DeactivateArc(a);
        DeactivateArc(d);
        
        DeactivateCrossing(c);
        
        goto exit;
    }
   
exit:
    
//    CheckAll();
    
    
    ++R_I_counter;
    
    return true;
}