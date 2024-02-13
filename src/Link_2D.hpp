#pragma  once

namespace KnotTools
{
    template<typename Real_ = double, typename Int_ = long long, typename SInt_ = signed char>
    struct Intersection
    {
        using Real = Real_;
        using Int  = Int_;
        using SInt = SInt_;
        
        const Int  edges [2] = {-2}; // First edge goes over, second edge goes under.
        const Real times [2] = {-2};
        
        const SInt sign;
        
        Intersection(
            const Int over_edge_,
            const Int under_edge_,
            const Real over_edge_time_,
            const Real under_edge_time_,
            const SInt sign_
        )
        :   edges { over_edge_,      under_edge_      }
        ,   times { over_edge_time_, under_edge_time_ }
        ,   sign  ( sign_ )
        {}
        
        ~Intersection() = default;
    };

    template<typename Real_ = double, typename Int_ = long long, typename SInt_ = short int>
    class alignas( ObjectAlignment ) Link_2D : public Link<Int_>
    {
        // This data type is mostly intended to read in 3D vertex coordinates, to apply a planar projection and then to generate an object of type PlanarDiagram or some other representation of planar diagrams. Hence this class' main routines are FindIntersections (using a static binary tree) and CreatePlanarDiagram.
        
        
        // This implementation is single-threaded only so that many instances of this object can be used in parallel.
    public:
        
        using Real = Real_;
        using Int  = Int_;
        using Int  = SInt_;
        
        using Base_T = Link<Int>;
        
    protected:
        
        ASSERT_FLOAT(Real);
        ASSERT_INT(Int);
        
        static constexpr Int max_depth = 128;
        
        static constexpr Real one     = 1;
        static constexpr Real eps     = std::numeric_limits<Real>::epsilon();
        static constexpr Real big_one = 1 + eps;
        
        
        using Base_T::edges;
        using Base_T::next_edge;
        using Base_T::edge_ctr;
        using Base_T::edge_ptr;
        using Base_T::edge_count;
        using Base_T::component_count;
        using Base_T::component_ptr;
        using Base_T::cyclicQ;
        using Base_T::preorderedQ;

        
        using Tree2_T        = AABBTree<2,Real,Int>;
        using Tree3_T        = AABBTree<3,Real,Int>;
        
        using Vector2_T      = Tree2_T::Vector_T:
        using Vector2_T      = Tree3_T::Vector_T:
        
        using EContainer_T   = Tree3_T::EContainer_T;

        using BContainer_T   = Tree2_T::BContainer_T;
        
        using Intersection_T = Intersection<Real,Int,SInt>;
        
        using BinaryMatrix_T = Sparse::BinaryMatrixCSR<Int,std::size_t>;
        
    public:
        
        using Base_T::ComponentCount;
        using Base_T::VertexCount;
        using Base_T::EdgeCount;
        using Base_T::Edges;
        
    protected:
        
        //Containers and data whose sizes stay constant under ReadVertexCoordinates.
        EContainer_T edge_coords;
        
        Tiny::Matrix<3,3,Real,Int> { { {1,0,0}, {0,1,0}, {0,0,1} } }; // a rotation matrix (later to be randomized)

        Tree2_T T;
        
        BContainer_T  box_coords;
        
        // Containers that might have to be reallocated after calls to ReadVertexCoordinates.
        std::vector<Intersection_T> intersections;
        Tensor1<Int,Int>  edge_intersections;
        Tensor1<Real,Int> edge_times;
        Tensor1<bool,Int> edge_overQ;
                 
        Int intersections_3D = 0;
        Int intersections_nontransversal = 0;


    public:
        
        Link_2D() = default;
        
        ~Link_2D() = default;
        
        
        // Calling this constructor makes the object assume that it represents a cyclic polyline.
        explicit Link_2D( const Int edge_count_ )
        :   Base_T      ( edge_count_   )
        ,   edge_coords ( edge_count_   )
        ,   T           ( edge_count_   )
        ,   box_coords  ( T.NodeCount() )
        {
            ptic(ClassName()+"() (cyclic)");
            
//            intersections.reserve( static_cast<size_t>(2 * edge_count_) );
                        
            ptoc(ClassName()+"() (cyclic)");
        }
        
        template<typename J, typename K, IS_INT(J), IS_INT(K)>
        explicit Link_2D( Tensor1<J,K> & component_ptr_ )
        :   Base_T      ( component_ptr_       )
        ,   edge_coords ( component_ptr.Last() )
        ,   T           ( component_ptr.Last() )
        ,   box_coords  ( T.NodeCount()        )
        {}
        
        // Provide a list of edges in interleaved form to make the object figure out its topology.
        Link_2D( cptr<Int> edges_, const Int edge_count_ )
        :   Base_T      ( edges_, edge_count_ )
        ,   edge_coords ( edge_count_         )
        ,   T           ( edge_count_         )
        ,   box_coords  ( T.NodeCount()       )
        {}
        
        // Provide lists of edge tails and edge tips to make the object figure out its topology.
        Link_2D( cptr<Int> edge_tails_, cptr<Int> edge_tips_, const Int edge_count_ )
        :   Base_T      ( edge_tails_, edge_tips_ )
        ,   edge_coords ( edge_count_             )
        ,   T           ( edge_count_             )
        ,   box_coords  ( T.NodeCount()           )
        {}
        
    public:
        
        void ReadVertexCoordinates( cptr<Real> v, const bool update = false )
        {
            ptic(ClassName()+"::ReadVertexCoordinates (AoS)");
            
            if( preorderedQ )
            {
                for( Int c = 0; c < component_count; ++c )
                {
                    const Int i_begin = component_ptr[c  ];
                    const Int i_end   = component_ptr[c+1];

                    for( Int i = i_begin; i < i_end-1; ++i )
                    {
                        const int j = i+1;
                      
                        copy_buffer<3>( &v[3*i], edge_coords.data(i,0) );
                        copy_buffer<3>( &v[3*j], edge_coords.data(i,1) );
                    }

                    {
                        const Int i = i_end-1;
                        const Int j = i_begin;

                        copy_buffer<3>( &v[3*i], edge_coords.data(i,0) );
                        copy_buffer<3>( &v[3*j], edge_coords.data(i,1) );
                    }
                }
            }
            else
            {
                cptr<Int> edge_tails = edges.data(0);
                cptr<Int> edge_tips  = edges.data(1);
                
                for( Int edge = 0; edge < edge_count; ++edge )
                {
                    const Int i = edge_tails[edge];
                    const Int j = edge_tips [edge];
                    
                    copy_buffer<3>( &v[3*i], edge_coords.data(edge,0) );
                    copy_buffer<3>( &v[3*j], edge_coords.data(edge,1) );
                }
            }
            ptoc(ClassName()+"::ReadVertexCoordinates (AoS)");
        }
        
        void ReadVertexCoordinates( cptr<Real> x, cptr<Real> y, cptr<Real> z )
        {
            if( preorderedQ )
            {
                for( Int c = 0; c < component_count; ++c )
                {
                    const Int i_begin = component_ptr[c  ];
                    const Int i_end   = component_ptr[c+1];

                    for( Int i = i_begin; i < i_end-1; ++i )
                    {
                        const int j = i+1;

                        edge_coords(i,0,0) = x[i];
                        edge_coords(i,0,1) = y[i];
                        edge_coords(i,0,2) = z[i];
                        
                        edge_coords(i,1,0) = x[j];
                        edge_coords(i,1,1) = y[j];
                        edge_coords(i,1,2) = z[j];
                    }

                    {
                        const Int i = i_end-1;
                        const Int j = i_begin;

                        edge_coords(i,0,0) = x[i];
                        edge_coords(i,0,1) = y[i];
                        edge_coords(i,0,2) = z[i];
                        
                        edge_coords(i,1,0) = x[j];
                        edge_coords(i,1,1) = y[j];
                        edge_coords(i,1,2) = z[j];
                    }
                }
            }
            else
            {
                cptr<Int> edge_tails = edges.data(0);
                cptr<Int> edge_tips  = edges.data(1);
                
                for( Int edge = 0; edge < edge_count; ++edge )
                {
                    const Int i = edge_tails[edge];
                    const Int j = edge_tips [edge];
                    
                    edge_coords(edge,0,0) = x[i];
                    edge_coords(edge,0,1) = y[i];
                    edge_coords(edge,0,2) = z[i];
                    
                    edge_coords(edge,1,0) = x[j];
                    edge_coords(edge,1,1) = y[j];
                    edge_coords(edge,1,2) = z[j];
                }
            }
            ptoc(ClassName()+"::ReadVertexCoordinates (SoA)");
        }
        
        
        void Rotate()
        {
            cptr<Int> edge_tails = edges.data(0);
            cptr<Int> edge_tips  = edges.data(1);
            
            for( Int edge = 0; edge < edge_count; ++edge )
            {
                const Int i = edge_tails[edge];
                const Int j = edge_tips [edge];
                
                const Vector3_T x ( edge_coords.data(edge,0) );
                const Vector3_T y ( edge_coords.data(edge,1) );
                
                const Vector3_T Rx = Dot( R, x );
                const Vector3_T Ry = Dot( R, y );
                
                Rx.Write( edge_coords.data(edge,0) );
                Ry.Write( edge_coords.data(edge,1) );
            }
        }
        
    public:
        
        static constexpr Int AmbientDimension()
        {
            return 3;
        }
        
    public:
        
        void FindIntersections()
        {
            ptic(ClassName()+"FindIntersections");
            
            // Here we do something strange:
            // We hand over edge_coords, a Tensor3 of size edge_count x 2 x 3
            // to a T which is a Tree2_T.
            // The latter expects a Tensor3 of size edge_count x 2 x 2, but it accesses the
            // enties only via operator(i,j,k), so this is safe!

            T.ComputeBoundingBoxes( edge_coords, box_coords );

            FindIntersectingEdges_DFS();
            
            // We are going to use edge_ptr for the assembly; because we are going to modify it, we need a copy.
            edge_ctr.Read( edge_ptr.data() );
            
            if( edge_intersections.Size() < edge_ptr.Last() )
            {
                edge_intersections = Tensor1<Int, Int>( edge_ptr.Last() );
                edge_times         = Tensor1<Real,Int>( edge_ptr.Last() );
                edge_overQ         = Tensor1<bool,Int>( edge_ptr.Last() );
            }

            // We are going to fill edge_intersections so that data of the i-th edge lies in edge_intersections[edge_ptr[i]],..,edge_intersections[edge_ptr[i+1]].
            // To this end, we use (and modify!) edge_ctr so that edge_ctr[i] points AFTER the position to insert.
            
            
            const Int intersection_count = static_cast<Int>(intersections.size());
            
            for( Int k = intersection_count-1; k > -1; --k )
            {
                Intersection_T & inter = intersections[k];
                
                // We have to write BEFORE the positions specified by edge_ctr (and decrease it for the next write;

                const Int pos_0 = --edge_ctr[inter.edges[0]+1];
                const Int pos_1 = --edge_ctr[inter.edges[1]+1];

                edge_intersections[pos_0] = k;
                edge_times        [pos_0] = inter.times[0];
                edge_overQ        [pos_0] = true;
                
                edge_intersections[pos_1] = k;
                edge_times        [pos_1] = inter.times[1];
                edge_overQ        [pos_1] = false;
            }
            
            // Sort intersections edgewise w.r.t. edge_times.
            ThreeArrayQuickSort<Real,Int,bool> Q;

            for( Int i = 0; i < edge_count; ++i )
            {
                // This is the range of data in edge_intersections/edge_times that belongs to edge i.
                const Int k_begin = edge_ptr[i  ];
                const Int k_end   = edge_ptr[i+1];

                Q.Sort(
                    &edge_times[k_begin],
                    &edge_intersections[k_begin],
                    &edge_overQ[k_begin],
                    k_end - k_begin
                );
            }
            
            // From now on we can safely cycle around each component and generate vertices, edges, crossings, etc. in their order.
            
            
            ptoc(ClassName()+"FindIntersections");
        }
        
        
    protected:
        
        void FindIntersectingEdges_DFS()
        {
            ptic(ClassName()+"::FindIntersectingEdges_DFS");
            
            const Int int_node_count = T.InteriorNodeCount();
            
            intersections.clear();
            
            intersections.reserve( 2 * edge_coords.Dimension(0) );
            
            intersections_3D = 0;
            intersections_nontransversal = 0;
            
            Int i_stack[max_depth] = {};
            Int j_stack[max_depth] = {};
            
            Int stack_ptr = 0;
            i_stack[0] = 0;
            j_stack[0] = 0;
            
            edge_ptr.Fill(0);

            //Preparing pointers for quick access.
            
            cptr<Int> next = next_edge.data();
            mptr<Int> ctr  = &edge_ptr.data()[1];
            
            while( (0 <= stack_ptr) && (stack_ptr < max_depth - 4) )
            {
                const Int i = i_stack[stack_ptr];
                const Int j = j_stack[stack_ptr];
                stack_ptr--;
                    
                bool boxes_intersecting = (i == j)
                    ? true
                    : (
                       ( box_coords(i,0,0) <= box_coords(j,0,1) )
                       &&
                       ( box_coords(i,0,1) >= box_coords(j,0,0) )
                       &&
                       ( box_coords(i,1,0) <= box_coords(j,1,1) )
                       &&
                       ( box_coords(i,1,1) >= box_coords(j,1,0) )
                    );
                

                if( boxes_intersecting )
                {
                    const bool is_interior_i = (i < int_node_count);
                    const bool is_interior_j = (j < int_node_count);
                    
                    // Warning: This assumes that both children in a cluster tree are either defined or empty.
                    if( is_interior_i || is_interior_j )
                    {
                        const Int L_i  = Tree_T::LeftChild(i);
                        const Int R_i = L_i+1;
                        
                        const Int L_j  = Tree_T::LeftChild(j);
                        const Int R_j = L_j+1;
                        
                        // TODO: Improve score.

                        if( (is_interior_i == is_interior_j ) /*&& (score_i > static_cast<Real>(0)) && score_j > static_cast<Real>(0)*/ )
                        {
                            if( i == j )
                            {
                                //  Creating 3 blockcluster children, since there is one block that is just the mirror of another one.
                                
                                ++stack_ptr;
                                i_stack[stack_ptr] = L_i;
                                j_stack[stack_ptr] = R_j;
                                
                                ++stack_ptr;
                                i_stack[stack_ptr] = R_i;
                                j_stack[stack_ptr] = R_j;
                                
                                ++stack_ptr;
                                i_stack[stack_ptr] = L_i;
                                j_stack[stack_ptr] = L_j;
                            }
                            else
                            {
                                // tie breaker: split both clusters
                                
                                ++stack_ptr;
                                i_stack[stack_ptr] = R_i;
                                j_stack[stack_ptr] = R_j;
                                
                                ++stack_ptr;
                                i_stack[stack_ptr] = L_i;
                                j_stack[stack_ptr] = R_j;
                                
                                ++stack_ptr;
                                i_stack[stack_ptr] = R_i;
                                j_stack[stack_ptr] = L_j;
                                
                                ++stack_ptr;
                                i_stack[stack_ptr] = L_i;
                                j_stack[stack_ptr] = L_j;
                            }
                        }
                        else
                        {
                            // split only larger cluster
                            if( is_interior_i ) // !is_interior_j follows from this.
                            {
                                ++stack_ptr;
                                i_stack[stack_ptr] = R_i;
                                j_stack[stack_ptr] = j;
                                
                                //split cluster i
                                ++stack_ptr;
                                i_stack[stack_ptr] = L_i;
                                j_stack[stack_ptr] = j;
                            }
                            else //score_i < score_j
                            {
                                //split cluster j
                                ++stack_ptr;
                                i_stack[stack_ptr] = i;
                                j_stack[stack_ptr] = R_j;
                                
                                ++stack_ptr;
                                i_stack[stack_ptr] = i;
                                j_stack[stack_ptr] = L_j;
                            }
                        }
                    }
                    else
                    {
                        // Translate node indices i and j to edge indices k and l.
                        const Int k = i - int_node_count;
                        const Int l = j - int_node_count;
                    
                        // Only check for intersection of edge k and l if they are not equal and not direct neighbors.
                        if( (l != k) && (l != next[k]) && (k != next[l]) )
                        {
                            // Get the edge lengths in order to decide what's a "small" determinant.
                            
                            const Vector3_T x[2] = {
                                { edge_coords.data(k,0) }, { edge_coords.data(k,1) }
                            };
                            
                            const Vector3_T y[2] = {
                                { edge_coords.data(l,0) }, { edge_coords.data(l,1) }
                            };
                            
                            const Vector2_T d { y[0][0] - x[0][0], y[0][1] - x[0][1] };
                            const Vector2_T u { x[1][0] - x[0][0], x[1][1] - x[0][1] };
                            const Vector2_T v { y[1][0] - y[0][0], y[1][1] - y[0][1] };

                            const Real det = Det2D_Kahan( u[0], u[1], v[0], v[1] );
                            
                            Real t[2];

                            bool intersecting;

                            if( std::abs(det*det) > eps * u.NormSquared() * v.NormSquared() )
                            {
                                const Real det_inv = static_cast<Real>(1) / det;
                                
                                t[0] = Det2D_Kahan( d[0], d[1], v[0], v[1] ) * det_inv;
                                t[1] = Det2D_Kahan( d[0], d[1], u[0], u[1] ) * det_inv;

                                intersecting = (t[0] > - eps) && (t[0] < big_one) && (t[1] > - eps) && (t[1] < big_one);
                            }
                            else
                            {
                                intersecting = false;

                                intersections_nontransversal++;
                            }

                            if( intersecting )
                            {
                                
                                // Compute heights at the intersection.
                                const Real h[2] = {
                                    x[0][2] * (one - t[0]) + t[0] * x[1][2],
                                    y[0][2] * (one - t[1]) + t[1] * y[1][2]
                                };
                                
                                // Tell edges k and l that they contain an additional crossing.
                                ctr[k]++;
                                ctr[l]++;

                                if( h[0] < h[1] )
                                {
                                    // edge k goes UNDER edge l
                                    
                                    intersections.push_back(
                                        Intersection_T( l, k, t[1], t[0], -Sign(det) )
                                    );
                                    
//      If det > 0, then this looks like this (negative crossing):
//
//        v       u
//         ^     ^
//          \   /
//           \ /
//            \
//           / \
//          /   \
//         /     \
//        k       l
//
//      If det < 0, then this looks like this (positive crossing):
//
//        u       v
//         ^     ^
//          \   /
//           \ /
//            /
//           / \
//          /   \
//         /     \
//        l       k

                                }
                                else if ( h[0] > h[1] )
                                {
                                    intersections.push_back(
                                        Intersection_T( k, l, t[0], t[1], Sign(det) )
                                    );
                                    // edge k goes OVER l
                                    
//      If det > 0, then this looks like this (positive crossing):
//
//        v       u
//         ^     ^
//          \   /
//           \ /
//            /
//           / \
//          /   \
//         /     \
//        k       l
//
//      If det < 0, then this looks like this (positive crossing):
//
//        u       v
//         ^     ^
//          \   /
//           \ /
//            \
//           / \
//          /   \
//         /     \
//        l       k
                                }
                                else
                                {
                                    intersections_3D++;
                                }
                            }
                        }
                    }
                }
            }
            
            edge_ptr.Accumulate();
            
            ptoc(ClassName()+"::FindIntersectingEdges_DFS");
        } // FindIntersectingClusters_DFS
        
        
    public:
        
        static inline SInt Sign( const Real x )
        {
            return Tools::Sign<SInt>(x);
        }
        
        Int CrossingCount() const
        {
            return static_cast<Int>( intersections.size() );
        }
        
        Int DegenerateIntersectionCount() const
        {
            return intersections_nontransversal;
        }
        
        Int InvalidIntersectionCount() const
        {
            return intersections_3D;
        }
        
        static std::string ClassName()
        {
            return "Link_2D<"+TypeName<Real>+","+TypeName<Int>+">";
        }
    };
    
} // namespace KnotTools
