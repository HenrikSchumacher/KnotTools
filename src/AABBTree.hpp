#pragma once

namespace KnotTools
{
    
    // A simple bounding volume hiearchy with static ordering.
    // This is specifically written for edge and triangle primitives for curves.
    // It won't work with more general clouds of primitives.
    
    template<int AmbDim_, typename Real_, typename Int_>
    class alignas( ObjectAlignment ) AABBTree : public CompleteBinaryTree<Int_>
    {
        static_assert(FloatQ<Real_>,"");
        static_assert(IntQ<Int_>,"");
        
    public:
        
        using Real = Real_;
        using Int  = Int_;
        
        using Tree_T = CompleteBinaryTree<Int>;
        using Tree_T::max_depth;

        
        static constexpr Int AmbDim = AmbDim_;
        
        using Vector_T     = Tiny::Vector<AmbDim_,Real,Int>;
        
        using BContainer_T = Tensor3<Real,Int>;
        
        using EContainer_T = Tensor3<Real,Int>;
        
        using PContainer_T = Tensor3<Real,Int>;
        
        using UInt = Scalar::Unsigned<Int>;
        
        AABBTree() = default;
        
        explicit AABBTree( const Int prim_count_  )
        :   Tree_T  ( prim_count_ )
        {}
        
        ~AABBTree() = default;
        
        
    protected:
        
        // Integer data for the combinatorics of the tree.
        
        using Tree_T::leaf_node_count;
        using Tree_T::node_count;
        using Tree_T::int_node_count;
        using Tree_T::last_row_begin;
        using Tree_T::offset;
        
    public:
        
        using Tree_T::MaxDepth;
        using Tree_T::NodeCount;
        using Tree_T::InteriorNodeCount;
        using Tree_T::LeafNodeCount;
        
        using Tree_T::RightChild;
        using Tree_T::LeftChild;
        using Tree_T::Children;
        using Tree_T::Parent;
        using Tree_T::Depth;
        using Tree_T::Column;
        using Tree_T::NodeBegin;
        using Tree_T::NodeEnd;
        
    private:
        
        /*!
         * @brief Computes the bounding boxes from the coordinates of a list of primitives. Primitives are assumed to be the convex hull of finitely many points. Only the leading `AmbDim` entries of each point are used.
         *
         * @tparam point_count Number of points in the primitive.
         *
         * @tparam dimP `dimP` is assumed to have dimensions `prim_count x point_count x dimP`. Here we require that `dimP >= AmbDim`. This sound off, but our main use case is creating planar diagrams; here `dimP = 3` and `AmbDim = 2`.
         *
         * @param P Array that represent a single primitive. It is assumed to have size `point_count x dimP`.
         *
         * @param B Represents a single box. It is assumed to be of dimensions `AmbDim x 2`.
         */
        
        template<Int point_count, Int dimP>
        static constexpr void PrimitiveToBox(
            cptr<Real> P, mptr<Real> B
        )
        {
            static_assert(point_count > 0, "");
            
            columnwise_minmax<point_count,AmbDim>( P, dimP, &B[0], Size_T(2), &B[1], Size_T(2) );
        }
        
        static constexpr void BoxesToBox(
            cptr<Real> B_L, cptr<Real> B_R, mptr<Real> B_N
        )
        {
            // TODO: Can this be vectorized?
            
            for( Int k = 0; k < AmbDim; ++k )
            {
                B_N[2*k+0] = Min( B_L[2*k+0], B_R[2*k+0] );
                B_N[2*k+1] = Max( B_L[2*k+1], B_R[2*k+1] );
            }
        }
        
    public:
        
        /*!
         * @brief Computes the bounding boxes of the whole tree from the coordinates of a list of primitives. Primitives are assumed to be the convex hull of finitely many points.
         *
         * @tparam point_count Number of points in the primitive.
         *
         * @tparam dimP `dimP` is assumed to have dimensions `prim_count x point_count x dimP`. Here we require that `dimP >= AmbDim`. This sound off, but our main use case is creating planar diagrams; here `dimP = 3` and `AmbDim = 2`.
         *
         * @param P Array that represents the primitive coordinates. It is assumed to have size `prim_count x point_count x dimP`.
         *
         * @param B Represents the container for the boxes. It is assumed to be a `Tensor3` of dimensions `prim_count x AmbDim x 2`.
         */
        
        template<Int point_count, Int dimP>
        void ComputeBoundingBoxes( cptr<Real> P, mref<BContainer_T> B ) const
        {
            ptic(ClassName()+"::ComputeBoundingBoxes");
            
            static_assert(dimP >= AmbDim,"");
            
            constexpr Int sizeP = point_count * dimP;

            ptic("Compute bounding boxes of leave nodes.");
            // Compute bounding boxes of leave nodes (last row of tree).
            for( Int N = last_row_begin; N < node_count; ++N )
            {
                const Int i = N - last_row_begin;
                
                PrimitiveToBox<point_count,dimP>( &P[sizeP * i], B.data(N) );
            }
            
            // Compute bounding boxes of leave nodes (penultimate row of tree).
            for( Int N = int_node_count; N < last_row_begin; ++N )
            {
                const Int i = N + offset;

                PrimitiveToBox<point_count,dimP>( &P[sizeP * i], B.data(N) );
            }
            ptoc("Compute bounding boxes of leave nodes.");
            
            ptic("Compute bounding boxes of interior nodes.");
            // Compute bounding boxes of interior nodes.
            for( Int N = int_node_count; N --> 0;  )
            {
                const auto [L,R] = Children(N);
                
                BoxesToBox( B.data(L), B.data(R), B.data(N) );
            }
            ptoc("Compute bounding boxes of interior nodes.");
            
            ptoc(ClassName()+"::ComputeBoundingBoxes");
        }
        
        template<Int point_count, Int dimP>
        void ComputeBoundingBoxes( cref<EContainer_T> E, mref<BContainer_T> B )
        {
            ComputeBoundingBoxes<point_count,dimP>( E.data(), B );
        }
        
        static constexpr bool BoxesIntersectQ( const cptr<Real> B_i, const cptr<Real> B_j )
        {
            // B_i and B_j are assumed to have size AmbDim x 2.
            
//            if constexpr ( VectorizableQ<Real> )
//            {
//                vec_T<2*AmbDim,Real> a;
//                vec_T<2*AmbDim,Real> b;
//                
//                for( Int k = 0; k < AmbDim; ++k )
//                {
//                    a[2 * k + 0] = B_i[2 * k + 0];
//                    a[2 * k + 1] = B_j[2 * k + 0];
//                    b[2 * k + 0] = B_j[2 * k + 1];
//                    b[2 * k + 1] = B_i[2 * k + 1];
//                }
//                
//                return __builtin_reduce_and( a <= b );
//            }
//            else
            {
                for( Int k = 0; k < AmbDim; ++k )
                {
                    if(
                       (B_i[2 * k + 0] > B_j[2 * k + 1])
                       ||
                       (B_j[2 * k + 0] > B_i[2 * k + 1])
                    )
                    {
                        return false;
                    }
                }
                
                return true;
            }
        }
        
        static constexpr Real BoxBoxSquaredDistance(
            const cptr<Real> B_i, const cptr<Real> B_j
        )
        {
            Real d2 = 0;
            
            for( Int k = 0; k < AmbDim; ++k )
            {
                const Real x = Ramp(
                    Max( B_i[2 * k + 0], B_j[2 * k + 0] )
                    -
                    Min( B_i[2 * k + 1], B_j[2 * k + 1] )
                );
                
                d2 += x * x;
            }
            return d2;
        }
        
        static constexpr Real BoxBoxSquaredDistance(
            cref<BContainer_T> B_0, const Int i, cref<BContainer_T> B_1, const Int j
        )
        {
            return BoxBoxSquaredDistance(
                &B_0.data()[AmbDim * 2 * i], &B_1.data()[AmbDim * 2 * j]
            );
        }
        
        BContainer_T AllocateBoxes()
        {
            ptic(ClassName()+"::AllocateBoxes");
            
            BContainer_T B (NodeCount(),AmbDim,2);
            
            ptoc(ClassName()+"::AllocateBoxes");
            
            return B;
        }
        
    public:
        
        static std::string ClassName()
        {
            return std::string("AABBTree")
                + "<" + ToString(AmbDim)
                + "," + TypeName<Real>
                + "," + TypeName<Int>
                + ">";
        }

    }; // AABBTree
    
} // namespace KnotTools
