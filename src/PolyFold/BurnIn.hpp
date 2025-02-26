private:

template<Size_T t0, int my_verbosity>
void BurnIn()
{
    constexpr Size_T t1 = t0 + 1;
    constexpr Size_T t2 = t0 + 2;
    constexpr Size_T t3 = t0 + 3;
    
//    constexpr bool V0Q = true;
//    constexpr bool V1Q = my_verbosity >= 1;
    constexpr bool V2Q = my_verbosity >= 2;
    
    log << ",\n" + ct_tabs<t0> + "\"Burn-in\" -> <|" << std::flush;

    TimeInterval T_clisby;
    TimeInterval T_fold;
    TimeInterval T_write;
    TimeInterval T_dealloc;
    
    PRNG_FullState_T full_state;
    Size_T bytes;
    
    LInt attempt_count;
    LInt accept_count;
    
    TimeInterval T_burn_in (0);
    
    {
        T_clisby.Tic<V2Q>();
        Clisby_T T ( x.data(), n, radius, random_engine );
        T_clisby.Toc<V2Q>();
        
        full_state = T.RandomEngineFullState();
        
        T_fold.Tic<V2Q>();
        auto counts = T.FoldRandom(burn_in_accept_count);
        T_fold.Toc<V2Q>();
        
        attempt_count = counts.Total();
        accept_count = counts[0];
        
        total_attempt_count += attempt_count;
        total_accept_count += accept_count;
        
        burn_in_attempt_count = attempt_count;
        
        burn_in_accept_count = accept_count;
        
        T_write.Tic<V2Q>();
        T.WriteVertexCoordinates( x.data() );
        T_write.Toc<V2Q>();
        
        random_engine = T.GetRandomEngine();
        
        e_dev = T.MinMaxEdgeLengthDeviation( x.data() );
        
        bytes = T.AllocatedByteCount();
        
        
        if( force_deallocQ )
        {
            T_dealloc.Tic<V2Q>();
            T = Clisby_T();
            T_dealloc.Toc<V2Q>();
        }
    }
    
    T_burn_in.Toc();

    const double timing = T_burn_in.Duration();
    
    burn_in_time = timing ;
    
    kv<t1,0>("Time Elapsed", burn_in_time );
    kv<t1>("Attempted Steps", attempt_count );
    kv<t1>("Attempted Steps/Second", Frac<Real>(attempt_count,timing) );
    kv<t1>("Accepted Steps", accept_count );
    kv<t1>("Accepted Steps/Second", Frac<Real>(accept_count,timing) );
    kv<t1>("Acceptance Probability", Frac<Real>(accept_count,attempt_count) );
    
    log << ",\n" << ct_tabs<t1> << "\"Clisby Tree\" -> <|";
        kv<t2,0>("Byte Count", bytes );
    if constexpr ( V2Q )
    {
        log << ",\n" << ct_tabs<t2> << "\"PCG64\" -> <|";
            kv<t3,0>("Multiplier", full_state.multiplier);
            kv<t3>("Increment" , full_state.increment );
            kv<t3>("State"     , full_state.state     );
        log << "\n" << ct_tabs<t2> << "|>";
    }
    log << "\n" << ct_tabs<t1> << "|>";
    
    if constexpr ( V2Q )
    {
        log << ",\n" << ct_tabs<t1> << "\"Time Details\" -> <|";
            kv<t2,0>("Create Clisby Tree", T_clisby.Duration());
            kv<t2>("Fold", T_fold.Duration());
            kv<t2>("Write Vertex Coordinates", T_write.Duration());
            if( force_deallocQ )
            {
                kv<t2>("Deallocate Clisby Tree", T_dealloc.Duration());
            }
        log << "\n" << ct_tabs<t1> << "|>";
    }
    
    kv<t1>("(Smallest Edge Length)/(Prescribed Edge Length) - 1", e_dev.first );
    kv<t1>("(Greatest Edge Length)/(Prescribed Edge Length) - 1", e_dev.second );
    
    log << "\n" + ct_tabs<t0> + "|>";
    
} // BurnIn

