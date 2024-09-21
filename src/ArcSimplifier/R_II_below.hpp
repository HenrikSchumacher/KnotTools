bool R_II_below()
{
    PD_DPRINT( "R_II_below()" );
    
    if( s_0 == s_1 )
    {
        PD_DPRINT( "\ts_0 == s_1" );
        
        // TODO: If the endpoints of n_0 and n_1 coincide, we can remove even 3 crossings by doing the unlocked Reidemeister I move.
        // TODO: Price: Load 1 additional crossing for test. Since a later R_I would remove the crossing anyways, this does not seem to be overly attractive.
        // TODO: On the other hand: we need to reference to the two end point anyways to do the reconnecting right...
        
        /*                n_0         n_1                    n_0         n_1
         *               |     a     |                      |     a     |
         *        w_0 -->|---------->|--> e_1   or   w_0 -->----------->---> e_1
         *               |c_0        |c_1                   |c_0        |c_1
         *               |           |                      |           |
         *               +-----------+                      +-----------+
         *                    s_0                                s_0
         */
        
        load_c_3();
        
        if( e_3 == n_1 )
        {
            PD_DPRINT( "\t\te_3 == n_1" );
            
            // We can make an additional R_I move here.
            
            /*              w_3 O   O n_3           w_3 O   O n_3
             *                   \ /                     \ /
             *                    X c_3                   X c_3
             *                   / \                     / \
             *                  O   O                   O   O
             *                 /     \                 /     \
             *                /       \               /       \
             *               O         O             O         O
             *               |         |             |         |
             *               |    a    |             |    a    |
             *        w_0 -->|-------->|-->   or  -->--------->---> e_1
             *               |c_0      |c_1          |c_0      |c_1
             *               +---------+             +---------+
             */
            
            if constexpr ( mult_compQ )
            {
                if( w_3 == n_3 )
                {
                    PD_DPRINT( "\t\t\tw_3 == n_3" );
                    
                    // These case w_0 == e_1, w_0 == s_0, e_1 == s_1 are ruled out already...
                    PD_ASSERT( w_0 != e_1 );
                    
                    ++pd.unlink_count;
                    Reconnect(w_0,Head,e_1); //... so this is safe.
                    DeactivateArc(a);
                    DeactivateArc(n_0);
                    DeactivateArc(s_0);
                    DeactivateArc(s_1);
                    DeactivateArc(w_3);
                    DeactivateCrossing(c_0);
                    DeactivateCrossing(c_1);
                    DeactivateCrossing(c_3);
                    ++pd.R_II_counter;
                    pd.R_I_counter += 2;
                    
                    AssertArc(w_0);
                    
                    return true;
                }
            }

            
            PD_DPRINT( "\t\t\tw_3 != n_3" );
            
            
            /*              w_3 O   O n_3           w_3 O   O n_3
             *                   \ /                     \ /
             *                    X c_3                   X c_3
             *                   / \                     / \
             *                  O   O                   O   O
             *                 /     \                 /     \
             *                /       \               /       \
             *               O         O             O         O
             *               |         |             |         |
             *               |    a    |             |    a    |
             *        w_0 -->|-------->|-->   or  -->--------->---> e_1
             *               |c_0      |c_1          |c_0      |c_1
             *               +---------+             +---------+
             */
            
            if( w_0 == w_3 )
            {
                PD_DPRINT( "\t\t\t\tw_0 == w_3" );
                
                if constexpr ( mult_compQ )
                {
                    if( e_1 == n_3 )
                    {
                        PD_DPRINT( "\t\t\t\t\te_1 == n_3" );
                        
                        /*               w_3     n_3
                         *        +---------O   O---------+
                         *        |          \ /          |
                         *        |           X c_3       |
                         *        |          / \          |
                         *        |         O   O         |
                         *        |        /     \        |
                         *        |       /       \       |
                         *        |      O         O      |
                         *        |      |         |      |
                         *        |      |c_0   c_1|      |
                         *        +--O-->|-------->|-->O--+
                         *        w_0    |    a    |    e_1
                         *               +---------+
                         */
                        
                        ++pd.unlink_count;
                        DeactivateArc(a);
                        DeactivateArc(n_0);
                        DeactivateArc(n_1);
                        DeactivateArc(s_0);
                        DeactivateArc(w_0);
                        DeactivateArc(e_1);
                        DeactivateCrossing(c_0);
                        DeactivateCrossing(c_1);
                        DeactivateCrossing(c_3);
                        ++pd.R_II_counter;
                        ++pd.R_I_counter;
                        
                        return true;
                    }
                }
                
                PD_DPRINT( "\t\t\t\t\te_1 != n_3" );
                PD_ASSERT( e_1 != n_3 );
                
                /*               w_3     n_3
                 *        +---------O   O
                 *        |          \ /
                 *        |           X c_3
                 *        |          / \
                 *        |         O   O
                 *        |        /     \
                 *        |       /       \
                 *        |      O         O
                 *        |      |         |
                 *        |      |c_0   c_1|
                 *        +--O-->|-------->|-->O
                 *        w_0    |    a    |    e_1
                 *               +---------+
                 */
                
                Reconnect(e_1,Tail,n_3);
                DeactivateArc(a);
                DeactivateArc(n_0);
                DeactivateArc(n_1);
                DeactivateArc(s_0);
                DeactivateArc(w_0);
                DeactivateCrossing(c_0);
                DeactivateCrossing(c_1);
                DeactivateCrossing(c_3);
                ++pd.R_II_counter;
                ++pd.R_I_counter;
                
                return true;
            }
            
            PD_DPRINT( "\t\t\t\tw_0 != w_3" );
            PD_ASSERT( w_0 != w_3 );
            
            if( e_1 == n_3 )
            {
                PD_DPRINT( "\t\t\t\t\te_1 == n_3" );
                
                /*               w_3     n_3
                 *                  O   O---------+
                 *                   \ /          |
                 *                    X c_3       |
                 *                   / \          |
                 *                  O   O         |
                 *                 /     \        |
                 *                /       \       |
                 *               O         O      |
                 *               |         |      |
                 *               |c_0   c_1|      |
                 *           O-->|-------->|-->O--+
                 *        w_0    |    a    |    e_1
                 *               +---------+
                 */
                
                Reconnect(w_0,Head,w_3);
                DeactivateArc(a);
                DeactivateArc(n_0);
                DeactivateArc(n_1);
                DeactivateArc(s_0);
                DeactivateArc(e_1);
                DeactivateCrossing(c_0);
                DeactivateCrossing(c_1);
                DeactivateCrossing(c_3);
                ++pd.R_II_counter;
                ++pd.R_I_counter;
                
                return true;
            }
            
            PD_DPRINT( "\t\t\t\t\te_1 != n_3" );
            PD_ASSERT( e_1 != n_3 );
            
            /*               w_3     n_3
             *                  O   O
             *                   \ /
             *                    X c_3
             *                   / \
             *                  O   O
             *                 /     \
             *                /       \
             *               O         O
             *               |         |
             *               |c_0   c_1|
             *           O-->|-------->|-->O
             *        w_0    |    a    |    e_1
             *               +---------+
             */
            
            // These case w_0 == e_1, w_0 == s_0, e_1 == s_1 are ruled out already...
            PD_ASSERT( w_0 != e_1 );
            
            Reconnect(w_0,Head,e_1); // ... so this is safe.

            // The ifs above make this safe..
            Reconnect(w_3,u_0,n_3);
            
            DeactivateArc(a);
            DeactivateArc(n_0);
            DeactivateArc(n_1);
            DeactivateArc(s_0);
            DeactivateCrossing(c_0);
            DeactivateCrossing(c_1);
            DeactivateCrossing(c_3);
            
            ++pd.R_II_counter;
            ++pd.R_I_counter;
            
            AssertArc(w_0);
            AssertArc(w_3);
            
            return true;
        }
        
        // These case w_0 == e_1, w_0 == n_0, e_1 == n_1 are ruled out already...
        PD_ASSERT( w_0 != e_1 );
        PD_ASSERT( w_0 != n_0 );
        PD_ASSERT( e_1 != n_1 );
        
        // .. so this is safe:
        Reconnect(w_0,Head,e_1);
        
        // We have already checked this in R_II_above.
        PD_ASSERT( n_0 != n_1 );
        
        Reconnect(n_0,!u_0,n_1);
        DeactivateArc(s_0);
        DeactivateArc(a);
        DeactivateCrossing(c_0);
        DeactivateCrossing(c_1);
        ++pd.R_II_counter;
        
        AssertArc(n_0);
        AssertArc(w_0);
        
        return true;
    }
    
    return false;
}
