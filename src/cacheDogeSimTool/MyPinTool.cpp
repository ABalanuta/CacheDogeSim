/*
 * Created on Mon Nov 19 2018
 * Author: Artur Balanuta
 * Copyright (c) 2018 Carnegie Mellon University
 */


/*! @file
 *  This file contains an ISA-portable PIN tool for functional simulation of
 *  instruction+data TLB+cache hierarchies
 */

#include <iostream>

#include "pin.H"

typedef UINT32 CACHE_STATS; // type of cache hit/miss counters

#include "mypin_cache.H"



// CPU Intel i7-7700K (Skylake) @4Ghz Characteristics (https://www.7-cpu.com/cpu/Skylake.html)
// .25 ns/cycle

#define L1_DATA_CACHE_LATENCY_NS    1.25 // ns

#define L2_CACHE_LATENCY_NS         3.0 // ns
#define L3_CACHE_LATENCY_NS         9.5 // ns
#define RAM_LATENCY_NS              51 // ns




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

// LOCALFUN ITLR::CACHE itlr("Inst Reg ", ITLR::cacheSize, ITLR::lineSize, ITLR::associativity);
// LOCALVAR DTLR::CACHE dtlr("Data Reg ", DTLR::cacheSize, DTLR::lineSize, DTLR::associativity);
LOCALVAR IL1::CACHE  il1( "L1I Cache", IL1::cacheSize, IL1::lineSize, IL1::associativity);
LOCALVAR DL1::CACHE  dl1( "L1D Cache", DL1::cacheSize, DL1::lineSize, DL1::associativity);
LOCALVAR UL2::CACHE  ul2( "L2U Cache", UL2::cacheSize, UL2::lineSize, UL2::associativity);
LOCALVAR UL3::CACHE  ul3( "L3U Cache", UL3::cacheSize, UL3::lineSize, UL3::associativity);

LOCALFUN VOID Ul2Access(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType)
{
    // second level unified cache
    const BOOL ul2Hit = ul2.Access(addr, size, accessType);

    // third level unified cache
    if ( ! ul2Hit) ul3.Access(addr, size, accessType);
}



LOCALFUN VOID InsRef(ADDRINT addr)
{
    const UINT32 size = 1; // assuming access does not cross cache lines
    const CACHE_BASE::ACCESS_TYPE accessType = CACHE_BASE::ACCESS_TYPE_LOAD;

    // ITLR
    //itlr.AccessSingleLine(addr, accessType);

    // first level I-cache
    const BOOL il1Hit = il1.AccessSingleLine(addr, accessType);

    // second level unified Cache
    if ( ! il1Hit) Ul2Access(addr, size, accessType);
}

LOCALFUN VOID MemRefMulti(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType)
{
    // DTLB
    //dtlr.AccessSingleLine(addr, CACHE_BASE::ACCESS_TYPE_LOAD);

    // first level D-cache
    const BOOL dl1Hit = dl1.Access(addr, size, accessType);

    // second level unified Cache
    if ( ! dl1Hit) Ul2Access(addr, size, accessType);
}

LOCALFUN VOID MemRefSingle(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType)
{
    // DTLB
    //dtlr.AccessSingleLine(addr, CACHE_BASE::ACCESS_TYPE_LOAD);

    // first level D-cache
    const BOOL dl1Hit = dl1.AccessSingleLine(addr, accessType);
    
    // second level unified Cache
    if ( ! dl1Hit) Ul2Access(addr, size, accessType);
}

LOCALFUN VOID Instruction(INS ins, VOID *v)
{
    // all instruction fetches access I-cache
    INS_InsertCall(
        ins, IPOINT_BEFORE, (AFUNPTR)InsRef,
        IARG_INST_PTR,
        IARG_END);

    if (INS_IsMemoryRead(ins) && INS_IsStandardMemop(ins))
    {
        const UINT32 size = INS_MemoryReadSize(ins);
        const AFUNPTR countFun = (size <= 4 ? (AFUNPTR) MemRefSingle : (AFUNPTR) MemRefMulti);

        // only predicated-on memory instructions access D-cache
        INS_InsertPredicatedCall(
            ins, IPOINT_BEFORE, countFun,
            IARG_MEMORYREAD_EA,
            IARG_MEMORYREAD_SIZE,
            IARG_UINT32, CACHE_BASE::ACCESS_TYPE_LOAD,
            IARG_END);
    }

    if (INS_IsMemoryWrite(ins) && INS_IsStandardMemop(ins))
    {
        const UINT32 size = INS_MemoryWriteSize(ins);
        const AFUNPTR countFun = (size <= 4 ? (AFUNPTR) MemRefSingle : (AFUNPTR) MemRefMulti);

        // only predicated-on memory instructions access D-cache
        INS_InsertPredicatedCall(
            ins, IPOINT_BEFORE, countFun,
            IARG_MEMORYWRITE_EA,
            IARG_MEMORYWRITE_SIZE,
            IARG_UINT32, CACHE_BASE::ACCESS_TYPE_STORE,
            IARG_END);
    }
}

LOCALFUN VOID Fini(int code, VOID * v)
{   
    std::cerr << il1;
    std::cerr << dl1;

    std::cerr << ul2;
    std::cerr << ul3;
}

GLOBALFUN int main(int argc, char *argv[])
{
    PIN_Init(argc, argv);


    

    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);

    // Never returns
    PIN_StartProgram();

    return 0; // make compiler happy
}
