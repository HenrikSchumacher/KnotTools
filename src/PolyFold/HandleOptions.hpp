private:

void HandleOptions( int argc, char** argv )
{
    namespace po = boost::program_options;
    
    // `try` executes some code; `catch` catches exceptions and handles them.
    try
    {

            
        // Declare all possible options with type information and a documentation string that will be printed when calling this routine with the -h [--help] option.
        po::options_description desc("Allowed options");
        desc.add_options()
        ("help,h", "produce help message")
        ("diam,d", po::value<Real>(), "set hard sphere diameter")
        ("edge-count,n", po::value<Int>(), "set number of edges")
        ("burn-in,b", po::value<LInt>(), "set number of burn-in steps")
        ("skip,s", po::value<LInt>(), "set number of steps skipped between samples")
        ("sample-count,N", po::value<Int>(), "set number of samples")
        ("output,o", po::value<std::string>(), "set output directory")
        ("tag,T", po::value<std::string>(), "set a tag to append to output directory")
        ("extend,e", "extend name of output directory by information about the experiment, then append value of --tag option")
        ("verbosity,v", po::value<int>(), "how much information should be printed to Log.txt file.")
        ("pcg-multiplier,M", po::value<std::string>(), "specify 128 bit unsigned integer used by pcg64 for the \"multiplier\" (implementation dependent -- better not touch it unless you really know what you do)")
        ("pcg-increment,I", po::value<std::string>(), "specify 128 bit unsigned integer used by pcg64 for the \"increment\" (every processor should have its own)")
        ("pcg-state,S", po::value<std::string>(), "specify 128 bit unsigned integer used by pcg64 for the state (use this for seeding)")
        ("low-mem,m", "force deallocation of large data structures; this will be a bit slower but peak memory will be less")
        ("angles,a", "compute statistics on curvature and torsion angles and report them in file \"Info.m\"")
        ("squared-gyradius,g", "compute squared radius of gyration and report in file \"Info.m\"")
        ("pd-code,c", "compute pd codes and print to file \"PDCodes.tsv\"")
        ("polygons,P", po::value<LInt>(), "print every [arg] sample to file")
        ("histograms,H", po::value<Int>(), "create histograms for curvature and torsion angles with [arg] bins")
        ("no-checks", "perform folding without checks for overlap of hard spheres")
        ("reflections,R", "allow pivot moves to be ortientation reversing")
        ;
        
        
        
        // Declare that arguments without option prefixe are reinterpreted as if they were assigned to -o [--output].
        po::positional_options_description p;
        p.add("output", -1);
        
        // Parse the arguments.
        po::variables_map vm;
        po::store(
            po::command_line_parser(argc, argv).options(desc).positional(p).run(),
            vm
        );
        po::notify(vm);
        
        if( vm.count("help") )
        {
            std::stringstream s;
            s << desc;
            print(s.str());
            exit(0);
        }
        
        
        valprint<a>("Using Quaternions", BoolString(Clisby_T::quaternionsQ) );
        print("");
        
        if( vm.count("diam") )
        {
            hard_sphere_diam = vm["diam"].as<Real>();
        }

        valprint<a>("Hard Sphere Diameter d", hard_sphere_diam);
        
        if( vm.count("edge-count") )
        {
            n = vm["edge-count"].as<Int>();
            
            valprint<a>("Edge Count n", n);
        }
        else
        {
            throw std::invalid_argument( "Number of edges unspecified. Use the option -n to set it." );
        }
        
        if( vm.count("burn-in") )
        {
            burn_in_accept_count = vm["burn-in"].as<LInt>();
            
            valprint<a>("Burn-in Count b", burn_in_accept_count);
        }
        else
        {
            throw std::invalid_argument( "Number of burn-in steps unspecified. Use the option -b to set it." );
        }
        
        if( vm.count("skip") )
        {
            skip = vm["skip"].as<LInt>();
            
            valprint<a>("Skip Count s", skip);
        }
        else
        {
            throw std::invalid_argument( "Number of skip unspecified. Use the option -s to set it." );
        }
        
        if( vm.count("sample-count") )
        {
            N = vm["sample-count"].as<Int>();
            
            valprint<a>("Sample Count N", N);
        }
        else
        {
            throw std::invalid_argument( "Number of samples unspecified. Use the option -N to set it." );
        }
        
        print("");
        
        anglesQ = (vm.count("angles") != 0);
        valprint<a>("Compute Angle Statistics", BoolString(anglesQ) );

        squared_gyradiusQ = (vm.count("squared-gyradius") != 0);
        valprint<a>("Compute Squared Gyradius", BoolString(squared_gyradiusQ) );
        
        pdQ = (vm.count("pd-code") != 0);
        valprint<a>("Compute PD Codes", BoolString(pdQ) );
        
        
        if( vm.count("polygons") )
        {
            steps_between_print = vm["polygons"].as<LInt>();
            printQ = (steps_between_print > 0);
        }
        valprint<a>("Polygon Snapshot Skip", ToString(steps_between_print) );
        
        if( vm.count("histograms") )
        {
            bin_count = Ramp( vm["histograms"].as<Int>() );
            curvature_hist = Tensor1<LInt,Int> ( bin_count, 0 );
            torsion_hist   = Tensor1<LInt,Int> ( Int(2) * bin_count, 0 );
        }
        valprint<a>("Histogram Bin Count", ToString(bin_count) );

        print("");
        
        force_deallocQ = (vm.count("low-mem") != 0);
        valprint<a>("Forced Deallocation", BoolString(force_deallocQ) );
        
        do_checksQ = (vm.count("no-checks") == 0);
        valprint<a>("Hard Sphere Checks", BoolString(do_checksQ) );
        
        allow_reflectionsQ = (vm.count("reflections") != 0);
        
        if( Clisby_T::quaternionsQ && allow_reflectionsQ)
        {
            wprint("Reflections cannot be activated while quaternions are used. Deactivating reflections");
            
            allow_reflectionsQ = false;
        }
        
        valprint<a>("Allow Reflections", BoolString(allow_reflectionsQ) );
        
        
        if( vm.count("pcg-multiplier") )
        {
            prng_multiplierQ = true;
            
            prng_init.multiplier = vm["pcg-multiplier"].as<std::string>();
            
            valprint("PCG Multiplier", prng_init.multiplier);
        }
        
        if( vm.count("pcg-increment") )
        {
            prng_incrementQ = true;
            
            prng_init.increment = vm["pcg-increment"].as<std::string>();
            
            valprint<a>("PCG Increment", prng_init.increment);
        }
        
        if( vm.count("pcg-state") )
        {
            prng_stateQ = true;
            
            prng_init.state = vm["pcg-state"].as<std::string>();
            
            valprint<a>("PCG State", prng_init.state);
        }
        
        print("");
        
        if( vm.count("verbosity") )
        {
            verbosity = vm["verbosity"].as<int>();
        }

        valprint<a>("Verbosity", verbosity);
        
        
        if( vm.count("output") )
        {
            if( vm.count("extend") )
            {
                    path = std::filesystem::path( vm["output"].as<std::string>()
                        + "-d" + ToStringFPGeneral(hard_sphere_diam)
                        + "-n" + ToString(n)
                        + "-b" + ToString(burn_in_accept_count)
                        + "-s" + ToString(skip)
                        + "-N" + ToString(N)
                        + (vm.count("tag") ? ("_" + vm["tag"].as<std::string>()) : "")
                    );
            }
            else
            {
                path = std::filesystem::path( vm["output"].as<std::string>()
                    + (vm.count("tag") ? ("_" + vm["tag"].as<std::string>()) : "")
                );
            }
            
            valprint<a>("Output Path", path.string());
        }
        else
        {
            throw std::invalid_argument( "Output path unspecified. Use the option -o to set it." );
        }
        
    }
    catch( std::exception & e )
    {
        eprint(e.what());
        exit(1);
    }
    catch(...)
    {
        eprint("Exception of unknown type!");
        exit(1);
    }
    
    print("");
    
    
    if( ! (squared_gyradiusQ || pdQ || anglesQ || (bin_count > 1) ) )
    {
        eprint("Not computing anything. Aborting.");
        exit(2);
    }
    
    // Make sure that the working directory exists.
    try{
        std::filesystem::create_directories(path);
    }
    catch( std::exception & e )
    {
        eprint(e.what());
        exit(1);
    }
    catch(...)
    {
        eprint("Exception of unknown type!");
        exit(1);
    }
    
}
