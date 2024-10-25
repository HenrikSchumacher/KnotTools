#pragma  once

#include <unordered_set>

namespace KnotTools
{
    // TODO: Use signs in A_cross entries to indicate whether the arc enters/leaves the corresponding crossing through left or right port, i.e.,
    // TODO: A_cross(a,Tail) = -c if a == C_arc(a,Out,Left )
    // TODO: A_cross(a,Tail) =  c if a == C_arc(a,Out,Right)
    
    
    template<typename Int_, Size_T optimization_level, bool use_flagsQ_, bool mult_compQ_>
    class ArcSimplifier;
    
    template<typename Int_, bool mult_compQ_> class CrossingSimplifier;
    
    template<typename Int_, bool mult_compQ_> class StrandSimplifier;
    
    template<typename Int_>
    class alignas( ObjectAlignment ) PlanarDiagram : public CachedObject
    {
    public:
        
        static_assert(SignedIntQ<Int_>,"");

        using Int  = Int_;
        using Sint = int;
        
        using UInt = Scalar::Unsigned<Int>;
        
        using Base_T  = CachedObject;
        using Class_T = PlanarDiagram<Int>;
        
        using CrossingContainer_T       = Tensor3<Int,Int>;
        using ArcContainer_T            = Tensor2<Int,Int>;
        using CrossingStateContainer_T  = Tensor1<CrossingState,Int>;
        using ArcStateContainer_T       = Tensor1<ArcState,Int>;
        
        

        template<typename I, Size_T lvl,bool use_flagsQ_, bool mult_compQ_>
        friend class ArcSimplifier;
        
        template<typename I, bool mult_compQ_>
        friend class CrossingSimplifier;
        
        template<typename I, bool mult_compQ_>
        friend class StrandSimplifier;
        
        
        using Arrow_T = std::pair<Int,bool>;
        
        static constexpr bool Tail  = 0;
        static constexpr bool Head  = 1;
        static constexpr bool Left  = 0;
        static constexpr bool Right = 1;
        static constexpr bool Out   = 0;
        static constexpr bool In    = 1;
        
        
    protected:
        
        // Class data members
        
        Int crossing_count          = 0;
        Int arc_count               = 0;
        Int unlink_count            = 0;
        Int initial_crossing_count  = 0;
        Int initial_arc_count       = 0;
        
        // Exposed to user via Crossings().
        CrossingContainer_T      C_arcs;
        // Exposed to user via CrossingStates().
        CrossingStateContainer_T C_state;
        
        // Exposed to user via Arcs().
        ArcContainer_T           A_cross;
        // Exposed to user via ArcStates().
        ArcStateContainer_T      A_state;
        
        // Counters for Reidemeister moves.
        Int R_I_counter             = 0;
        Int R_Ia_counter            = 0;
        Int R_II_counter            = 0;
        Int R_IIa_counter           = 0;
        Int twist_counter           = 0;
        Int four_counter            = 0;
        
//        Tensor1<Int,Int> comp_arc_idx {0};
//        Tensor1<Int,Int> comp_arc_ptr {2,0};
//        
//        Tensor1<Int,Int> A_comp {0};
//        Tensor1<Int,Int> A_pos  {0};
        
//        bool comp_initialized  = false;
        
        Tensor1<Int,Int> C_scratch; // Some multi-purpose scratch buffers.
        Tensor1<Int,Int> A_scratch; // Some multi-purpose scratch buffers.

        Stack<Int,Int> stack;
        
    public:
        
        PlanarDiagram() = default;
        
        virtual ~PlanarDiagram() override = default;
        
        
    protected:
        
        /*! @brief This constructor is supposed to only allocate all relevant buffers.
         *  Data has to be filled in manually. Only for internal use.
         */
        
        PlanarDiagram( const Int crossing_count_, const Int unlink_count_ )
        :   crossing_count          ( crossing_count_                         )
        ,   arc_count               ( Int(2)*crossing_count                   )
        ,   unlink_count            ( unlink_count_                           )
        ,   initial_crossing_count  ( crossing_count                          )
        ,   initial_arc_count       ( arc_count                               )
        ,   C_arcs                  ( crossing_count, Int(2), Int(2), -1      )
        ,   C_state                 ( crossing_count, CrossingState::Inactive )
        ,   A_cross                 ( arc_count,      Int(2), -1              )
        ,   A_state                 ( arc_count,      ArcState::Inactive      )
        ,   C_scratch               ( crossing_count                          )
        ,   A_scratch               ( arc_count                               )
        {}
        
    public:
  
//        // Copy constructor
//        PlanarDiagram( const PlanarDiagram & other ) = default;
        
        // Copy constructor
        PlanarDiagram( const PlanarDiagram & other )
        :   crossing_count          { other.crossing_count          }
        ,   arc_count               { other.arc_count               }
        ,   unlink_count            { other.unlink_count            }
        ,   initial_crossing_count  { other.initial_crossing_count  }
        ,   initial_arc_count       { other.initial_arc_count       }
        ,   C_arcs                  { other.C_arcs                  }
        ,   C_state                 { other.C_state                 }
        ,   A_cross                 { other.A_cross                 }
        ,   A_state                 { other.A_state                 }
        ,   R_I_counter             { other.R_I_counter             }
        ,   R_Ia_counter            { other.R_Ia_counter            }
        ,   R_II_counter            { other.R_II_counter            }
        ,   R_IIa_counter           { other.R_IIa_counter           }
        ,   twist_counter           { other.twist_counter           }
        ,   four_counter            { other.four_counter            }

//        ,   comp_arc_idx            { other.comp_arc_idx            }
//        ,   comp_arc_ptr            { other.comp_arc_ptr            }
//        ,   A_comp                  { other.A_comp                  }
//        ,   A_pos                   { other.A_pos                   }
//        ,   comp_initialized        { other.comp_initialized        }
        ,   C_scratch               { other.C_scratch               }
        ,   A_scratch               { other.A_scratch               }
        {}
            
        
        friend void swap(PlanarDiagram & A, PlanarDiagram & B ) noexcept
        {
            // see https://stackoverflow.com/questions/5695548/public-friend-swap-member-function for details
            using std::swap;
            
            swap( static_cast<CachedObject &>(A), static_cast<CachedObject &>(B) );
            
            swap( A.crossing_count         , B.crossing_count          );
            swap( A.arc_count              , B.arc_count               );
            swap( A.unlink_count           , B.unlink_count            );
            swap( A.initial_crossing_count , B.initial_crossing_count  );
            swap( A.initial_arc_count      , B.initial_arc_count       );
            
            swap( A.C_arcs                 , B.C_arcs                  );
            swap( A.C_state                , B.C_state                 );
            swap( A.A_cross                , B.A_cross                 );
            swap( A.A_state                , B.A_state                 );
            
            swap( A.R_I_counter            , B.R_I_counter             );
            swap( A.R_Ia_counter           , B.R_Ia_counter            );
            swap( A.R_II_counter           , B.R_II_counter            );
            swap( A.R_IIa_counter          , B.R_IIa_counter           );
            swap( A.twist_counter          , B.twist_counter           );
            swap( A.four_counter           , B.four_counter            );

//            swap( A.comp_arc_idx           , B.comp_arc_idx            );
//            swap( A.comp_arc_ptr           , B.comp_arc_ptr            );
//            swap( A.A_comp                 , B.A_comp                  );
//            swap( A.A_pos                  , B.A_pos                   );
//            
//            swap( A.comp_initialized       , B.comp_initialized        );
            
            swap( A.C_scratch              , B.C_scratch               );
            swap( A.A_scratch              , B.A_scratch               );
        }
        
        // Move constructor
        PlanarDiagram( PlanarDiagram && other ) noexcept
        :   PlanarDiagram()
        {
            swap(*this, other);
        }

        /* Copy assignment operator */
        PlanarDiagram & operator=( PlanarDiagram other ) noexcept
        {   //                                     ^
            //                                     |
            // Use the copy constructor     -------+
            swap( *this, other );
            return *this;
        }
        
        
        /*! @brief Construction from `Link_2D` object.
         */
        
        template<typename Real, typename SInt>
        PlanarDiagram( cref<Link_2D<Real,Int,SInt>> L )
        :   PlanarDiagram( L.CrossingCount(), L.UnlinkCount() )
        {
            using Link_T         = Link_2D<Real,Int>;
            using Intersection_T = Link_T::Intersection_T;

            const Int component_count     = L.ComponentCount();
            
            cptr<Int>  component_ptr      = L.ComponentPointers().data();
            cptr<Int>  edge_ptr           = L.EdgePointers().data();
            cptr<Int>  edge_intersections = L.EdgeIntersections().data();
            cptr<bool> edge_overQ         = L.EdgeOverQ().data();
            
            cref<std::vector<Intersection_T>> intersections = L.Intersections();
            
            // Now we go through all components
            //      then through all edges of the component
            //      then through all intersections of the edge
            // and generate new vertices, edges, crossings, and arcs in one go.
            
            for( Int comp = 0; comp < component_count; ++comp )
            {
                // The range of arcs belonging to this component.
                const Int arc_begin = edge_ptr[component_ptr[comp  ]];
                const Int arc_end   = edge_ptr[component_ptr[comp+1]];

                if( arc_begin == arc_end )
                {
                    // Component is an unlink. Just skip it.
                    continue;
                }
                
                // If we arrive here, then there is definitely a crossing in the first edge.

                for( Int b = arc_begin, a = arc_end-1; b < arc_end; a = (b++) )
                {
                    const Int c = edge_intersections[b];
                    
                    const bool overQ = edge_overQ[b];
                    
                    cref<Intersection_T> inter = intersections[c];
                    
                    A_cross(a,Head) = c; // c is head of a
                    A_cross(b,Tail) = c; // c is tail of b
                    
                    PD_ASSERT( (inter.handedness > SInt(0)) || (inter.handedness < SInt(0)) );
                    
                    bool righthandedQ = inter.handedness > SInt(0);

                    
                    /*
                     *
                     *    negative         positive
                     *    right-handed     left-handed
                     *    .       .        .       .
                     *    .       .        .       .
                     *    +       +        +       +
                     *     ^     ^          ^     ^
                     *      \   /            \   /
                     *       \ /              \ /
                     *        /                \
                     *       / \              / \
                     *      /   \            /   \
                     *     /     \          /     \
                     *    +       +        +       +
                     *    .       .        .       .
                     *    .       .        .       .
                     *
                     */
                    
                    C_state[c] = righthandedQ ? CrossingState::RightHanded : CrossingState::LeftHanded;
                    A_state[a] = ArcState::Active;
                    
                    /*
                    * righthandedQ == true and overQ == true:
                    *
                    *        C_arcs(c,Out,Left)  .       .  C_arcs(c,Out,Right) = b
                    *                            .       .
                    *                            +       +
                    *                             ^     ^
                    *                              \   /
                    *                               \ /
                    *                                /
                    *                               / \
                    *                              /   \
                    *                             /     \
                    *                            +       +
                    *                            .       .
                    *     a = C_arcs(c,In,Left)  .       .  C_arcs(c,In,Right)
                    */
                    
                    const bool over_in_side = (righthandedQ == overQ) ? Left : Right ;
                    
                    C_arcs(c,In , over_in_side) = a;
                    C_arcs(c,Out,!over_in_side) = b;
                }
            }
        }
        
        
        template<typename ExtInt>
        PlanarDiagram(
            cptr<ExtInt> crossings, cptr<ExtInt> crossing_states,
            cptr<ExtInt> arcs     , cptr<ExtInt> arc_states,
            const ExtInt crossing_count_, const ExtInt unlink_count_
        )
        :   PlanarDiagram( crossing_count_, unlink_count_ )
        {
            C_arcs.Read(crossings);
            A_cross.Read(arcs);
            
            for( Int c = 0; c < initial_crossing_count; ++c )
            {
                C_state[c] = static_cast<CrossingState>(crossing_states[c]);
            }
            
            for( Int a = 0; a < initial_arc_count; ++a )
            {
                A_state[a] = static_cast<ArcState>(arc_states[a]);
            }
        }
        
        
        /*!
         * @brief Returns the number of trivial unlinks in the diagram, i.e., unknots that do not share any crossings with other link components.
         */
        
        Int UnlinkCount() const
        {
            return unlink_count;
        }
        
        // Crossings

        /*!
         * @brief Returns how many crossings there were in the original planar diagram, before any simplifications.
         */
        
        Int InitialCrossingCount() const
        {
            return initial_crossing_count;
        }
        
        /*!
         * @brief The number of crossings in the planar diagram.
         */
        
        Int CrossingCount() const
        {
            return crossing_count;
        }
        
        /*!
         * @brief Returns how many Reidemeister I moves have been performed so far.
         */
        
        Int Reidemeister_I_Counter() const
        {
            return R_I_counter;
        }
        
        /*!
         * @brief Returns how many Reidemeister Ia moves have been performed so far.
         */
        
        Int Reidemeister_Ia_Counter() const
        {
            return R_Ia_counter;
        }
        
        /*!
         * @brief Returns how many Reidemeister II moves have been performed so far.
         */
        
        Int Reidemeister_II_Counter() const
        {
            return R_II_counter;
        }
        
        /*!
         * @brief Returns how many Reidemeister IIa moves have been performed so far.
         */
        
        Int Reidemeister_IIa_Counter() const
        {
            return R_IIa_counter;
        }
        
        /*!
         * @brief Returns how many twist moves have been performed so far.
         *
         * See TwistMove.hpp for details.
         */
        
        Int TwistMove_Counter() const
        {
            return twist_counter;
        }
        
        /*!
         * @brief Returns how many moves that remove 4 crossings at once have been performed so far.
         */
        
        Int FourMove_Counter() const
        {
            return four_counter;
        }
        
        /*!
         * @brief Returns the list of crossings in the internally used storage format as a reference to a `Tensor3` object, which is basically a heap-allocated array of dimension 3.
         *
         * The reference is constant because things can go wild  (segfaults, infinite loops) if we allow the user to mess with this data.
         *
         * The `c`-th crossing is stored in `Crossings()(c,i,j)`, `i = 0,1`, `j = 0,1` as follows; note that we defined Booleans `Out = 0`, `In = 0`, `Left = 0`, `Right = 0` for easing the indexing:
         *
         *    Crossings()(c,0,0)                   Crossings()(c,0,1)
         *            =              O       O             =
         * Crossings()(c,Out,Left)    ^     ^   Crossings()(c,Out,Right)
         *                             \   /
         *                              \ /
         *                               X
         *                              / \
         *                             /   \
         *    Crossings()(c,1,0)      /     \      Crossings()(c,1,1)
         *            =              O       O             =
         *  Crossings()(c,In,Left)               Crossings()(c,In,Right)
         *
         *  Beware that a crossing can have various states, such as `CrossingState::LeftHanded`, `CrossingState::RightHanded`, or `CrossingState::Deactivated`. This information is stored in the corresponding entry of `CrossingStates()`.
         */
        
        cref<CrossingContainer_T> Crossings() const
        {
            return C_arcs;
        }
        
        /*!
         * @brief Returns the states of the crossings.
         *
         *  The states that a crossing can have are:
         *
         *  - `CrossingState::RightHanded`
         *  - `CrossingState::RightHandedUnchanged`
         *  - `CrossingState::LeftHanded`
         *  - `CrossingState::LeftHandedUnchanged`
         *  - `CrossingState::Inactive`
         *
         * `CrossingState::Inactive` means that the crossing has been deactivated by topological manipulations.
         */
        
        cref<CrossingStateContainer_T> CrossingStates() const
        {
            return C_state;
        }
        
        
        /*!
         * @brief Returns how many arcs there were in the original planar diagram, before any simplifications.
         */
        
        Int InitialArcCount() const
        {
            return initial_arc_count;
        }
        
        /*!
         * @brief Returns the number of arcs in the planar diagram.
         */
        
        Int ArcCount() const
        {
            return arc_count;
        }
        
        /*!
         * @brief Returns the arcs that connect the crossings as a reference to a constant `Tensor2` object, which is basically a heap-allocated matrix.
         *
         * This reference is constant because things can go wild (segfaults, infinite loops) if we allow the user to mess with this data.
         *
         * The `a`-th arc is stored in `Arcs()(a,i)`, `i = 0,1`, in the following way; note that we defined Booleans `Tail = 0` and `Head = 1` for easing the indexing:
         *
         *                          a
         *    Arcs()(a,0) X -------------> X Arcs()(a,0)
         *          =                             =
         *    Arcs()(a,Tail)                Arcs()(a,Head)
         *
         * This means that arc `a` leaves crossing `GetArc()(a,0)` and enters `GetArc()(a,1)`.
         *
         * Beware that an arc can have various states such as `CrossingState::Active` or `CrossingState::Deactivated`. This information is stored in the corresponding entry of `ArcStates()`.
         */
        
        cref<ArcContainer_T> Arcs()
        {
            return A_cross;
        }
        
        /*!
         * @brief Returns the arcs that connect the crossings as a reference to a Tensor2 object.
         *
         * The `a`-th arc is stored in `Arcs()(a,i)`, `i = 0,1`, in the following way:
         *
         *                          a
         *       Arcs()(a,0) X -------------> X Arcs()(a,0)
         *            =                              =
         *      Arcs()(a,Tail)                 Arcs()(a,Head)
         *
         * This means that arc `a` leaves crossing `GetArc()(a,0)` and enters `GetArc()(a,1)`.
         */
        
        cref<ArcContainer_T> Arcs() const
        {
            return A_cross;
        }
        
        /*!
         * @brief Returns the states of the arcs.
         *
         *  The states that an arc can have are:
         *
         *  - `ArcState::Active`
         *  - `ArcState::Inactive`
         *
         * `CrossingState::Inactive` means that the arc has been deactivated by topological manipulations.
         */
        
        cref<ArcStateContainer_T> ArcStates() const
        {
            return A_state;
        }
        
        
        void SanitizeArcStates()
        {
            for( Int a = 0; a < initial_arc_count; ++a )
            {
                if( ArcActiveQ(a) )
                {
                    A_state[a] = ArcState::Active;
                }
            }
        }

        
    public:
        
        std::string CrossingString( const Int c ) const
        {
            return "crossing " + Tools::ToString(c) +" = { { "
                + Tools::ToString(C_arcs(c,Out,Left )) + ", "
                + Tools::ToString(C_arcs(c,Out,Right)) + " }, { "
                + Tools::ToString(C_arcs(c,In ,Left )) + ", "
                + Tools::ToString(C_arcs(c,In ,Right)) + " } } ("
                + KnotTools::ToString(C_state[c])      +")";
        }
        
        /*!
         * @brief Checks whether crossing `c` is still active.
         */
        
        bool CrossingActiveQ( const Int c ) const
        {
            return KnotTools::ActiveQ(C_state[c]);
        }
        
        bool CrossingUnchangedQ( const Int c ) const
        {
            return KnotTools::UnchangedQ(C_state[c]);
        }
        
        bool CrossingChangedQ( const Int c ) const
        {
            return KnotTools::ChangedQ(C_state[c]);
        }
        
        bool CrossingRightHandedQ( const Int c ) const
        {
            return KnotTools::RightHandedQ(C_state[c]);
        }
        
        bool CrossingLeftHandedQ( const Int c ) const
        {
            return KnotTools::LeftHandedQ(C_state[c]);
        }

        bool OppositeHandednessQ( const Int c_0, const Int c_1 ) const
        {
            return KnotTools::OppositeHandednessQ(C_state[c_0],C_state[c_1]);
        }
        
        bool SameHandednessQ( const Int c_0, const Int c_1 ) const
        {
            return KnotTools::SameHandednessQ(C_state[c_0],C_state[c_1]);
        }
        
        void RotateCrossing( const Int c, const bool dir )
        {
//            Int C [2][2];
//            copy_buffer<4>( C_arcs.data(c), &C[0][0] );
            
        // Before:
        //
        //   C[Out][Left ] O       O C[Out][Right]
        //                  ^     ^
        //                   \   /
        //                    \ /
        //                     X
        //                    / \
        //                   /   \
        //                  /     \
        //   C[In ][Left ] O       O C[In ][Right]
            
            if( dir == Right )
            {
        // After:
        //
        //   C[Out][Left ] O       O C[Out][Right]
        //                  \     ^
        //                   \   /
        //                    \ /
        //                     X
        //                    / \
        //                   /   \
        //                  /     v
        //   C[In ][Left ] O       O C[In ][Right]

                const Int buffer = C_arcs(c,Out,Left );
                
                C_arcs(c,Out,Left ) = C_arcs(c,Out,Right);
                C_arcs(c,Out,Right) = C_arcs(c,In ,Right);
                C_arcs(c,In ,Right) = C_arcs(c,In ,Left );
                C_arcs(c,In ,Left ) = buffer;
            }
            else
            {
        // After:
        //
        //   C[Out][Left ] O       O C[Out][Right]
        //                  ^     /
        //                   \   /
        //                    \ /
        //                     X
        //                    / \
        //                   /   \
        //                  v     \
        //   C[In ][Left ] O       O C[In ][Right]
                
                const Int buffer = C_arcs(c,Out,Left );
                
                C_arcs(c,Out,Left ) = C_arcs(c,In ,Left );
                C_arcs(c,In ,Left ) = C_arcs(c,In ,Right);
                C_arcs(c,In ,Right) = C_arcs(c,Out,Right);
                C_arcs(c,Out,Right) = buffer;
            }
        }
        
        std::string ArcString( const Int a ) const
        {
            return "arc " +Tools::ToString(a) +" = { "
                + Tools::ToString(A_cross(a,Tail)) + ", "
                + Tools::ToString(A_cross(a,Head)) + " } ("
                + KnotTools::ToString(A_state[a]) + ")";
        }
        
        /*!
         * @brief Checks whether arc `a` is still active.
         */
        
        bool ArcActiveQ( const Int a ) const
        {
            return KnotTools::ActiveQ(A_state[a]);
        }
        
        bool ArcUnchangedQ( const Int a ) const
        {
            return KnotTools::UnchangedQ(A_state[a]);
        }
        
        bool ArcChangedQ( const Int a ) const
        {
            return KnotTools::ChangedQ(A_state[a]);
        }
        
        /*!
         * @brief Returns the crossing you would get to by starting at crossing `c` and
         * leaving trough the
         */
        
        Int NextCrossing( const Int c, bool io, bool side ) const
        {
            AssertCrossing(c);

            const Int a = C_arcs(c,io,side);

            AssertArc(a);
            
//            const Int c_next = A_cross( a, ( io == Out ) ? Head : Tail );
            const Int c_next = A_cross( a, !io );

            AssertCrossing(c_next);
            
            return c_next;
        }

        
    private:
        
        /*!
         * @brief Deactivates crossing `c`. Only for internal use.
         */
        
        template<bool assertsQ = true>
        void DeactivateCrossing( const Int c )
        {
            if( CrossingActiveQ(c) )
            {
                PD_PRINT("Deactivating " + CrossingString(c) + "." );
                --crossing_count;
                C_state[c] = CrossingState::Inactive;
            }
            else
            {
                
#ifdef PD_DEBUG
                if constexpr ( assertsQ )
                {
                    wprint("Attempted to deactivate already inactive " + CrossingString(c) + ".");
                }
#endif
            }
            
            PD_ASSERT( crossing_count >= 0 );
            
            
#ifdef PD_DEBUG
            if constexpr ( assertsQ )
            {
                for( bool io : { In, Out } )
                {
                    for( bool side : { Left, Right } )
                    {
                        const Int a  = C_arcs(c,io,side);
                        
                        if( ArcActiveQ(a) && (A_cross(a,io) == c) )
                        {
                            pd_eprint(ClassName()+"DeactivateCrossing: active " + ArcString(a) + " is still attached to deactivated " + CrossingString(c) + ".");
                        }
                    }
                }
            }
#endif
        }
        
        /*!
         * @brief Deactivates arc `a`. Only for internal use.
         */
        
        void DeactivateArc( const Int a )
        {
            if( ArcActiveQ(a) )
            {
                PD_PRINT("Deactivating " + ArcString(a) + "." );
                
                --arc_count;
                A_state[a] = ArcState::Inactive;
            }
            else
            {
#if defined(PD_DEBUG)
                wprint("Attempted to deactivate already inactive " + ArcString(a) + ".");
#endif
            }
            
            PD_ASSERT( arc_count >= 0 );
        }
        
    public:
        
        Int CountActiveCrossings() const
        {
            Int counter = 0;
            
            for( Int c = 0; c < initial_crossing_count; ++c )
            {
                counter += CrossingActiveQ(c);
            }
            
            return counter;
        }
        
        Int CountActiveArcs() const
        {
            Int counter = 0;
            
            for( Int a = 0; a < initial_arc_count; ++a )
            {
                counter += ArcActiveQ(a);
            }
            
            return counter;
        }
        
        /*!
         * @brief Creates a copy of the planar diagram with all inactive crossings and arcs removed.
         *
         * Relabeling is done as follows:
         * First active arc becomes first arc in new planar diagram.
         * The _tail_ of this arc becomes the new first crossing.
         * Then we follow all arcs in the component with `NextArc<Head>(a)`.
         * The new labels increase by one for each invisited arc.
         * Same for the crossings.
         */
        
        PlanarDiagram CreateCompressed()
        {
            ptic( ClassName()+"::CreateCompressed");
            
            PlanarDiagram pd ( crossing_count, unlink_count );
            
            const Int m = A_cross.Dimension(0);
            
            mref<CrossingContainer_T> C_arcs_new  = pd.C_arcs;
            mptr<CrossingState>       C_state_new = pd.C_state.data();
            
            mref<ArcContainer_T>      A_cross_new = pd.A_cross;
            mptr<ArcState>            A_state_new = pd.A_state.data();
            
            C_scratch.Fill(-1);
            mptr<Int> C_labels = C_scratch.data();
            
            mptr<bool> A_visited = reinterpret_cast<bool *>(A_scratch.data());
            fill_buffer(A_visited,false,m);
            
            Int a_counter = 0;
            Int c_counter = 0;
            Int a_ptr     = 0;
            
            Int comp_counter = 0;
            
            while( a_ptr < m )
            {
                // Search for next arc that is active and has not yet been handled.
                while( ( a_ptr < m ) && ( A_visited[a_ptr] || (!ArcActiveQ(a_ptr)) ) )
                {
                    ++a_ptr;
                }
                
                if( a_ptr >= m )
                {
                    break;
                }
                
                ++comp_counter;
                
                Int a = a_ptr;
                
                AssertArc(a);
                
                {
                    const Int c_prev = A_cross(a,Tail);
                    
                    AssertCrossing(c_prev);
                    
                    if( C_labels[c_prev] < 0 )
                    {
                        const Int c = C_labels[c_prev] = c_counter;
                        
                        C_state_new[c] = C_state[c_prev];
                        
                        ++c_counter;
                    }
                }
                
                // Cycle along all arcs in the link component, until we return where we started.
                do
                {
                    const Int c_prev = A_cross(a,Tail);
                    const Int c_next = A_cross(a,Head);
                    
                    AssertCrossing(c_prev);
                    AssertCrossing(c_next);
                    
                    if( A_visited[a] )
                    {
                        eprint(ClassName()+"::CreateCompressed: A_visited[a].");
                    }
                    
                    A_state_new[a_counter] = ArcState::Active;
                    A_visited[a] = true;
                    
                    if( C_labels[c_next] < 0 )
                    {
                        const Int c = C_labels[c_next] = c_counter;
                        
                        C_state_new[c] = C_state[c_next];
                        
                        ++c_counter;
                    }
                    
                    {
                        const Int  c  = C_labels[c_prev];
                        const bool side = (C_arcs(c_prev,Out,Right) == a);
                        
                        C_arcs_new(c,Out,side) = a_counter;
                        
                        A_cross_new(a_counter,Tail) = c;
                    }
                    
                    {
                        const Int  c  = C_labels[c_next];
                        const bool side = (C_arcs(c_next,In,Right) == a);
                        
                        C_arcs_new(c,In,side) = a_counter;
                        
                        A_cross_new(a_counter,Head) = c;
                    }
                    
                    a = NextArc<Head>(a);
                    
                    AssertArc(a);
                    
                    ++a_counter;
                }
                while( a != a_ptr );
                
                ++a_ptr;
            }
            
            ptoc( ClassName()+"::CreateCompressed");
            
            return pd;
        }
        
        /*!
         * @brief Computes the writhe = number of right-handed crossings - number of left-handed crossings.
         */
        
        Int Writhe() const
        {
            const Int writhe = 0;
            
            for( Int c = 0; c < C_state.Size(); ++c )
            {
                if( CrossingRightHandedQ(C_state[c]) )
                {
                    ++writhe;
                }
                else
                {
                    --writhe;
                }
            }
            
            return writhe;
        }
        
        
        
    public:

        
#include "PlanarDiagram/Reconnect.hpp"
#include "PlanarDiagram/Checks.hpp"
#include "PlanarDiagram/R_I.hpp"
//#include "PlanarDiagram/R_II.hpp"
//#include "PlanarDiagram/R_IIa_Vertical.hpp"
//#include "PlanarDiagram/R_IIa_Horizontal.hpp"
        
#include "PlanarDiagram/Arcs.hpp"
#include "PlanarDiagram/Faces.hpp"
#include "PlanarDiagram/Components.hpp"
#include "PlanarDiagram/Strands.hpp"
#include "PlanarDiagram/ConnectedSum.hpp"
        
#include "PlanarDiagram/Simplify1.hpp"
#include "PlanarDiagram/Simplify2.hpp"
#include "PlanarDiagram/Simplify3.hpp"
#include "PlanarDiagram/Simplify4.hpp"
#include "PlanarDiagram/Simplify5.hpp"
        
#include "PlanarDiagram/PDCode.hpp"

#include "PlanarDiagram/Resolve.hpp"
#include "PlanarDiagram/Switch.hpp"
#include "PlanarDiagram/Split.hpp"
        
    public:
        
        
        void PrintInfo()
        {
            logprint(ClassName()+"::PrintInfo");
            
            logdump(initial_crossing_count);
            logdump(Arcs().Max());
            logdump(Arcs().Min());
            
            logdump(initial_arc_count);
            logdump(Crossings().Max());
            logdump(Crossings().Min());
//            
//            logdump(Crossings());
//            logdump(Arcs());
        }
        
        static std::string ClassName()
        {
            return std::string("PlanarDiagram") + "<" + TypeName<Int> + ">";
        }
        
    };

} // namespace KnotTools
