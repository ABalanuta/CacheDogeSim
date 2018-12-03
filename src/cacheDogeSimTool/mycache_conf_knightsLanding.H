
#include "mypin_cache.H"

typedef THREADID VIRTUALID;

// CPU Intel Knights Landing - 1.4GHz
// .71 ns/cycle
// http://mrfunk.info/?page_id=181
// https://www.agner.org/optimize/blog/read.php?i=772&v=t

#define L1_CACHE_LATENCY         2
#define L2_CACHE_LATENCY         10
#define RAM_LATENCY 		212

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
    //const UINT32 lineSize = 32; -- Changed by Nidhi to 64B
    const UINT32 lineSize = 64;
    //const UINT32 associativity = 32; -- Changed by Nidhi to 8
    const UINT32 associativity = 8;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_NO_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);
    const UINT32 max_associativity = associativity;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}

namespace DL1
{
    // 1st level data cache: 32 kB, 32 B lines, 32-way associative
    const UINT32 cacheSize = 32*KILO;
    //const UINT32 lineSize = 32; -- Changed by Nidhi to 64B
    const UINT32 lineSize = 64;
    //const UINT32 associativity = 32; -- Changed by Nidhi to 8
    const UINT32 associativity = 8;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_NO_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);
    const UINT32 max_associativity = associativity;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}

namespace UL2
{
    // 2nd level unified cache: 2 MB, 64 B lines, direct mapped
    const UINT32 cacheSize = 1024*KILO;
    const UINT32 lineSize = 64;
    //const UINT32 associativity = 1; --> changed to 4 by Nidhi
    const UINT32 associativity = 4;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);

    typedef CACHE_DIRECT_MAPPED(max_sets, allocation) CACHE;
}

namespace UL3
{
    // 3rd level unified cache: 2 MB, 64 B lines, direct mapped
    //const UINT32 cacheSize = 2*MEGA; -- changed to 8 by Nidhi 
    const UINT32 cacheSize = 8*MEGA;
    const UINT32 lineSize = 64;
    //const UINT32 associativity = 1; -- changed to 16 by Nidhi
    const UINT32 associativity = 16; 
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_ALLOCATE;

    const UINT32 max_sets = cacheSize / (lineSize * associativity);

    typedef CACHE_DIRECT_MAPPED(max_sets, allocation) CACHE;
}


class MyCache
{
    private:

        // Level 1 Instruction Cache
        IL1::CACHE* il1[72] = {
            new IL1::CACHE("L1I Core0 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core1 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core2 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core3 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core4 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core5 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core6 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core7 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core8 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core9 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core10 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core11 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core12 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core13 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core14 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core15 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core16 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core17 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core18 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core19 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core20 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core21 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core22 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core23 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core24 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core25 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core26 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core27 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core28 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core29 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core30 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core31 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core32 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core33 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core34 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core35 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core36 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core37 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core38 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core39 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core40 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core41 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core42 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core43 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core44 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core45 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core46 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core47 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core48 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core49 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core50 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core51 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core52 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core53 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core54 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core55 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core56 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core57 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core58 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core59 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core60 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core61 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core62 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
            new IL1::CACHE("L1I Core63 Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity),
        };
        
        // Level 1 Data Cache
        DL1::CACHE* dl1[72] = {
            new DL1::CACHE("L1I Core0 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core1 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core2 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core3 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core4 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core5 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core6 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core7 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core8 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core9 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core10 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core11 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core12 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core13 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core14 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core15 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core16 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core17 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core18 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core19 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core20 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core21 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core22 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core23 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core24 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core25 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core26 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core27 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core28 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core29 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core30 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core31 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core32 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core33 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core34 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core35 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core36 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core37 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core38 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core39 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core40 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core41 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core42 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core43 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core44 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core45 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core46 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core47 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core48 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core49 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core50 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core51 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core52 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core53 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core54 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core55 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core56 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core57 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core58 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core59 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core60 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core61 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core62 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
            new DL1::CACHE("L1I Core63 Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity),
        };
        
        // Level 2 Unified Cache        
        UL2::CACHE* ul2[36] = {
            new UL2::CACHE("L2 Core0  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core1  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core2  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core3  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core4  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core5  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core6  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core7  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core8  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core9  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core10  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core11 Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core12  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core13  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core14  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core15  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core16  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core17  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core18  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core19  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core20  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core21  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core22  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core23  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core24  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core25  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core26  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core27 Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core28  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core29  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core30  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
            new UL2::CACHE("L2 Core31  Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity),
        };

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
        
};
