#pragma once

namespace KnotTools
{
    template<Size_T M_, Size_T N_, typename Real_, typename Int_>
    class alignas( ObjectAlignment ) ClangMatrix
    {
        static_assert(FloatQ<Real_>,"");
        static_assert(IntQ<Int_>,"");
        
    public:
        
        using Real = Real_;
        using Int  = Int_;
        
        static constexpr Int M = M_;
        static constexpr Int N = N_;
        
        using M_T = mat_T<M,N,Real>;
       
        ClangMatrix() = default;
        
        template<typename ExtReal>
        ClangMatrix( cptr<ExtReal> A_ptr )
        {
            Read(A_ptr);
        }

        ClangMatrix( const Real init )
        {
            fill_buffer<M*N>( reinterpret_cast<Real *>( &A ), init );
        }
        
        ClangMatrix( cref<M_T> B )
        :   A ( B )
        {}
        
        ~ClangMatrix() = default;

    private:
        
        M_T A;

    public:
        
        static constexpr Int Size()
        {
            return M * N;
        }
        
        static constexpr Int RowCount()
        {
            return M;
        }
        
        static constexpr Int ColCount()
        {
            return M;
        }
        
        
        Real operator()( const Int i, const Int j ) const
        {
            return A[i][j];
        }
        
        Real operator[]( const Int i ) const
        {
            return A[i][0];
        }
        
        void Set( const Int i, const Int j, const Real val )
        {
            A[i][j] = val;
        }
        
        void Read( cptr<Real> A_ptr )
        {
            
            A = __builtin_matrix_column_major_load( A_ptr, M, N, M );
        }
        
        void Write( mptr<Real> A_ptr ) const
        {
            __builtin_matrix_column_major_store( A, A_ptr, M );
        }
        
        cref<M_T> Matrix() const
        {
            return A;
        }
        
        mref<M_T> Matrix()
        {
            return A;
        }
        
        
//        template<Int K>
//        ClangMatrix<M,K,Real,Int> operator()( cref<ClangMatrix<N,K,Real,Int>> B ) const
//        {
//            return ClangMatrix<M,K,Real,Int>( A * B.Matrix() );
//        }
        
        template<Int K>
        void TransformMatrix( const Real * B_ptr, Real * C_ptr ) const
        {
            mat_T<N,K,Real> B = __builtin_matrix_column_major_load(B_ptr, N, K, N );
            
            mat_T<M,K,Real> C = A * B;
            
            __builtin_matrix_column_major_store( C, C_ptr, M );
        }
        
        
        void SetIdentity()
        {
            for( Int j = 0; j < N; ++ j )
            {
                for( Int i = 0; i < M; ++ i )
                {
                    A[i][j] = KroneckerDelta<Real>(i,j);
                }
            }
        }
        
        ClangMatrix & operator*=( cref<Real> a )
        {
            A *= a;
            
            return *this;
        }
        
        
        
        Real SquaredNorm() const
        {
            return norm_2_squared<M*N>( reinterpret_cast<const Real *>( &A ) );
        }
        
        Real Norm() const
        {
            return Sqrt(SquaredNorm());
        }
        
        ClangMatrix & Normalize()
        {
            return ((*this) *= Frac<Real>(1,Norm()));
        }
        
        
        friend ClangMatrix operator+(
            cref<ClangMatrix> A, const Real lambda
        )
        {
            return ClangMatrix( A.Matrix() + lambda );
        }
        
        friend ClangMatrix operator+(
            const Real lambda, cref<ClangMatrix> A
        )
        {
            return ClangMatrix( lambda + A.Matrix() );
        }
        
        
        friend ClangMatrix operator-(
            cref<ClangMatrix> A, const Real lambda
        )
        {
            return ClangMatrix( A.Matrix() - lambda );
        }
        
        friend ClangMatrix operator-(
            const Real lambda, cref<ClangMatrix> A
        )
        {
            return ClangMatrix( lambda - A.Matrix() );
        }

        friend ClangMatrix operator*(
            cref<ClangMatrix> A, const Real lambda
        )
        {
            return ClangMatrix( A.Matrix() * lambda );
        }
        
        friend ClangMatrix operator*(
            const Real lambda, cref<ClangMatrix> A
        )
        {
            return ClangMatrix( lambda * A.Matrix() );
        }


        
        friend ClangMatrix operator+( cref<ClangMatrix> B, cref<ClangMatrix> C )
        {
            return ClangMatrix( B.Matrix() + C.Matrix() );
        }
        
        friend ClangMatrix operator-( cref<ClangMatrix> B, cref<ClangMatrix> C )
        {
            return ClangMatrix( B.Matrix() - C.Matrix() );
        }
        
        template<Size_T K>
        friend ClangMatrix<M,K,Real,Int> operator*(
            cref<ClangMatrix> B, cref<ClangMatrix<N,K,Real,Int>> C
        )
        {
            return ClangMatrix<M,K,Real,Int>( B.Matrix() * C.Matrix() );
        }

        
        
        [[nodiscard]] friend std::string ToString( cref<ClangMatrix> A_ )
        {
            return ArrayToString( reinterpret_cast<const Real *>(&A_), {N,M} );
        }
        
    public:
        
        static std::string ClassName()
        {
            return std::string("ClangMatrix")
                + "<" + ToString(M)
                + "," + ToString(N)
                + "," + TypeName<Real>
                + "," + TypeName<Int>
                + ">";
        }

    }; // ClangMatrix
    
} // namespace KnotTools
