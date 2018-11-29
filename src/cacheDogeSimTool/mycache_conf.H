
#include "mypin_cache.H"

typedef THREADID VIRTUALID;

// CPU Intel i7-7700K (Skylake) @4Ghz Characteristics (https://www.7-cpu.com/cpu/Skylake.html)
// .25 ns/cycle

#define L1_CACHE_LATENCY         5
#define L2_CACHE_LATENCY         12
#define L3_CACHE_LATENCY         47
#define RAM_LATENCY              204

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
    // 1st level instruction cache: 32 kB, 32 B lines, 32-way associative
    const UINT32 cacheSize = 32*KILO;
    const UINT32 lineSize = 32;
    const UINT32 associativity = 32;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_NO_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);
    const UINT32 max_associativity = associativity;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}

namespace DL1
{
    // 1st level data cache: 32 kB, 32 B lines, 32-way associative
    const UINT32 cacheSize = 32*KILO;
    const UINT32 lineSize = 32;
    const UINT32 associativity = 32;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_NO_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);
    const UINT32 max_associativity = associativity;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}

namespace UL2
{
    // 2nd level unified cache: 2 MB, 64 B lines, direct mapped
    const UINT32 cacheSize = 256*KILO;
    const UINT32 lineSize = 64;
    const UINT32 associativity = 1;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);

    typedef CACHE_DIRECT_MAPPED(max_sets, allocation) CACHE;
}

namespace UL3
{
    // 3rd level unified cache: 2 MB, 64 B lines, direct mapped
    const UINT32 cacheSize = 2*MEGA;
    const UINT32 lineSize = 64;
    const UINT32 associativity = 1;
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
        UL2::CACHE* ul2[2] = {
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
            return ul2[id/2];
        }
        
        UL3::CACHE* getUL3(VIRTUALID id){
            return ul3;
        }

};