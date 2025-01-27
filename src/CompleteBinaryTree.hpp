#pragma once

namespace KnotTools
{
    
    template<typename Int_, bool precompute_rangesQ_ = false>
    class alignas( ObjectAlignment ) CompleteBinaryTree
    {
        static_assert(IntQ<Int_>,"");
        
    public:
        
        using Int  = Int_;
        
        static constexpr Int max_depth = 128;
        static constexpr bool precompute_rangesQ = precompute_rangesQ_;
        
        using UInt = Scalar::Unsigned<Int>;
        
        CompleteBinaryTree() = default;
        
        // TODO: What to do if leaf_node_count_ == 0?
        
        explicit CompleteBinaryTree( const Int leaf_node_count_  )
        :         leaf_node_count { leaf_node_count_                                   }
        ,              node_count { 2 * leaf_node_count - 1                            }
        ,          int_node_count { node_count - leaf_node_count                       }
        ,          last_row_begin { (Int(1) << Depth(node_count-1)) - 1                }
        ,                  offset { node_count - int_node_count - last_row_begin       }
        ,            actual_depth { Depth(node_count-1)                                }
        , regular_leaf_node_count { Int(1) << actual_depth                             }
        ,          last_row_count { Int(2) * leaf_node_count - regular_leaf_node_count }
        {
            if( leaf_node_count <= Int(0) )
            {
                eprint(ClassName()+" initialized with 0 leaf nodes.");
            }
            
//            dump(actual_depth);
//            dump(regular_leaf_node_count);
//            dump(last_row_count);
//
            if constexpr ( precompute_rangesQ )
            {
                N_ranges = Tensor2<Int,Int>(node_count,2);
                
                mptr<Int> r = N_ranges.data();
                
                // Compute range of leaf nodes in last row.
                for( Int N = last_row_begin; N < node_count; ++N )
                {
                    r[2 * N + 0] = N - last_row_begin    ;
                    r[2 * N + 1] = N - last_row_begin + 1;
                }
                
                // Compute range of leaf nodes in penultimate row.
                for( Int N = int_node_count; N < last_row_begin; ++N )
                {
                    r[2 * N + 0] = N + offset;
                    r[2 * N + 1] = N + offset + 1;
                }
                
                for( Int N = int_node_count; N --> 0; )
                {
                    const auto [L,R] = Children(N);
                    
                    r[2 * N + 0] = Min( r[2 * L + 0], r[2 * R + 0] );
                    r[2 * N + 1] = Max( r[2 * L + 1], r[2 * R + 1] );
                }
            }
        }
        
        ~CompleteBinaryTree() = default;
        
        
    protected:
        
        // Integer data for the combinatorics of the tree.
        // Corners of the bounding boxes.

        Int leaf_node_count = 0;
        
        Int node_count = 0;
        
        Int int_node_count = 0;

        Int last_row_begin = 0;
//        
        Int offset = 0;

        Tensor2<Int,Int> N_ranges;
        
        Int actual_depth            = 0;
        
        // A full binary tree with depth = actual_depth has this many leaf nodes.
        Int regular_leaf_node_count = 0;
        Int last_row_count          = 0;
        
    public:
        
        // See https://trstringer.com/complete-binary-tree/

        static constexpr Int Root()
        {
            return 0;
        }
        
        static constexpr Int MaxDepth()
        {
            return max_depth;
        }
        
        static constexpr Int LeftChild( const Int i )
        {
            return 2 * i + 1;
        }
        
        static constexpr Int RightChild( const Int i )
        {
            return 2 * i + 2;
        }
        
        static constexpr std::pair<Int,Int> Children( const Int i )
        {
            return { 2 * i + 1, 2 * i + 2 };
        }
        
        static constexpr Int Parent( const Int i )
        {
            return (i > 0) ? (i - 1) / 2 : -1;
        }
        
        static constexpr Int Depth( const Int i )
        {
            // Depth equals the position of the most significant bit if i+1.
            constexpr UInt one = 1;
            
            return static_cast<Int>( MSB( static_cast<UInt>(i) + one ) - one );
        }
        
        
        static constexpr Int Column( const Int i )
        {
            // The start of each column is the number with all bits < Depth() being set.
            
            constexpr UInt one = 1;
            
            UInt k = static_cast<UInt>(i) + one;

            return i - (PrevPow(k) - one);
        }
        
        
        Int ActualDepth() const
        {
            return actual_depth;
        }
        
        Int RegularLeafNodeCount( const Int i ) const
        {
            // I a full binary tree this node would contain this many leaf nodes.
            return regular_leaf_node_count >> Depth(i);
        }
        
        Int NodeBegin( const Int i ) const
        {
            if constexpr ( precompute_rangesQ )
            {
                return N_ranges.data()[2 * i + 0];
            }
            else
            {
                const Int regular_begin = RegularLeafNodeCount(i) * Column(i);
                
                return regular_begin - (Ramp(regular_begin - last_row_count) >> 1);
            }
        }
        
        Int NodeEnd( const Int i ) const
        {
            if constexpr ( precompute_rangesQ )
            {
                return N_ranges.data()[2 * i + 1];
            }
            else
            {
                const Int regular_end   = RegularLeafNodeCount(i) * (Column(i) + 1);
                
                return regular_end - (Ramp(regular_end - last_row_count) >> 1);
            }
        }
        
        std::pair<Int,Int> NodeRange( const Int i) const
        {
            
            if constexpr ( precompute_rangesQ )
            {
                return { N_ranges.data()[2 * i + 0], N_ranges.data()[2 * i + 1] };
            }
            else
            {
                const Int reg_leaf_count = RegularLeafNodeCount(i);
                const Int regular_begin  = reg_leaf_count * Column(i);
                const Int regular_end    = regular_begin + reg_leaf_count;
                
                return {
                    regular_begin - (Ramp(regular_begin - last_row_count) >> 1),
                    regular_end   - (Ramp(regular_end   - last_row_count) >> 1)
                };
            }
        }
        
        bool NodeContainsLeafNodeQ( const Int node, const Int leafnode ) const
        {
            auto [begin,end] = NodeRange(node);
            
            return (begin <= leafnode) && (leafnode < end);
        }
        
        bool InteriorNodeQ( const Int node ) const
        {
            return (node < int_node_count);
        }
        
        bool LeafNodeQ( const Int node ) const
        {
            return node >= int_node_count;
        }
        
        Int PrimitiveNode( const Int primitive ) const
        {
            if( primitive < last_row_count )
            {
                return last_row_begin + primitive;
            }
            else
            {
                return int_node_count + (primitive - last_row_count);
            }
        }
        
//###############################################################################
//##        Get functions
//###############################################################################

    public:

        Int NodeCount() const
        {
            return node_count;
        }
        
        Int InteriorNodeCount() const
        {
            return int_node_count;
        }
        
        Int LeafNodeCount() const
        {
            return leaf_node_count;
        }
        
        template<
            class Lambda_IntPreVisit,  class Lambda_IntPostVisit,
            class Lambda_LeafPreVisit, class Lambda_LeafPostVisit
        >
        void DepthFirstSearch(
            Lambda_IntPreVisit   int_pre_visit,  Lambda_IntPostVisit  int_post_visit,
            Lambda_LeafPreVisit  leaf_pre_visit, Lambda_LeafPostVisit leaf_post_visit,
            const Int start_node = -1
        )
        {
            ptic(ClassName()+"::DepthFirstSearch");
            
            Int stack [2 * max_depth];

            Int stack_ptr = Int(0);
            stack[stack_ptr] = (start_node < 0) ? Root() : start_node;
            
            while( (Int(0) <= stack_ptr) && (stack_ptr < Int(2) * max_depth - Int(2) ) )
            {
                const Int code = stack[stack_ptr];
                const Int node = (code >> 1);
                
                const bool visitedQ = (code & Int(1));
                
                if( !visitedQ  )
                {
                    auto [L,R] = Children(node);
                    
                    // We visit this node for the first time.
                    if( InteriorNodeQ(node) )
                    {
                        int_pre_visit(node);

                        // Mark node as visited.
                        stack[stack_ptr] = (code | Int(1)) ;
                        
                        stack[++stack_ptr] = (R << 1);
                        stack[++stack_ptr] = (L << 1);
                    }
                    else
                    {
                        // Things to be done when node is a leaf.
                        leaf_pre_visit(node);
                        
                        // There are no children to be visited first.
                        leaf_post_visit(node);
                        
                        // Popping current node from the stack.
                        --stack_ptr;
                    }
                }
                else
                {
                    // We visit this node for the second time.
                    // Thus node cannot be a leave node.
                    // We are moving in direction towards the root.
                    // Hence all children have already been visited.
                    
                    int_post_visit(node);

                    // Popping current node from the stack.
                    --stack_ptr;
                }
            }
            
            if( stack_ptr >= Int(2) * max_depth - Int(2) )
            {
                eprint(ClassName() + "::DepthFirstSearch: Stack overflow.");
            }
            
            ptoc(ClassName()+"::DepthFirstSearch");
        }
        
    public:
        
        static std::string ClassName()
        {
            return std::string("CompleteBinaryTree")+"<"+TypeName<Int>+">";
        }

    }; // CompleteBinaryTree
    
} // namespace KnotTools

