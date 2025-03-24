public:

// TODO: This is almost literally copied from ClisbyTree.
template<bool allow_reflectionsQ, bool check_overlapsQ = true>
FoldFlag_T FoldRandom()
{
    FoldFlagCounts_T counters;
    
    counters.SetZero();
    
    using unif_real = std::uniform_real_distribution<Real>;
    
    unif_real u_real (- Scalar::Pi<Real>,Scalar::Pi<Real> );
    
    const Real angle = u_real(random_engine);
    
    auto [i,j] = RandomPivots();
    
    bool mirror_bit = false;
    
    if constexpr ( allow_reflectionsQ )
    {
        using unif_bool = std::uniform_int_distribution<int>;

        mirror_bit = unif_bool(0,1)(random_engine);
    }
    
    return Fold<allow_reflectionsQ,check_overlapsQ>( i, j, angle, mirror_bit );
}


// TODO: This is almost literally copied from ClisbyTree.
template<bool allow_reflectionsQ, bool check_overlapsQ = true>
FoldFlag_T Fold( Int p_, Int q_, Real theta_, bool reflectQ_ )
{
//    print("Fold");

    Clear();
    
    int pivot_flag = LoadPivots<allow_reflectionsQ>( p_, q_, theta_, reflectQ_ );
    
//    TOOLS_DUMP(p);
//    TOOLS_DUMP(q);
//    TOOLS_DUMP(theta);
//    TOOLS_DUMP(reflectQ);
//    TOOLS_DUMP(mid_changedQ);
    
    if constexpr ( check_overlapsQ )
    {
        if( pivot_flag != 0 )
        {
            // Folding step aborted because pivots indices are too close.
            return pivot_flag;
        }
        
        int joint_flag = CheckJoints();
        
        if( joint_flag != 0 )
        {
            // Folding step failed because neighbors of pivot touch.
            return joint_flag;
        }
    }
    
    Update();

    if constexpr ( check_overlapsQ )
    {
        if( OverlapQ() )
        {
            // Folding step failed.
            return 4;
        }
        else
        {
            // Folding step succeeded.
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

// TODO: This is almost literally copied from ClisbyTree.
std::pair<Int,Int> RandomPivots()
{
    const Int n = T.VertexCount();
    
    using unif_int = std::uniform_int_distribution<Int>;
    
    unif_int u_int ( Int(0), n - Int(1) );
    
    Int i = u_int(random_engine);
    Int j = u_int(random_engine);

    while( ModDistance(n,i,j) <= Int(1) )
    {
        i = u_int(random_engine);
        j = u_int(random_engine);
    }
    
    return MinMax(i,j);
}

private:

// TODO: This is almost literally copied from ClisbyTree.
template<bool allow_reflectionsQ>
int LoadPivots( const Int p_, const Int q_, const Real theta_, const bool reflectQ_ )
{
//        print("LoadPivots");
    p = Min(p_,q_);
    q = Max(p_,q_);
    theta = theta_;
    reflectQ = reflectQ_;
    
//        TOOLS_DUMP(theta);
//        TOOLS_DUMP(reflectQ);
    
    const Int n = T.VertexCount() ;
    const Int mid_size = q - p - 1;
    const Int rem_size = n - mid_size - 2;
    
    if( (mid_size <= 0) || (rem_size <= 0) ) [[unlikely]]
    {
        return 1;
    }
    
    mid_changedQ = (mid_size <= rem_size);
    
    // TODO: There is maybe a more efficient way to compute the pivot vectors.
    X_p = T.VertexCoordinates(p);
    X_q = T.VertexCoordinates(q);
    transform = PivotTransform<allow_reflectionsQ>( X_p, X_q, theta, reflectQ );
    
    return 0;
}
