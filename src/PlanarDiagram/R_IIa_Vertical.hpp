private:

bool Reidemeister_IIa_Vertical( const Int c_0 )
{
    if( !CrossingActiveQ(c_0) )
    {
        return false;
    }
    
    PD_PRINT("Reidemeister_IIa_Vertical");

    auto C_0 = GetCrossing( c_0 );
    
    auto B_1 = GetArc( C_0(Out,Left ) );
    auto A_1 = GetArc( C_0(Out,Right) );
    auto A_0 = GetArc( C_0(In ,Left ) );
    auto B_0 = GetArc( C_0(In ,Right) );
    auto C_1 = GetCrossing( A_1(Head) );
    auto A_2 = NextArc(A_1);
    auto C_2 = GetCrossing( A_2(Head) );
    
    if( SameHandednessQ(C_0,C_2) )
    {
        // Not what we are looking for.
        return false;
    }
    else
    {
        auto A_3 = GetArc( C_2(Out,Left ) );
        auto B_3 = GetArc( C_2(Out,Right) );
        auto B_2 = GetArc( C_2(In ,Left ) );
        auto C_3 = GetCrossing( B_1(Head) );
        
        if( C_1 == C_3  )
        {
            // Not what we are looking for.
            return false;
        }
        
        if( OppositeHandednessQ(C_1,C_3) )
        {
            return false;
        }
        
        if( C_3 != B_2(Tail) )
        {
            // Not what we are looking for.
            return false;
        }

// Now C_1 and C_3 should share an arc and the situation should look like this.
// (The handedness of crossings c_0 and c_1 might be flipped.)
//
//          A_3 O       O B_3
//               ^     ^
//                \   /
//                 \ /
//                  \ C_2
//                 / \
//                /   \
//               /     \
//              O       O
//         B_2 ^         ^ A_2
//            /           \
//           O             O
//           ^             ^
//           |C_3          |C_1
//    ---O---X---O-----O---X---O---
//           |             |
//           |             |
//           O             O
//        B_1 ^           ^ A_1
//             \         /
//              O       O
//               ^     ^
//                \   /
//                 \ /
//                  / C_0
//                 / \
//                /   \
//               /     \
//          A_0 O       O B_0
//
//
        
        
        PD_ASSERT( C_0(In ,Left ) == A_0 );
        PD_ASSERT( C_0(Out,Right) == A_1 );
        
        PD_ASSERT( C_0(In ,Right) == B_0 );
        PD_ASSERT( C_0(Out,Left ) == B_1 );

        PD_ASSERT( C_2(In ,Right) == A_2 );
        PD_ASSERT( C_2(Out,Left ) == A_3 );
        
        PD_ASSERT( C_2(In ,Left ) == B_2 );
        PD_ASSERT( C_2(Out,Right) == B_3 );
        
        PD_ASSERT( A_0(Head) == C_0 );
        PD_ASSERT( B_0(Head) == C_0 );
        
        PD_ASSERT( A_1(Head) == C_1 );
        PD_ASSERT( A_2(Tail) == C_1 );
        
        PD_ASSERT( B_1(Head) == C_3 );
        PD_ASSERT( B_2(Tail) == C_3 );
        
        PD_ASSERT( A_3(Tail) == C_2 );
        PD_ASSERT( B_3(Tail) == C_2 );
        
        PD_ASSERT( OppositeHandednessQ(C_0,C_2) );
        PD_ASSERT( SameHandednessQ(C_1,C_3) );
        
        
        PD_PRINT("Incoming data");
        
        PD_VALPRINT("C_0",C_0);
        PD_VALPRINT("C_1",C_1);
        PD_VALPRINT("C_2",C_2);
        PD_VALPRINT("C_3",C_3);
        PD_VALPRINT("A_0",A_0);
        PD_VALPRINT("A_1",A_1);
        PD_VALPRINT("A_2",A_2);
        PD_VALPRINT("A_3",A_3);
        PD_VALPRINT("B_0",B_0);
        PD_VALPRINT("B_1",B_1);
        PD_VALPRINT("B_2",B_2);
        PD_VALPRINT("B_3",B_3);
        
        Reconnect(A_0,Head,B_1);
        Reconnect(B_0,Head,A_1);
        
        Reconnect(A_3,Tail,B_2);
        Reconnect(B_3,Tail,A_2);
        
        DeactivateCrossing(C_0.Idx());
        DeactivateCrossing(C_2.Idx());
        
        // Now c_1 and c_3 should share an arc and the situation should look like this.
        // (The handedness of crossings c_0 and c_1 might be flipped.)
        //
        //           ^             ^
        //           |             |
        //       E_3 |             | B_3
        //           |             |
        //           |             |
        //           O             O
        //           ^             ^
        //           |C_3          |C_1
        //    ---O---X---O-----O---X---O---
        //           |             |
        //           |             |
        //           O             O
        //           ^             ^
        //           |             |
        //           |             |
        //       E_0 |             | B_0
        //           |             |
        //
        //
        
        
        PD_PRINT("Changed data");
        
        PD_VALPRINT("C_1",C_1);
        PD_VALPRINT("C_3",C_3);
        PD_VALPRINT("A_0",A_0);
        PD_VALPRINT("A_3",A_3);
        PD_VALPRINT("B_0",B_0);
        PD_VALPRINT("B_3",B_3);
        
           
        PD_ASSERT(!C_0.ActiveQ() );
        PD_ASSERT(!C_2.ActiveQ() );
        PD_ASSERT( C_1.ActiveQ() );
        PD_ASSERT( C_3.ActiveQ() );
        
        PD_ASSERT( CheckCrossing(C_1.Idx()) );
        PD_ASSERT( CheckCrossing(C_3.Idx()) );
        
        PD_ASSERT(!A_1.ActiveQ() );
        PD_ASSERT(!B_1.ActiveQ() );
        PD_ASSERT(!A_2.ActiveQ() );
        PD_ASSERT(!B_2.ActiveQ() );
        PD_ASSERT( A_0.ActiveQ() );
        PD_ASSERT( B_0.ActiveQ() );
        PD_ASSERT( A_3.ActiveQ() );
        PD_ASSERT( B_3.ActiveQ() );
        
        PD_ASSERT( CheckArc(A_0.Idx()) );
        PD_ASSERT( CheckArc(B_0.Idx()) );
        PD_ASSERT( CheckArc(A_3.Idx()) );
        PD_ASSERT( CheckArc(B_3.Idx()) );
        
        PD_ASSERT( SameHandednessQ(C_1,C_3) );
        
        PD_ASSERT( A_0(Head) == C_3 );
        PD_ASSERT( B_0(Head) == C_1 );
        
        PD_ASSERT( A_3(Tail) == C_3 );
        PD_ASSERT( B_3(Tail) == C_1 );
        
        ++R_IIa_counter;
        
        return true;
    }
}


bool Reidemeister_IIa_Vertical_B( const Int c_0 )
{
    if( !CrossingActiveQ(c_0) )
    {
        return false;
    }
    
    PD_PRINT("Reidemeister_IIa_Vertical");

    PD_ASSERT( CheckCrossing(c_0) );
    
    const Int f_1 = C_arcs(c_0,Out,Left );
    const Int e_1 = C_arcs(c_0,Out,Right);
    const Int e_0 = C_arcs(c_0,In ,Left);
    const Int f_0 = C_arcs(c_0,In ,Right);
    
    PD_ASSERT( ArcActiveQ(f_1) );
    PD_ASSERT( ArcActiveQ(e_1) );
    PD_ASSERT( ArcActiveQ(e_0) );
    PD_ASSERT( ArcActiveQ(f_0) );
    
    PD_ASSERT( CheckArc(f_1) );
    PD_ASSERT( CheckArc(e_1) );
    PD_ASSERT( CheckArc(e_0) );
    PD_ASSERT( CheckArc(f_0) );

    const Int c_1 = A_cross(e_1,Head);
    const Int e_2 = NextArc(e_1);
    const Int c_2 = A_cross(e_2,Head);
    
    PD_ASSERT(CrossingActiveQ(c_1));
    PD_ASSERT(CrossingActiveQ(c_2));
    
    PD_ASSERT(CheckCrossing(c_1));
    PD_ASSERT(CheckCrossing(c_2));
    
    PD_ASSERT(ArcActiveQ(e_2));
    PD_ASSERT(CheckArc(e_2));
    
    PD_ASSERT( (0 <= c_1) && (c_1 < C_arcs.Dimension(0)) );
    PD_ASSERT( (0 <= c_2) && (c_2 < C_arcs.Dimension(0)) );
    
    if( SameHandednessQ(c_0,c_2) )
    {
        // Not what we are looking for.
        return false;
    }
    else
    {
        const Int e_3 = C_arcs(c_2,Out,Left );
        const Int f_3 = C_arcs(c_2,Out,Right);
        const Int f_2 = C_arcs(c_2,In,Left);
        
        PD_ASSERT( ArcActiveQ(e_3) );
        PD_ASSERT( ArcActiveQ(f_3) );
        PD_ASSERT( ArcActiveQ(f_2) );
        
        PD_ASSERT( CheckArc(e_3) );
        PD_ASSERT( CheckArc(f_3) );
        PD_ASSERT( CheckArc(f_2) );
        
        
        const Int c_3 = A_cross(f_1,Head);
        
        if( c_1 == c_3  )
        {
            // Not what we are looking for.
            return false;
        }
        
        if( OppositeHandednessQ(c_1,c_3) )
        {
            return false;
        }
        
        if( (c_3 != A_cross(f_2,Tail))  )
        {
            // Not what we are looking for.
            return false;
        }

// Now c_1 and c_3 should share an arc and the situation should look like this.
// (The handedness of crossings c_0 and c_1 might be flipped.)
//
//          e_3 O       O f_3
//               ^     ^
//                \   /
//                 \ /
//                  \ c_2
//                 / \
//                /   \
//               /     \
//              O       O
//         f_2 ^         ^ e_2
//            /           \
//           O             O
//           ^             ^
//           |c_3          |c_1
//    ---O---X---O-----O---X---O---
//           |             |
//           |             |
//           O             O
//        f_1 ^           ^ e_1
//             \         /
//              O       O
//               ^     ^
//                \   /
//                 \ /
//                  / c_0
//                 / \
//                /   \
//               /     \
//          e_0 O       O f_0
//
//
        
        
        PD_ASSERT( C_arcs(c_0,In ,Left ) == e_0 );
        PD_ASSERT( C_arcs(c_0,Out,Right) == e_1 );
        
        PD_ASSERT( C_arcs(c_0,In ,Right) == f_0 );
        PD_ASSERT( C_arcs(c_0,Out,Left ) == f_1 );

        PD_ASSERT( C_arcs(c_2,In ,Right) == e_2 );
        PD_ASSERT( C_arcs(c_2,Out,Left ) == e_3 );
        
        PD_ASSERT( C_arcs(c_2,In ,Left ) == f_2 );
        PD_ASSERT( C_arcs(c_2,Out,Right) == f_3 );
        
        PD_ASSERT( A_cross(e_0,Head) == c_0 );
        PD_ASSERT( A_cross(f_0,Head) == c_0 );
        
        PD_ASSERT( A_cross(e_1,Head) == c_1 );
        PD_ASSERT( A_cross(e_2,Tail) == c_1 );
        
        PD_ASSERT( A_cross(f_1,Head) == c_3 );
        PD_ASSERT( A_cross(f_2,Tail) == c_3 );
        
        PD_ASSERT( A_cross(e_3,Tail) == c_2 );
        PD_ASSERT( A_cross(f_3,Tail) == c_2 );
        
        PD_ASSERT( OppositeHandednessQ(c_0,c_2) );
        PD_ASSERT( SameHandednessQ(c_1,c_3) );
        
        
        PD_PRINT("Incoming data");
        
        PD_VALPRINT("c_0",CrossingString(c_0));
        PD_VALPRINT("c_1",CrossingString(c_1));
        PD_VALPRINT("c_2",CrossingString(c_2));
        PD_VALPRINT("c_3",CrossingString(c_3));
        PD_VALPRINT("e_0",ArcString(e_0));
        PD_VALPRINT("e_1",ArcString(e_1));
        PD_VALPRINT("e_2",ArcString(e_2));
        PD_VALPRINT("e_3",ArcString(e_3));
        PD_VALPRINT("f_0",ArcString(f_0));
        PD_VALPRINT("f_1",ArcString(f_1));
        PD_VALPRINT("f_2",ArcString(f_2));
        PD_VALPRINT("f_3",ArcString(f_3));
        
        Reconnect(e_0,Head,f_1);
        Reconnect(f_0,Head,e_1);
        
        Reconnect(e_3,Tail,f_2);
        Reconnect(f_3,Tail,e_2);
        
        DeactivateCrossing(c_0);
        DeactivateCrossing(c_2);
        
        // Now c_1 and c_3 should share an arc and the situation should look like this.
        // (The handedness of crossings c_0 and c_1 might be flipped.)
        //
        //           ^             ^
        //           |             |
        //       e_3 |             | f_3
        //           |             |
        //           |             |
        //           O             O
        //           ^             ^
        //           |c_3          |c_1
        //    ---O---X---O-----O---X---O---
        //           |             |
        //           |             |
        //           O             O
        //           ^             ^
        //           |             |
        //           |             |
        //       e_0 |             | f_0
        //           |             |
        //
        //
        
        
        PD_PRINT("Changed data");
        
        PD_VALPRINT("c_1",CrossingString(c_1));
        PD_VALPRINT("c_3",CrossingString(c_3));
        PD_VALPRINT("e_0",ArcString(e_0));
        PD_VALPRINT("e_3",ArcString(e_3));
        PD_VALPRINT("f_0",ArcString(f_0));
        PD_VALPRINT("f_3",ArcString(f_3));
        
        PD_ASSERT(!CrossingActiveQ(c_0) );
        PD_ASSERT(!CrossingActiveQ(c_2) );
        PD_ASSERT( CrossingActiveQ(c_1) );
        PD_ASSERT( CrossingActiveQ(c_3) );
        
        PD_ASSERT( CheckCrossing(c_1) );
        PD_ASSERT( CheckCrossing(c_3) );
        
        PD_ASSERT(!ArcActiveQ(e_1) );
        PD_ASSERT(!ArcActiveQ(f_1) );
        PD_ASSERT(!ArcActiveQ(e_2) );
        PD_ASSERT(!ArcActiveQ(f_2) );
        PD_ASSERT( ArcActiveQ(e_0) );
        PD_ASSERT( ArcActiveQ(f_0) );
        PD_ASSERT( ArcActiveQ(e_3) );
        PD_ASSERT( ArcActiveQ(f_3) );
        
        PD_ASSERT( CheckArc(e_0) );
        PD_ASSERT( CheckArc(f_0) );
        PD_ASSERT( CheckArc(e_3) );
        PD_ASSERT( CheckArc(f_3) );
        
        PD_ASSERT( SameHandednessQ(c_1,c_3) );
        
        PD_ASSERT( A_cross(e_0,Head) == c_3 );
        PD_ASSERT( A_cross(f_0,Head) == c_1 );
        
        PD_ASSERT( A_cross(e_3,Tail) == c_3 );
        PD_ASSERT( A_cross(f_3,Tail) == c_1 );
        
        ++R_IIa_counter;
        
        return true;
    }
}