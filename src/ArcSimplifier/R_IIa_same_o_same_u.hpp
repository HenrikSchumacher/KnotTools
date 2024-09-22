void R_IIa_same_o_same_u()
{
    PD_DPRINT( "\t\tR_IIa_same_o_same_u()" );
    PD_ASSERT( u_0 == u_1 );
    
    // Check for four-crossing move.
    if( s_2 == w_3 )
    {
        PD_DPRINT( "\t\t\ts_2 == w_3" );
/*
 *          +--------------+                                            +--------------+
 *          |              |                                            |              |
 *          |  n_3         |                  n_3                       |  n_3         |
 *      w_3 O   O########  |               +---O########                +---O########  |
 *           \ /        #  |                \          #                            #  |
 *            / c_3     #  |                 \         #                            #  |
 *           / \        #  |                  \        #                            #  |
 *          O   O       #  |                   \       #                            #  |
 *     n_0 /     \ n_1  #  |                    \ n_1  #                            #  |
 *        /       \     #  |                     \     #                            #  |
 *       O         O    #  |                      O    #                            #  |
 *   w_0 |    a    |    #  |        w_0           |    #         w_0                #  |
 *  ##O--|->O---O--|->O##  |  ==>  ##O------------|->O##   ==>  ##O-------------->O##  |
 *  #    |c_0   c_1| e_1   |       #           c_1| e_1         #                e_1   |
 *  #    O         O       |       #              O             #                      |
 *  #     \       /        |       #             /              #                      |
 *  #  s_0 \     / s_1     |       #            /               #                      |
 *  #       O   O          |       #           /                #                      |
 *  #        \ /           |       #          /                 #                      |
 *  #         \ c_2        |       #         /                  #                      |
 *  #        / \           |       #        /                   #                      |
 *  ########O   O----------+       ########O                    ########O<-------------+
 *       w_2     s_2                    w_2                            w_2
 */
        if( w_0 == w_2 )
        {
            PD_DPRINT( "\t\t\t\tw_0 == w_2" );

            if( e_1 == n_3 )
            {
                PD_DPRINT( "\t\t\t\t\te_1 == n_3" );
                
                /*
                *            +--------------+
                *            |              |
                *            |  n_3         |
                *            +---O-------+  |
                *                        |  |
                *                        |  |
                *                        |  |
                *                        |  |
                *                        v  |
                *                        |  |
                *                        |  |
                *     w_0                |  |
                *    +-O-------------->O-+  |
                *    |                e_1   |
                *    |                      |
                *    |                      |
                *    ^                      |
                *    |                      |
                *    |                      |
                *    |                      |
                *    |                      |
                *    +-------O<-------------+
                *           w_2
                */
                
                ++pd.unlink_count;
                DeactivateArc(a);
                DeactivateArc(n_0);
                DeactivateArc(n_1);
                DeactivateArc(s_0);
                DeactivateArc(s_1);
                DeactivateArc(e_1); // e_1 == n_3;
                DeactivateArc(w_0); // w_2 == w_0;
                DeactivateArc(w_3); // s_2 == w_3;
                DeactivateCrossing(c_0);
                DeactivateCrossing(c_1);
                DeactivateCrossing(c_2);
                DeactivateCrossing(c_3);
                ++pd.R_IIa_counter;
                pd.R_I_counter += 2;
                
                return;
            }
            
            PD_DPRINT( "\t\t\t\t\te_1 != n_3" );
            PD_ASSERT( e_1 != n_3 );
            
            /*
            *            +--------------+
            *            |              |
            *            |  n_3         |
            *            +---O########  |
            *                        #  |
            *                        #  |
            *                        #  |
            *                        #  |
            *                        #  |
            *                        #  |
            *                        #  |
            *     w_0                #  |
            *    +-O-------------->O##  |
            *    |                e_1   |
            *    |                      |
            *    |                      |
            *    ^                      |
            *    |                      |
            *    |                      |
            *    |                      |
            *    |                      |
            *    +-------O<-------------+
            *           w_2
            */
            
            Reconnect(e_1,Tail,n_3);
            DeactivateArc(a);
            DeactivateArc(n_0);
            DeactivateArc(n_1);
            DeactivateArc(s_0);
            DeactivateArc(s_1);
            DeactivateArc(w_0); // w_2 == w_0;
            DeactivateArc(w_3); // s_2 == w_3;
            DeactivateCrossing(c_0);
            DeactivateCrossing(c_1);
            DeactivateCrossing(c_2);
            DeactivateCrossing(c_3);
            ++pd.R_IIa_counter;
            ++pd.R_I_counter;
            
            AssertArc(e_1);
            
            return;
        }
        
        PD_DPRINT( "\t\t\t\tw_0 != w_2" );
        PD_ASSERT( w_0 != w_2 );
        
        if( e_1 == n_3 )
        {
            PD_DPRINT( "\t\t\t\te_1 == n_3" );
            
            /*
            *            +--------------+
            *            |              |
            *            |  n_3         |
            *            +---O-------+  |
            *                        |  |
            *                        |  |
            *                        |  |
            *                        |  |
            *                        v  |
            *                        |  |
            *                        |  |
            *     w_0                |  |
            *    ##O-------------->O-+  |
            *    #                e_1   |
            *    #                      |
            *    #                      |
            *    #                      |
            *    #                      |
            *    #                      |
            *    #                      |
            *    #                      |
            *    ########O<-------------+
            *           w_2
            */
            
            Reconnect(w_0,Head,w_2);
            DeactivateArc(a);
            DeactivateArc(n_0);
            DeactivateArc(n_1);
            DeactivateArc(s_0);
            DeactivateArc(s_1);
            DeactivateArc(n_3); // e_1 == n_3
            DeactivateArc(w_3); // s_2 == w_3;
            DeactivateCrossing(c_0);
            DeactivateCrossing(c_1);
            DeactivateCrossing(c_2);
            DeactivateCrossing(c_3);
            ++pd.R_IIa_counter;
            ++pd.R_I_counter;
            
            AssertArc(w_0);
            
            return;
        }
        
        PD_DPRINT( "\t\t\t\te_1 != n_3" );
        
        /*
        *            +--------------+
        *            |              |
        *            |  n_3         |
        *            +---O########  |
        *                        #  |
        *                        #  |
        *                        #  |
        *                        #  |
        *                        #  |
        *                        #  |
        *                        #  |
        *     w_0                #  |
        *    ##O-------------->O##  |
        *    #                e_1   |
        *    #                      |
        *    #                      |
        *    #                      |
        *    #                      |
        *    #                      |
        *    #                      |
        *    #                      |
        *    ########O<-------------+
        *           w_2
        */
        
        Reconnect(w_0,Head,e_1);
        Reconnect(n_3,Head,w_2);
        DeactivateArc(a  );
        DeactivateArc(n_0);
        DeactivateArc(n_1);
        DeactivateArc(s_0);
        DeactivateArc(s_1);
        DeactivateCrossing(c_0);
        DeactivateCrossing(c_1);
        DeactivateCrossing(c_2);
        DeactivateCrossing(c_3);
        ++pd.R_IIa_counter;
        ++pd.twist_counter;
        
        AssertArc(w_0);
        AssertArc(n_3);
        
        return;
        
    } // if( s_2 == w_3 )
    
    
    // Check for four-crossing move.
    if( w_2 == n_3 )
    {
        PD_DPRINT( "\t\t\tw_2 == n_3" );
        
/*
*       w_3     n_3                        w_3                            w_3
*  ########O   O ---------+       ########O                    ########O--------------+
*  #        \ /           |       #        \                   #                      |
*  #         / c_3        |       #         \                  #                      |
*  #        / \           |       #          \                 #                      |
*  #       O   O          |       #           \                #                      |
*  #  n_0 /     \ n_1     |       #            \ n_1           #                      |
*  #     /       \        |       #             \              #                      |
*  #    O         O       |       #              O             #                      |
*  #    |    a    | e_1   |       #              | e_1         #                e_1   |
*  ##O--|->O---O--|->O##  |  ==>  ##O------------|->O##   ==>  ##O-------------->O##  |
*   w_0 |c_0   c_1|    #  |        w_0        c_1|    #         w_0                #  |
*       O         O    #  |                      O    #                            #  |
*        \       /     #  |                     /     #                            #  |
*     s_0 \     / s_1  #  |                    /      #                            #  |
*          O   O       #  |                   /       #                            #  |
*           \ /        #  |                  /        #                            #  |
*            \ c_2     #  |                 /         #                            #  |
*           / \        #  |                /          #                            #  |
*          O   O########  |               +---O########                    O########  |
*       w_2|    s_2       |                    s_2                         |s_2       |
*          |              |                                                |          |
*          +--------------+                                                +----------+
*/
        
        if( w_0 == w_3 )
        {
            PD_DPRINT( "\t\t\t\tw_0 == w_3" );
            
            if( e_1 == s_2 )
            {
                PD_DPRINT( "\t\t\t\t\te_1 == s_2" );
                
                
                /*
                 *         w_3
                 *  +-------O--------------+
                 *  |                      |
                 *  |                      |
                 *  |                      |
                 *  |                      |
                 *  |                      |
                 *  |                      |
                 *  |                      |
                 *  |                e_1   |
                 *  +-O-------------->O-+  |
                 *   w_0                |  |
                 *                      |  |
                 *                      |  |
                 *                      |  |
                 *                      |  |
                 *                      |  |
                 *                      |  |
                 *                      |  |
                 *              O-------+  |
                 *              |s_2       |
                 *              |          |
                 *              +----------+
                 */
                
                ++pd.unlink_count;
                DeactivateArc(a  );
                DeactivateArc(n_0);
                DeactivateArc(n_1);
                DeactivateArc(s_0);
                DeactivateArc(s_1);
                DeactivateArc(w_2);
                DeactivateArc(w_0); // w_3 == w_0
                DeactivateArc(e_1); // s_2 == e_1
                DeactivateCrossing(c_0);
                DeactivateCrossing(c_1);
                DeactivateCrossing(c_2);
                DeactivateCrossing(c_3);
                ++pd.four_counter;
                
                return;
            }
            
            PD_DPRINT( "\t\t\t\t\te_1 != s_2" );
            
            /*
             *         w_3
             *  +-------O--------------+
             *  |                      |
             *  |                      |
             *  |                      |
             *  |                      |
             *  |                      |
             *  |                      |
             *  |                      |
             *  |                e_1   |
             *  +-O-------------->O##  |
             *   w_0                #  |
             *                      #  |
             *                      #  |
             *                      #  |
             *                      #  |
             *                      #  |
             *                      #  |
             *                      #  |
             *              O########  |
             *              |s_2       |
             *              |          |
             *              +----------+
             */
            
            Reconnect(e_1,Tail,s_2);
            DeactivateArc(a  );
            DeactivateArc(n_0);
            DeactivateArc(n_1);
            DeactivateArc(s_0);
            DeactivateArc(s_1);
            DeactivateArc(w_2);
            DeactivateArc(w_0); // w_3 == w_0
            DeactivateCrossing(c_0);
            DeactivateCrossing(c_1);
            DeactivateCrossing(c_2);
            DeactivateCrossing(c_3);
            ++pd.four_counter;
            
            AssertArc(e_1);
            
            return;
        }
        
        PD_DPRINT( "\t\t\t\tw_0 != w_3" );
        if( e_1 == s_2 )
        {
            PD_DPRINT( "\t\t\t\t\te_1 == s_2" );
            
            /*
             *         w_3
             *  ########O--------------+
             *  #                      |
             *  #                      |
             *  #                      |
             *  #                      |
             *  #                      |
             *  #                      |
             *  #                      |
             *  #                e_1   |
             *  ##O-------------->O-+  |
             *   w_0                |  |
             *                      |  |
             *                      |  |
             *                      |  |
             *                      |  |
             *                      |  |
             *                      |  |
             *                      |  |
             *              O-------+  |
             *              |s_2       |
             *              |          |
             *              +----------+
             */
            
            Reconnect(w_0,Head,w_3);
            DeactivateArc(a  );
            DeactivateArc(n_0);
            DeactivateArc(n_1);
            DeactivateArc(s_0);
            DeactivateArc(s_1); // e_2 == s_1
            DeactivateArc(w_2);
            DeactivateArc(e_1);
            DeactivateCrossing(c_0);
            DeactivateCrossing(c_1);
            DeactivateCrossing(c_2);
            DeactivateCrossing(c_3);
            ++pd.four_counter;
            
            AssertArc(w_0);
            
            return;
        }
        
        
        PD_DPRINT( "\t\t\t\t\te_1 != s_2" );
        /*
        *         w_3
        *  ########O--------------+
        *  #                      |
        *  #                      |
        *  #                      |
        *  #                      |
        *  #                      |
        *  #                      |
        *  #                      |
        *  #                e_1   |
        *  ##O-------------->O##  |
        *   w_0                #  |
        *                      #  |
        *                      #  |
        *                      #  |
        *                      #  |
        *                      #  |
        *                      #  |
        *                      #  |
        *              O########  |
        *              |s_2       |
        *              |          |
        *              +----------+
        */
        
        Reconnect(w_0,Head,e_1);
        Reconnect(s_2,Head,w_3);
        DeactivateArc(a  );
        DeactivateArc(n_0);
        DeactivateArc(n_1);
        DeactivateArc(s_0);
        DeactivateArc(s_1);
        DeactivateArc(n_3);
        DeactivateCrossing(c_0);
        DeactivateCrossing(c_1);
        DeactivateCrossing(c_2);
        DeactivateCrossing(c_3);
        ++pd.four_counter;
        
        AssertArc(w_0);
        
        return;
        
    } // if( w_2 == n_3 )
    
    PD_DPRINT( "\t\t\t(s_2 != w_3) && (w_2 != n_3)" );
    PD_ASSERT( (s_2 != w_3) && (w_2 != n_3) );
    
    AssertArc(a  );
    AssertArc(n_0);
    AssertArc(s_0);
    AssertArc(n_1);
    AssertArc(s_1);
    AssertArc(w_0);
    AssertArc(e_1);
    
    AssertArc(w_3);
    AssertArc(w_2);
    AssertArc(n_3);
    AssertArc(s_2);
    
    PD_ASSERT( n_0 != w_3 );
    PD_ASSERT( s_0 != w_2 );
    PD_ASSERT( n_1 != n_3 );
    PD_ASSERT( s_1 != s_2 );
    
    Reconnect(w_3,!u_0,n_0);
    Reconnect(w_2, u_0,s_0);
    Reconnect(n_3,!u_1,n_1);
    Reconnect(s_2, u_1,s_1);
    DeactivateCrossing(c_2);
    DeactivateCrossing(c_3);
    ++pd.R_IIa_counter;
    
    AssertArc(a  );
    AssertArc(w_3);
    AssertArc(w_2);
    AssertArc(n_3);
    AssertArc(s_2);
    AssertArc(w_0);
    AssertArc(e_1);
    AssertCrossing(c_0);
    AssertCrossing(c_1);
}
