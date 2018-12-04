
#include "../mypin_cache.H"

// CPU AMD FX 4350 (Bulldozer) @4.2Ghz Characteristics (http://www.cpu-world.com/CPUs/Bulldozer/AMD-FX-Series%20FX-4350.html)
// .24 ns/cycle
//https://www.extremetech.com/extreme/188776-how-l1-and-l2-cpu-caches-work-and-why-theyre-an-essential-part-of-modern-chips

#define L1_CACHE_LATENCY         4
#define L2_CACHE_LATENCY         21
#define L3_CACHE_LATENCY         87
//#define RAM_LATENCY              204
//Modified to 204 cycles to account for 51ns + 42 cycles
#define RAM_LATENCY 		246

//Defines Cache Configurations
namespace ITLR
{
    // instruction Top Level registers: 4 kB pages, 32 entries, fully associative
    const UINT32 lineSize = 1*KILO;
    const UINT32 cacheSize = 32 * lineSize;
    const UINT32 associativity = 32;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);
    const UINT32 max_associativity = associativity;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}

namespace DTLR
{
    // data Top level registers: 4 kB pages, 32 entries, fully associative
    const UINT32 lineSize = 1*KILO;
    const UINT32 cacheSize = 32 * lineSize;
    const UINT32 associativity = 32;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);
    const UINT32 max_associativity = associativity;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}

namespace IL1
{
    //1st level instruction cache - 4 x 32K - 4 way
    const UINT32 cacheSize = 32*KILO;
    const UINT32 lineSize = 64;
    const UINT32 associativity = 4;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_NO_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);
    const UINT32 max_associativity = associativity;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}

namespace DL1
{
    // 1st level data cache: 32 kB, 32 B lines, 32-way associative
    const UINT32 cacheSize = 32*KILO;
    const UINT32 lineSize = 64;
    const UINT32 associativity = 4;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_NO_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);
    const UINT32 max_associativity = associativity;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}

namespace UL2
{
    // 2nd level unified cache: 2 MB, 64 B lines, direct mapped
    const UINT32 cacheSize = 2*KILO*KILO;
    const UINT32 lineSize = 64;
    const UINT32 associativity = 16;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);

    typedef CACHE_DIRECT_MAPPED(max_sets, allocation) CACHE;
}

namespace UL3
{
    // 3rd level unified cache: 8 MB, 64 B lines, direct mapped
    const UINT32 cacheSize = 8*MEGA;
    const UINT32 lineSize = 64;
    const UINT32 associativity = 64; 
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);

    typedef CACHE_DIRECT_MAPPED(max_sets, allocation) CACHE;
}


class MyCache
{
    private:

        // Level 1 Instruction Cache
        IL1::CACHE* il1[4] = {
            new IL1::CACHE("L1I Core0 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core1 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core2 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core3 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
        };
        
        // Level 1 Data Cache
        DL1::CACHE* dl1[4] = {
            new DL1::CACHE("L1D Core0 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1D Core1 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1D Core2 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1D Core3 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
        };
        
        // Level 2 Unified Cache        
        UL2::CACHE* ul2[4] = {
            new UL2::CACHE("L2 Left  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Right Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
        };

        // Level 3 Unified Cache
        UL3::CACHE* ul3 = new UL3::CACHE("L3 Unified Cache", UL3::cacheSize, \
            UL3::lineSize, UL3::associativity);
    
    public:

        IL1::CACHE* getIL1(VIRTUALID id){
            return il1[id];
        }

        DL1::CACHE* getDL1(VIRTUALID id){
            return dl1[id];
        }
        
        UL2::CACHE* getUL2(VIRTUALID id){
            //return ul2[id/2]; -- changed by Nidhi because 4 L2 caches
            return ul2[id]; 
        }
        
        UL3::CACHE* getUL3(VIRTUALID id){
            return ul3;
        }

};
