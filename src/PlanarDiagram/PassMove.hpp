// ### = over-strand
//
//              ############
//              #  s       #
//              O          #
//              |          #
//              |          #
//      w_0     v       s  #             w_0
//     -----O------->O######     =>     -----O---+
//              |c_0                             |
//              v                                |
//              O                                O
//              | s_0                            | s_0
//
// Beware of the case w_0 == s_0!

// ### = over-strand
//
//    #####################
//    #                   #
//    #         O b       #                     O
//    #         |         #                     |
//    #         |         #                     |
//    ######O-------O######     =>              |    + unlink
//              |c                              |
//              |                               |
//              O                               |
//              | a                             | a
//


// ### = over-strand
//
//                                            a_0
//    ##########O                       O-------------O
//    #         |                       |             |
//    #         |      a                |     a_2     |      a
//    #     O-------O---     =>     O------>O-----O---|---O---
//    #         |c                      |c            |c'
//    #         |                       |             |
//    ##########O                       O-------------O
//                                            a_1
//


// ### = over-strand
//
// If over-strand contains 3 arcs or more:
//
//              ###############
//              #             #                           b
//              O             O                    O-------------O
//              |             |                    |             |
//              |      a      |                    |      a      |
//          O-------O-----O---|---O     =>     O-------O-----O---|---O
//              |c_0          |c_1                 |c_0          |c_1
//              |             |                    |             |
//              O             O                    O             O
//
// Beware of cases w_0 == s_0, (s_0 == s_1), s_1 == e_1, w_0 == s_1!!

// ### = over-strand
//
//              ###############
//              #             #
//              O             O
//              |             |
//       w_0    |      a      |    e_1                   w_0
//          O---|---O-----O---|---O     =>     O---------------------O
//              |c_0          |c_1
//              |             |                          s_0
//              O             O                    O-------------O
//               s_0           s_1
//
// Beware of cases w_0 == s_0, s_0 == s_1, s_1 == e_1, w_0 == e_1!!

// ### = over-strand
//
//  n_0 == e_1 cannot happen.
//
//              ###################
//              #                 #                               n_1
//          n_0 O         n_1 O   #                              O
//              |             |   #                              |
//       w_0    |      a      |   #                   w_0        |
//          O---|---O-----O---|---O     =>      -------------O---|---O
//              |c_0          |c_1 e_1                           |c_1|
//              |             |                                  |   |
//              O             O                                  O   |
//                s_0           s_1                              |   |
//                                                           s_1 |   | e_1
//                                                               |   |
//                                                               O   |
//                                                               |   |
//                                                     s_0       |   |
//                                                 ----------O-------O
//                                                               |c_0
//                                                               |
//                                                               O n_0
// Beware of the cases w_0 == s_0 and s_0 == s_1.

// ### = over-strand
//
//              ###################
//              #                 #
//          n_0 O         n_1 O   #
//              |             |   #
//       w_0    |      a      |   #
//          O---|---O-----O-------O     =>      It's a 1-crossing case!
//              |c_0          |c_1 e_1
//              |             |
//              O             O
//              | s_0           s_1
//

// ### = over-strand
//
//
//     ##########
//     #        #              n_1                  n_1
//     #        O             O                    O
//     #        |             |                    |
//     #        |      a      |    e_1             |    e_1
//     #    O---|---O-----O---|---O     =>     O---|---O
//     # w_0    |c_0          |c_1          w_0    |c_0
//     #        |             |                    |
//     #        O             O                    O
//     #         s_0          #                     s_0
//     ########################
//
// If n_1 == e_1, then we can chain this with an R_I move.


// ### = over-strand
//
//
//     ##########
//     #        #              n_1                  n_1
//     #        O             O                    O
//     #        |             |                    |
//     #        |      a      |    e_1             |    e_1
//     #    O---|---O-----O-------O     =>     O-------O
//     # w_0    |c_0          |c_1          w_0    |c_0
//     #        |             |                    |
//     #        O             O                    O
//     #         s_0          #                     s_0
//     ########################
//
// If n_1 == e_1, then we can chain this with an R_I move.

// ### = over-strand
//
//
//                             n_1
//              O             O
//              |             |
//       w_0    |      a      |    e_1
//          O---|---O-----O-------O     =>  It's a 1-crossing case!
//          #   |c_0          |c_1#
//          #   |             |   #
//          #   O             O   #
//          #    s_0       s_1    #
//          #######################

// ### = over-strand
//
// If s_0 != s_1
//                             n_1
//              O             O
//              |             |
//       w_0    |      a      |    e_1
//          O---|---O-----O---|---O     =>  Have to insert crossings into s_0, s_1.
//          #   |c_0          |c_1#
//          #   |             |   #
//          #   O             O   #
//          #    s_0       s_1    #
//          #######################
//
// If s_0 == s_1, then this simplifies a lot more: --> R_II move


// Edge case:
//
//              |c_0
//        +------------+
//        |     |      |
//        |     |      |c_1
//        |     +------------+
//        |            |     |
//        |            |     |
//        +------------------+
//                     |
//
//
//              ###############
//              #             #n_1
//              O             O
//              |             |
//              |      a      |    e_1
//       ---O---|---O-----O-------O####
//              |c_0          |c_1    #
//              |             |       #
//              O             O       #
//              #s_0          |       #
//              #######################
//                            |



//      |  |  |  |  |  |  |  |
//   +--------------------------+
//   |  |  |  |  |  |  |  |  |  |
// b |                          |
//   |                          |
//   |                          |
//   |            |             |
//   |            |             |
//   |            |             |     a
//-------------------------------------
//   |c_0         |             |c_1



//      |  |  |  |  |  |  |  |
//      |  |  |  |  |  |  |  |
//      |  |  |  |  |  |  |  |
//
//
//
// b'             |
//   +--------------------------+
//   |            |             |     a
//-------------------------------------
//   |c_0         |             |c_1





//public:
//
//    Int OverPassMoves()
//    {
////        Int old_counter = -1;
//        Int counter = 0;
////        Int iter = 0;
//        
////        while( counter != old_counter )
////        {
////            ++iter;
////            old_counter = counter;
////            
//            const auto C_pass_arcs = CrossingOverStrands();
//            
//            for( Int c = 0; c < initial_crossing_count; ++c )
//            {
//                counter += PassMove(c,C_pass_arcs);
//            }
////            
////        }
//        
////        valprint( ClassName() +"::OverPassMoves: iterations", iter );
//        
//        return counter;
//    }
//
//    Int UnderPassMoves()
//    {
//        const auto C_pass_arcs = CrossingUnderStrands();
//
//        Int counter = 0;
//        
//        for( Int c = 0; c < initial_crossing_count; ++c )
//        {
//            counter += PassMove(c,C_pass_arcs);
//        }
//        
//        return counter;
//    }
//
//private:
//
//    bool PassMove( const Int c, cref<Tensor3<Int,Int>> C_pass_arcs  )
//    {
//        if( !CrossingActiveQ(c) )
//        {
//            PD_PRINT( ClassName()+"PassMove: Crossing "+ToString(c)+" is not active. Skipping");
//
//            return false;
//        }
//        
//        // If we can do a Reidemeister I move, then we should do it.
//        
//        const bool RI = Reidemeister_I(c);
//        
//        if( RI )
//        {
//            return true;
//        }
//        
//        // TODO: Detect potential over/under-arc 8-loop and reduce it to unlink!
//        
//        if( C_pass_arcs(c,Out,Left) == C_pass_arcs(c,In,Left) )
//        {
//            print("side = Left");
//            
//            const Int a_begin = C_arcs(c,Out,Left);
//            const Int a_end   = C_arcs(c,In ,Left);
//            
//            // When we arrive here, we know that a_begin != a_end.
//            // Otherwise we would have quit through Reidemeister_I.
//            
//            PD_ASSERT( a_begin != a_end );
//            PD_ASSERT( A_cross(a_begin,Tail) == A_cross(a_end,Head) );
//            
//            // We also know that c is not the crossing of an 8-loop.
//            PD_ASSERT( C_arcs(c,Out,Right) != C_arcs(c,In,Right) );
//            
//            // DEBUGGING
//            
//            if( C_pass_arcs(c,Out,Right) == C_pass_arcs(c,In,Right) )
//            {
//                wprint("Overpassing 8-loop.");
//            }
//            
//            /*
//             *             a_begin
//             *       ...X<---         --->X
//             *      .        \       /
//             *     .          \     /
//             *    .            \   /
//             *    .             \ /
//             *    .         c -->X
//             *    .             ^ ^
//             *    .            /   \
//             *     .          /     \
//             *      .        /       \
//             *       ...X----         ----X
//             *             a_end
//             */
//
//
//            RemoveArcs( a_begin, a_end );
//        
//            Reconnect(C_arcs(c,In,Right),Head,C_arcs(c,Out,Right));
//            
//            DeactivateCrossing(c);
//            
//            return true;
//            
////            return false;
//            
//        }
//        else if ( C_pass_arcs(c,Out,Right) == C_pass_arcs(c,In,Right) )
//        {
//            print("side = Right");
//            const Int a_begin = C_arcs(c,Out,Right);
//            const Int a_end   = C_arcs(c,In ,Right);
//            
//            // When we arrive here, we know that a_begin != a_end.
//            // Otherwise we would have quit through Reidemeister_I.
//            
//            PD_ASSERT( a_begin != a_end );
//            PD_ASSERT( A_cross(a_begin,Tail) == A_cross(a_end,Head) );
//            
//            // We also know that c is not the crossing of an 8-loop.
//            PD_ASSERT( C_arcs(c,Out,Left) != C_arcs(c,In,Left) );
//            
//            // DEBUGGING
//            
//            if( C_pass_arcs(c,Out,Left) == C_pass_arcs(c,In,Left) )
//            {
//                wprint("Overpassing 8-loop.");
//            }
//            /*
//             *              a_begin
//             *    X<---         --->X...
//             *         \       /        .
//             *          \     /          .
//             *           \   /            .
//             *            \ /             .
//             *        c -->X              .
//             *            ^ ^             .
//             *           /   \            .
//             *          /     \          .
//             *         /       \        .
//             *    X----         ----X...
//             *                a_end
//             */
//            
//            RemoveArcs( a_begin, a_end );
////
//            Reconnect(C_arcs(c,In,Left),Head,C_arcs(c,Out,Left));
////
//            DeactivateCrossing(c);
////            
//            return true;
//            
////            return false;
//        }
//        else
//        {
//            return false;
//        }
//    }
//
//    void RemoveArcs( const Int a_begin, const Int a_end )
//    {
//        Int a = a_begin;
//        
//        PD_ASSERT( ArcActiveQ(a_begin) );
//        PD_ASSERT( ArcActiveQ(a_end) );
//        
//        PD_ASSERT( A_cross(a_begin,Tail) == A_cross(a_end,Head) );
//        
//        dump( A_cross(a_begin,Tail) );
//        
//        while( a != a_end )
//        {
//            const Int c = A_cross(a,Head);
//            
//            dump( ArcString(a) );
//            dump( CrossingString(c) );
//                 
//            
////            if( !CrossingActiveQ(c) )
////            {
////                continue;
////            }
//            
//            
//            PD_ASSERT( CrossingActiveQ(c) );
//            PD_ASSERT( ArcActiveQ(a) );
//            
//            PD_ASSERT( (C_arcs(c,In,Left) == a) || (C_arcs(c,In,Right) == a) );
//            
//            // Find the arcs a_0 and a_1 go through c to the left and the right of a.
//            bool side = C_arcs(c,In,Right) == a;
//            
//            // Arc `a` comes into crossing `c` through the right port.
//            
//            /*  side == right       side == left
//             *
//             *        ^                   ^
//             *        |                   |
//             *        |                   |
//             *   a_0  | a_1          a_1  | a_0
//             *  ----->+----->       <-----+<-----
//             *        ^                   ^
//             *        |                   |
//             *        | a                 | a
//             *        |                   |
//             */
//            
//            const Int a_next = NextArc(a);
//            const Int a_0    = C_arcs(c,In ,!side);
//            const Int a_1    = C_arcs(c,Out, side);
//
//            PD_ASSERT( ArcActiveQ(a_0) );
//            PD_ASSERT( ArcActiveQ(a_1) );
//            
//            Reconnect(a_0,Head,a_1);
//
//            DeactivateCrossing(c);
//            DeactivateArc(a);
//            
//            a = a_next;
//        }
//        
//        dump( ArcString(a) );
//        dump( A_cross(a_end,Head) );
//        
////        DeactivateArc(a_end);
//    }
