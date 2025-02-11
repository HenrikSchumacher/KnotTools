protected:
    
    
    void FindIntersectingEdges_DFS()
    {
        ptic(ClassName()+"::FindIntersectingEdges_DFS");
        
//        FindIntersectingEdges_DFS_impl_0();
        
        FindIntersectingEdges_DFS_impl_1();

        ptoc(ClassName()+"::FindIntersectingEdges_DFS");
        
    } // FindIntersectingClusters_DFS

    void FindIntersectingEdges_DFS_impl_0()
    {
        const Int int_node_count = T.InteriorNodeCount();
        
        intersections.clear();
        
        intersections.reserve( ToSize_T(2 * edge_coords.Dimension(0)) );
        
        S = Intersector_T();
        
        intersection_count_3D = 0;
        
        Int stack [4 * max_depth][2];
        Int stack_ptr = -1;
        
        // Helper routine to manage the pair_stack.
        auto push = [&stack,&stack_ptr]( const Int i, const Int j )
        {
            ++stack_ptr;
            stack[stack_ptr][0] = i;
            stack[stack_ptr][1] = j;
        };
    
        // Helper routine to manage the pair_stack.
        auto pop = [&stack,&stack_ptr]()
        {
            const std::pair result ( stack[stack_ptr][0], stack[stack_ptr][1] );
            stack_ptr--;
            return result;
        };
        
        push(0,0);
        
        edge_ptr.Fill(0);
        
//        Size_T box_call_count  = 0;
//        Size_T edge_call_count = 0;
//        
//        double box_time  = 0;
//        double edge_time = 0;
        
        while( (0 <= stack_ptr) && (stack_ptr < 4 * max_depth - 4) )
        {
            // Pop from stack.

            auto [i,j] = pop();

//            Time box_start_time = Clock::now();
//            ++box_call_count;
            
            const bool boxes_intersectQ = BoxesIntersectQ(i,j);
            
//            Time box_end_time = Clock::now();
//            box_time += Tools::Duration( box_start_time, box_end_time );
            
            if( boxes_intersectQ )
            {
                const bool i_interiorQ = (i < int_node_count);
                const bool j_interiorQ = (j < int_node_count);
                
                // Warning: This assumes that both children in a cluster tree are either defined or empty.
                
                if( i_interiorQ || j_interiorQ )
                {
                    auto [L_i,R_i] = Tree2_T::Children(i);
                    auto [L_j,R_j] = Tree2_T::Children(j);
                    
                    // T is a balanced bindary tree.

                    if( i_interiorQ == j_interiorQ )
                    {
                        if( i == j )
                        {
                            //  Creating 3 blockcluster children, since there is one block that is just the mirror of another one.
                            
                            push(L_i,R_j);
                            push(R_i,R_j);
                            push(L_i,L_j);
                        }
                        else
                        {
                            // tie breaker: split both clusters
                            push(R_i,R_j);
                            push(L_i,R_j);
                            push(R_i,L_j);
                            push(L_i,L_j);
                        }
                    }
                    else
                    {
                        // split only larger cluster
                        if( i_interiorQ ) // !j_interiorQ follows from this.
                        {
                            //split cluster i
                            push(R_i,j);
                            push(L_i,j);
                        }
                        else
                        {
                            //split cluster j
                            push(i,R_j);
                            push(i,L_j);
                        }
                    }
                }
                else
                {
//                    Time edge_start_time = Clock::now();
//                    ++edge_call_count;

                    ComputeEdgeIntersection( T.NodeBegin(i), T.NodeBegin(j) );

//                    Time edge_end_time = Clock::now();
//                    edge_time += Tools::Duration( edge_start_time, edge_end_time );
                }
            }
        }
        
        edge_ptr.Accumulate();
        
//        dump(box_call_count);
//        dump(box_time);
//        dump(edge_call_count);
//        dump(edge_time);
        
    } // FindIntersectingClusters_DFS_impl_0


    // Improved version of FindIntersectingEdges_DFS_impl_0; we do the box-box checks of all the children at once; this saves is a couple of cache misses.
    void FindIntersectingEdges_DFS_impl_1()
    {
        const Int int_node_count = T.InteriorNodeCount();
        
        intersections.clear();
        
        intersections.reserve( ToSize_T(2 * edge_coords.Dimension(0)) );
        
        S = Intersector_T();
        
        intersection_count_3D = 0;
        
        Int stack [4 * max_depth][2];
        Int stack_ptr = -1;

        // Helper routine to manage the pair_stack.
        auto push = [&stack,&stack_ptr]( const Int i, const Int j )
        {
            ++stack_ptr;
            stack[stack_ptr][0] = i;
            stack[stack_ptr][1] = j;
        };
        
        // Helper routine to manage the pair_stack.
        auto check_push = [&stack,&stack_ptr,this]( const Int i, const Int j )
        {
            if( this->BoxesIntersectQ(i,j) )
            {
                ++stack_ptr;
                stack[stack_ptr][0] = i;
                stack[stack_ptr][1] = j;
            }
        };

        // Helper routine to manage the pair_stack.
        auto pop = [&stack,&stack_ptr]()
        {
            const std::pair result ( stack[stack_ptr][0], stack[stack_ptr][1] );
            stack_ptr--;
            return result;
        };
        
        auto continueQ = [&stack_ptr,this]()
        {
            const bool overflowQ = (stack_ptr >= 4 * max_depth - 4);
            
            if( (0 <= stack_ptr) && (!overflowQ) ) [[likely]]
            {
                return true;
            }
            else
            {
                if ( overflowQ ) [[unlikely]]
                {
                    eprint(this->ClassName()+"::FindIntersectingEdges_DFS_impl_1: Stack overflow.");
                }
                return false;
            }
        };
        
        
        push(0,0);
        
        edge_ptr.Fill(0);
        
    //        Size_T box_call_count  = 0;
    //        Size_T edge_call_count = 0;
    //
    //        double box_time  = 0;
    //        double edge_time = 0;
        
        while( continueQ() )
        {
            // Pop from stack.

            auto [i,j] = pop();

    //            Time box_start_time = Clock::now();
    //            ++box_call_count;
            
            
    //            Time box_end_time = Clock::now();
    //            box_time += Tools::Duration( box_start_time, box_end_time );
            
            
            const bool i_interiorQ = (i < int_node_count);
            const bool j_interiorQ = (j < int_node_count);
            
            // Warning: This assumes that both children in a cluster tree are either defined or empty.
            
            if( i_interiorQ || j_interiorQ ) // [[likely]]
            {
                auto [L_i,R_i] = Tree2_T::Children(i);
                auto [L_j,R_j] = Tree2_T::Children(j);
                
                // T is a balanced bindary tree.

                if( i_interiorQ == j_interiorQ )
                {
                    if( i == j )
                    {
                        //  Creating 3 blockcluster children, since there is one block that is just the mirror of another one.
                        
                        check_push(L_i,R_j);
                        check_push(R_i,R_j);
                        check_push(L_i,L_j);
                    }
                    else
                    {
                        // tie breaker: split both clusters
                        check_push(R_i,R_j);
                        check_push(L_i,R_j);
                        check_push(R_i,L_j);
                        check_push(L_i,L_j);
                    }
                }
                else
                {
                    // split only larger cluster
                    if( i_interiorQ ) // !j_interiorQ follows from this.
                    {
                        //split cluster i
                        check_push(R_i,j);
                        check_push(L_i,j);
                    }
                    else
                    {
                        //split cluster j
                        check_push(i,R_j);
                        check_push(i,L_j);
                    }
                }
            }
            else
            {
//                    Time edge_start_time = Clock::now();
//                    ++edge_call_count;

                ComputeEdgeIntersection( T.NodeBegin(i), T.NodeBegin(j) );

//                    Time edge_end_time = Clock::now();
//                    edge_time += Tools::Duration( edge_start_time, edge_end_time );
            }
        }
        
        edge_ptr.Accumulate();
        
    //        dump(box_call_count);
    //        dump(box_time);
    //        dump(edge_call_count);
    //        dump(edge_time);
        
    } // FindIntersectingClusters_DFS_impl_1

public:

    bool BoxesIntersectQ( const Int i, const Int j ) const
    {
        return T.BoxesIntersectQ( box_coords.data(i), box_coords.data(j) );
    }

protected:

    void ComputeEdgeIntersection( const Int k, const Int l )
    {
        // Only check for intersection of edge k and l if they are not equal and not direct neighbors.
        if( (l != k) && (l != next_edge[k]) && (k != next_edge[l]) )
        {
            // Get the edge lengths in order to decide what's a "small" determinant.
            
            const Tiny::Matrix<2,3,Real,Int> x { edge_coords.data(k) };
            const Tiny::Matrix<2,3,Real,Int> y { edge_coords.data(l) };
            
            const LineSegmentsIntersectionFlag flag
                = S.IntersectionType( x[0], x[1], y[0], y[1] );
            
//            ++intersection_counts[ ToUnderlying(flag) ];
            
            if( IntersectingQ(flag) )
            {
                auto [t,sign] = S.IntersectionTimesAndSign();
                
                // Compute heights at the intersection.
                const Real h[2] = {
                    x[0][2] * (one - t[0]) + t[0] * x[1][2],
                    y[0][2] * (one - t[1]) + t[1] * y[1][2]
                };
                
                // Tell edges k and l that they contain an additional crossing.
                edge_ptr[k+1]++;
                edge_ptr[l+1]++;

                if( h[0] < h[1] )
                {
                    // edge k goes UNDER edge l
                    
                    intersections.push_back( Intersection_T(l,k,t[1],t[0],-sign) );
                    
                    /*      If det > 0, then this looks like this (left-handed crossing):
                     *
                     *        v       u
                     *         ^     ^
                     *          \   /
                     *           \ /
                     *            \
                     *           / \
                     *          /   \
                     *         /     \
                     *        k       l
                     *
                     *      If det < 0, then this looks like this (right-handed crossing):
                     *
                     *        u       v
                     *         ^     ^
                     *          \   /
                     *           \ /
                     *            /
                     *           / \
                     *          /   \
                     *         /     \
                     *        l       k
                     */
                }
                else if ( h[0] > h[1] )
                {
                    intersections.push_back( Intersection_T(k,l,t[0],t[1],sign) );
                    // edge k goes OVER l
                    
                    /*      If det > 0, then this looks like this (positive crossing):
                     *
                     *        v       u
                     *         ^     ^
                     *          \   /
                     *           \ /
                     *            /
                     *           / \
                     *          /   \
                     *         /     \
                     *        k       l
                     *
                     *      If det < 0, then this looks like this (positive crossing):
                     *
                     *        u       v
                     *         ^     ^
                     *          \   /
                     *           \ /
                     *            \
                     *           / \
                     *          /   \
                     *         /     \
                     *        l       k
                     */
                }
                else
                {
                    ++intersection_count_3D;
                }
            }
        }
    }
