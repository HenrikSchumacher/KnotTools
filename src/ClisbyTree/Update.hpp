#include "UpdateSubtree_Recursive.hpp"
#include "UpdateSubtree_ManualStack.hpp"

public:

void Update( const Int pivot_p, const Int pivot_q, const Real angle_theta, const bool mirrorQ )
{
    int pivot_flag = LoadPivots( pivot_p, pivot_q, angle_theta, mirrorQ );
    
    if( pivot_flag == 0 ) [[likely]]
    {
        Update();
    }
}

private:

void Update()
{
    if constexpr ( manual_stackQ )
    {
        UpdateSubtree_ManualStack(Root());
    }
    else
    {
        UpdateSubtree_Recursive(Root());
    }
}

private:

void UndoUpdate()
{
    InvertPivotTransform();
    
    Update();
}

public:

UpdateFlag_T NodeNeedsUpdateQ( const Int node ) const
{
    return NodeNeedsUpdateQ( node, p, q, mid_changedQ );
}

UpdateFlag_T NodeNeedsUpdateQ(
    const Int node, const Int pivot_p, const Int pivot_q, const bool midQ
) const
{
    // Assuming that 0 <= pivot_p < pivot_q < LeafNodeCount();
    
    auto [begin,end] = NodeRange(node);
    
    const bool a =  midQ;
    const bool b = !midQ;
    
    const Int p_ = pivot_p + a;
    const Int q_ = pivot_q + b;
    
    if( (p_ <= begin) && (end <= q_) )
    {
        return UpdateFlag_T(a);
    }
    else if( (end <= p_) || (q_ <= begin) )
    {
        return UpdateFlag_T(b);
    }
    else
    {
        return UpdateFlag_T::Split;
    }
}

private:

template<bool allow_reflectionsQ>
int LoadPivots(
    const Int pivot_p, const Int pivot_q, const Real angle_theta, const bool mirror_bit
)
{
    p = Min(pivot_p,pivot_q);
    q = Max(pivot_p,pivot_q);
    theta = angle_theta;
    reflectQ = mirror_bit;
    
    const Int n = VertexCount() ;
    const Int mid_size = q - p - 1;
    const Int rem_size = n - mid_size - 2;
    
    if( (mid_size <= 0) || (rem_size <= 0) ) [[unlikely]]
    {
        return 1;
    }
    
    mid_changedQ = (mid_size <= rem_size);
    
    // TODO: There is maybe a more efficient way to compute the pivot vectors.
    X_p = VertexCoordinates(p);
    X_q = VertexCoordinates(q);
    ComputePivotTransform<allow_reflectionsQ>();
    
    return 0;
}
