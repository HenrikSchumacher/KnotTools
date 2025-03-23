#pragma once

namespace Knoodle
{
    template<int AmbDim_, typename Real_, typename Int_>
    class ClangAffineTransform
    {
        static_assert(FloatQ<Real_>,"");
        static_assert(IntQ<Int_>,"");
        
    public:
        
        using Real = Real_;
        using Int  = Int_;
        
        static constexpr Int AmbDim  = AmbDim_;
        
        using Flag_T   = AffineTransformFlag_T;
        using Vector_T = ClangMatrix<AmbDim,1       ,Real,Int>;
        using Matrix_T = ClangMatrix<AmbDim,AmbDim  ,Real,Int>;
       
        static constexpr Flag_T Id    = Flag_T::Id;
        static constexpr Flag_T NonId = Flag_T::NonId;
        
        ClangAffineTransform() = default;
        
        template<typename ExtReal>
        ClangAffineTransform(
            cptr<ExtReal> f_ptr, const Flag_T f_flag
        )
        {
            Read( f_ptr, f_flag );
        }
        
        template<typename ExtReal>
        ClangAffineTransform(
            cptr<ExtReal> A_ptr, cptr<ExtReal> b_ptr, const Flag_T f_flag
        )
        {
            Read( A_ptr, b_ptr, f_flag );
        }
        
        ClangAffineTransform(
            cref<Matrix_T> A_, cref<Vector_T> b_, const Flag_T flag_
        )
        {
            Read( A_, b_, flag_ );
        }
        
        ~ClangAffineTransform() = default;

    private:
        
        Matrix_T A;
        Vector_T b;
        Flag_T   flag = Id;
        

    public:
        
        static constexpr Int Size()
        {
            return AmbDim * AmbDim + AmbDim;
        }
        
        bool NontrivialQ() const
        {
            return flag == NonId;
        }
        
        bool IdentityQ() const
        {
            return flag == Id;
        }
        
        template<typename ExtReal>
        void ForceReadMatrix( cptr<ExtReal> A_ptr )
        {
            A.Read(A_ptr);
        }
        
        template<typename ExtReal>
        void ForceReadVector( cptr<ExtReal> b_ptr )
        {
            b.Read(b_ptr);
        }
        
        void ForceReadVector( cref<Vector_T> b_vec )
        {
            b = b_vec;
        }
        
        template<typename ExtReal>
        void ForceRead( cptr<ExtReal> f_ptr, const Flag_T f_flag )
        {
            flag = f_flag;
            ForceReadMatrix(f_ptr);
            ForceReadVector(&f_ptr[AmbDim*AmbDim]);
        }
        
        template<typename ExtReal>
        void Read( cptr<ExtReal> f_ptr, const Flag_T f_flag )
        {
            if( f_flag == NonId )
            {
                ForceRead( f_ptr, f_flag );
            }
            else
            {
                flag = f_flag;
            }
        }

        void ForceRead( cref<Matrix_T> A_, cref<Vector_T> b_, const Flag_T flag_ )
        {
            A = A_;
            b = b_;
            flag = flag_;
        }

        void Read( cref<Matrix_T> A_, cref<Vector_T> b_, const Flag_T flag_ )
        {
            if( flag_ == NonId )
            {
                ForceRead(A_,b_,flag_);
            }
            else
            {
                flag = flag_;
            }
        }
        
        
        template<typename ExtReal>
        void ForceWriteMatrix( mptr<ExtReal> A_ptr ) const
        {
            A.Write(A_ptr);
        }
        
        void ForceWriteMatrix( mref<Matrix_T> A_mat ) const
        {
            A_mat = A;
        }
        
        template<typename ExtReal>
        void ForceWriteVector( mptr<ExtReal> b_ptr ) const
        {
            b.Write(b_ptr);
        }
        
        void ForceWriteVector( mref<Vector_T> b_vec ) const
        {
            b_vec = b;
        }
        
        template<typename ExtReal>
        void ForceWrite( mptr<ExtReal> f_ptr, mref<Flag_T> f_flag ) const
        {
            f_flag = flag;
            ForceWriteMatrix(f_ptr);
            ForceWriteVector(&f_ptr[AmbDim*AmbDim]);
        }
        
        template<typename ExtReal>
        void Write( mptr<ExtReal> f_ptr, mref<Flag_T> f_flag ) const
        {
            if( NontrivialQ() )
            {
                ForceWrite( f_ptr, f_flag );
            }
            else
            {
                f_flag = Id;
            }
        }
        
        void ForceWrite( mref<Matrix_T> A_, mref<Vector_T> b_, mref<Flag_T> flag_ ) const
        {
            ForceWriteMatrix(A_);
            ForceWriteVector(b_);
            flag_ = flag;
        }
        
        void Write( mref<Matrix_T> A_, mref<Vector_T> b_, mref<Flag_T> flag_ )
        {
            if( NontrivialQ() )
            {
                ForceWrite( A_, b_, flag_ );
            }
            else
            {
                flag_ = Id;
            }
        }
        
        cref<Matrix_T> Matrix() const
        {
            return A;
        }
        
        cref<Vector_T> Vector() const
        {
            return b;
        }
        
        // In-place transformation of vector.
        bool TransformVector( mptr<Real> x_ptr ) const
        {
            if( NontrivialQ() )
            {
                Vector_T x ( x_ptr );
                Vector_T y = A * x + b;
                y.Write(x_ptr);
                
                return true;
            }
            else
            {
                return false;
            }
        }
        
        Vector_T operator()( cref<Vector_T> x ) const
        {
            if( NontrivialQ() )
            {
                return A * x + b;
            }
            else
            {
                return x;
            }
            
        }
        
        // In-place transformation of transform. Taking advantage of the fact that the quaternion is stored as 4-vector.
        bool TransformTransform( mptr<Real> f_ptr, mref<Flag_T> f_flag ) const
        {
            if( NontrivialQ() )
            {
                if( f_flag == NonId )
                {
                    // Fusing matrix-matrix and matrix-vector multiplication into a single matrix-matrix multiplication.
                    using M_T = ClangMatrix<AmbDim,AmbDim+1,Real,Int>;
                    
                    M_T B ( f_ptr );
                    M_T C = A * B;
                    for( Int i = 0; i < AmbDim; ++i )
                    {
                        C.Set( i, AmbDim, C(i,AmbDim) + b(i,0) );
                    }
                    C.Write(f_ptr);
                    
                    return true;
                }
                else
                {
                    Write( f_ptr, f_flag );
                    
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        
        ClangAffineTransform operator()( cref<ClangAffineTransform> f ) const
        {
            if( flag == NonId)
            {
                if( f.flag == NonId )
                {
                    return ClangAffineTransform( A * f.A, A * f.b + b, NonId );
                }
                else
                {
                    return *this;
                }
                
            }
            else
            {
                return f;
            }
            
        }
       
        void Reset()
        {
            flag = Id;
        }
        
        void SetIdentity()
        {
            A.SetIdentity();
            b.SetZero();
            flag = Id;
        }
        
        static constexpr ClangAffineTransform IdentityTransform()
        {
            ClangAffineTransform f;
            f.SetIdentity();
            return f;
        }

        
        [[nodiscard]] friend std::string ToString(
            cref<ClangAffineTransform> f,
            std::string line_prefix = std::string("")
        )
        {
            std::string line_prefix_2 = line_prefix + "\t";
            std::string line_prefix_3 = line_prefix + "\t\t";
            return std::string("<| ")
                +  "\n" + line_prefix_2 + "\"A\""    + " -> " + ToString(f.A, line_prefix_3)
                + ",\n" + line_prefix_2 + "\"b\""    + " -> " + ToString(f.b, line_prefix_3)
                + ",\n" + line_prefix_2 + "\"Flag\"" + " -> " + ToString(ToUnderlying(f.flag))
                +  "\n" + line_prefix + "|>";
        }
        
    public:
        
        static std::string ClassName()
        {
            return ct_string("ClangAffineTransform")
                + "<" + ToString(AmbDim)
                + "," + TypeName<Real>
                + "," + TypeName<Int>
                + ">";
        }

    }; // ClangAffineTransform
    
} // namespace Knoodle
