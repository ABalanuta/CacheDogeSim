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
#include "mycache_conf.H"


#define CACHE_DOGE_ENABLED false


// Other Vars
UINT32 threads[4];
MyCache cache;

LOCALFUN VOID InsRef(ADDRINT addr, THREADID tid);
LOCALFUN VOID MemRefMulti(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, THREADID tid);
LOCALFUN VOID MemRefSingle(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, THREADID tid);
LOCALFUN VOID Ul2Access(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, VIRTUALID vid);
LOCALFUN VOID Instruction(INS ins, VOID *v);
LOCALFUN VOID Fini(int code, VOID * v);
THREADID getVirtualId(THREADID thread_id);



THREADID getVirtualId(THREADID thread_id)
{
    if (CACHE_DOGE_ENABLED) {
        //TODO MAGIC HERE
        // Access Swap Table
        return -1;
    }
    return (thread_id % 4);
}

// Accessing Instructions Read Only
LOCALFUN VOID InsRef(ADDRINT addr, THREADID tid)
{
    const UINT32 size = 1; // assuming access does not cross cache lines
    const CACHE_BASE::ACCESS_TYPE accessType = CACHE_BASE::ACCESS_TYPE_LOAD;
    THREADID vid = getVirtualId(tid);

    //const BOOL il1Hit = il1.AccessSingleLine(addr, accessType);
    const BOOL il1Hit = cache.getIL1(vid)->AccessSingleLine(addr, accessType);

    // second level unified Cache
    if ( ! il1Hit) Ul2Access(addr, size, accessType, vid);
}

// Accessing L1 Data Caches Read or Write
LOCALFUN VOID MemRefMulti(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, THREADID tid)
{
    THREADID vid = getVirtualId(tid);
    
    // first level D-cache
    // const BOOL dl1Hit = dl1.Access(addr, size, accessType);
    const BOOL dl1Hit = cache.getDL1(vid)->Access(addr, size, accessType);

    //TODO IF WRITE, Evict CACHES !?
    // TODO IF ITS A HIT ignore EVICTION


    // second level unified Cache
    if ( ! dl1Hit) Ul2Access(addr, size, accessType, vid);
}

// Accessing L1 Data Caches Read or Write
LOCALFUN VOID MemRefSingle(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, THREADID tid)
{
    THREADID vid = getVirtualId(tid);

    // first level D-cache
    //const BOOL dl1Hit = dl1.AccessSingleLine(addr, accessType);
    const BOOL dl1Hit = cache.getDL1(vid)->AccessSingleLine(addr, accessType);
    
    //TODO IF WRITE, Evict CACHES !?
    // TODO IF ITS A HIT ignore EVICTION

    // second level unified Cache
    if ( ! dl1Hit) Ul2Access(addr, size, accessType, vid);   
}

// Accessing L2/L3 Data Caches Read or Write
LOCALFUN VOID Ul2Access(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, VIRTUALID vid)
{
    // second level cache
    // const BOOL ul2Hit = ul2.Access(addr, size, accessType);
    const BOOL ul2Hit = cache.getUL2(vid)->Access(addr, size, accessType);

    // // third level unified cache
    // if ( ! ul2Hit) ul3.Access(addr, size, accessType);
    if ( ! ul2Hit) cache.getUL3(vid)->Access(addr, size, accessType);
}



LOCALFUN VOID Instruction(INS ins, VOID *v)
{
    // all instruction fetches access I-cache
    INS_InsertCall(
        ins,
        IPOINT_BEFORE, 
        (AFUNPTR)InsRef,
        IARG_INST_PTR,
        IARG_THREAD_ID,
        IARG_END);

    if (INS_IsMemoryRead(ins) && INS_IsStandardMemop(ins))
    {
        const UINT32 size = INS_MemoryReadSize(ins);
        const AFUNPTR countFun = (size <= 4 ? (AFUNPTR) MemRefSingle : (AFUNPTR) MemRefMulti);

        // only predicated-on memory instructions access D-cache
        INS_InsertPredicatedCall(
            ins, 
            IPOINT_BEFORE,
            countFun,
            IARG_MEMORYREAD_EA,
            IARG_MEMORYREAD_SIZE,
            IARG_UINT32,
            CACHE_BASE::ACCESS_TYPE_LOAD,
            IARG_THREAD_ID,
            IARG_END);
    }

    if (INS_IsMemoryWrite(ins) && INS_IsStandardMemop(ins))
    {
        const UINT32 size = INS_MemoryWriteSize(ins);
        const AFUNPTR countFun = (size <= 4 ? (AFUNPTR) MemRefSingle : (AFUNPTR) MemRefMulti);

        // only predicated-on memory instructions access D-cache
        INS_InsertPredicatedCall(
            ins, IPOINT_BEFORE,
            countFun,
            IARG_MEMORYWRITE_EA,
            IARG_MEMORYWRITE_SIZE,
            IARG_UINT32,
            CACHE_BASE::ACCESS_TYPE_STORE,
            IARG_THREAD_ID,
            IARG_END);
    }
}

LOCALFUN VOID Fini(int code, VOID * v)
{   
    std::cerr << *cache.getIL1(0) << endl;
    std::cerr << *cache.getIL1(1) << endl;
    std::cerr << *cache.getIL1(2) << endl;
    std::cerr << *cache.getIL1(3) << endl;

    std::cerr << *cache.getDL1(0) << endl;
    std::cerr << *cache.getDL1(1) << endl;
    std::cerr << *cache.getDL1(2) << endl;
    std::cerr << *cache.getDL1(3) << endl;
    
    std::cerr << *cache.getUL2(0) << endl;
    std::cerr << *cache.getUL2(2) << endl;

    std::cerr << *cache.getUL3(0) << endl;
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
