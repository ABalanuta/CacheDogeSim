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
#include <sstream>

#include "pin.H"

typedef UINT32 CACHE_STATS; // type of cache hit/miss counters

#include "mypin_cache.H"
#include "mycache_conf.H"


#define CACHE_DOGE_ENABLED false


// Other Vars
MyCache cache;
uint64_t exec_time[4];
uint64_t exec_inst_access[4];
uint64_t exec_data_access[4];

LOCALFUN VOID InsRef(ADDRINT addr, THREADID tid);
LOCALFUN VOID MemRefMulti(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, THREADID tid);
LOCALFUN VOID MemRefSingle(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, THREADID tid);
LOCALFUN VOID Ul2Access(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, VIRTUALID vid);
LOCALFUN VOID Ul3Access(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, VIRTUALID vid);
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

// received the virtual id of the thread that is trying to write
void tryEvictL1(ADDRINT addr, UINT32 size, VIRTUALID vid){

    //std::cout << "tryEvictL1 " << vid << endl;

    for (uint i = 0; i < 4; i++ ){
        if (i != vid)
        {
            cache.getDL1(i)->Evict(addr, size);
            //std::cout << "EvictL1 " << i << endl;
        }
    }

}

// received the virtual id of the thread that is trying to write
void tryEvictL2(ADDRINT addr, UINT32 size, VIRTUALID vid){
    
    //std::cout << "tryEvictL2 " << vid << endl;
    for (uint i = 0; i < 2; i++ ){
        if (i != vid/2)
        {
            cache.getUL2(i*2)->Evict(addr, size);
            //std::cout << "EvictL2 " << i*2 << endl;
        }
    }

}


// Accessing Instructions Read Only
LOCALFUN VOID InsRef(ADDRINT addr, THREADID tid)
{
    const UINT32 size = 1; // assuming access does not cross cache lines
    const CACHE_BASE::ACCESS_TYPE accessType = CACHE_BASE::ACCESS_TYPE_LOAD;
    THREADID vid = getVirtualId(tid);
    
    // Update Counters
    exec_inst_access[vid]++;
    exec_time[vid] += L1_CACHE_LATENCY; // Adds L1 cache latency
    // TODO Add size dependent latency

    const BOOL il1Hit = cache.getIL1(vid)->AccessSingleLine(addr, accessType);

    // second level unified Cache
    if ( ! il1Hit) Ul2Access(addr, size, accessType, vid);
}

// Accessing L1 Data Caches Read or Write
LOCALFUN VOID MemRefMulti(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, THREADID tid)
{
    THREADID vid = getVirtualId(tid);
    
    // Update Counters
    exec_data_access[vid]++;
    exec_time[vid] += L1_CACHE_LATENCY; // Adds L1 cache latency
    // TODO Add size dependent latency

    // first level D-cache
    const BOOL dl1Hit = cache.getDL1(vid)->Access(addr, size, accessType);

    if ( !dl1Hit ){
        tryEvictL1(addr, size, vid);
        // second level unified Cache
        Ul2Access(addr, size, accessType, vid);
    }
}

// Accessing L1 Data Caches Read or Write
LOCALFUN VOID MemRefSingle(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, THREADID tid)
{
    THREADID vid = getVirtualId(tid);

    // Update Counters
    exec_data_access[vid]++;
    exec_time[vid] += L1_CACHE_LATENCY; // Adds L1 cache latency
    // TODO Add size dependent latency

    // first level D-cache
    const BOOL dl1Hit = cache.getDL1(vid)->AccessSingleLine(addr, accessType);

    if ( !dl1Hit ){
        tryEvictL1(addr, size, vid);
        // second level unified Cache
        Ul2Access(addr, size, accessType, vid);
    }
}

// Accessing L2/L3 Data Caches Read or Write
LOCALFUN VOID Ul2Access(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, VIRTUALID vid)
{
    
    // Update Counters
    exec_time[vid] += L2_CACHE_LATENCY; // Adds L2 cache latency

    // second level cache
    // const BOOL ul2Hit = ul2.Access(addr, size, accessType);
    const BOOL ul2Hit = cache.getUL2(vid)->Access(addr, size, accessType);

    if ( !ul2Hit ) {
        tryEvictL2(addr, size, vid);

        // third level unified cache
        Ul3Access(addr, size, accessType, vid);
    }
}

// Accessing L2/L3 Data Caches Read or Write
LOCALFUN VOID Ul3Access(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType, VIRTUALID vid)
{
    // Update Counters
    exec_time[vid] += L3_CACHE_LATENCY; // Adds L1 cache latency

    // third level cache
    const BOOL ul3Hit = cache.getUL3(vid)->Access(addr, size, accessType);

    if ( !ul3Hit ) {
        // Update Counters
        exec_time[vid] += RAM_LATENCY; // Adds RAM cache latency
    }
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


string humanize(uint64_t var){
    static const char sz[] = {' ', 'K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y'};
    int index = 0;
    std::ostringstream out;

    while(var > 1000){
        var /= 1000;
        index++;
    }
    
    out << var << sz[index];

    return out.str();
}

LOCALFUN VOID Fini(int code, VOID * v)
{   
    // std::cerr << *cache.getIL1(0);
    // std::cerr << *cache.getIL1(1);
    // std::cerr << *cache.getIL1(2);
    // std::cerr << *cache.getIL1(3);

    std::cerr << *cache.getDL1(0);
    std::cerr << *cache.getDL1(1);
    std::cerr << *cache.getDL1(2);
    std::cerr << *cache.getDL1(3);
    
    std::cerr << *cache.getUL2(0);
    std::cerr << *cache.getUL2(2);

    std::cerr << *cache.getUL3(0);

    uint64_t total_delay = 0;
    
    for(int i = 0; i < 4; i++ ){

        total_delay += exec_time[i];

        std::cerr << "Core" << i << endl;
        std::cerr << "\tAcces Delay Sum: " << humanize(exec_time[i]) << " Cycles" << endl;
        std::cerr << "\tInst Access Sum: " << humanize(exec_inst_access[i]) << endl;
        std::cerr << "\tData Access Sum: " << humanize(exec_data_access[i]) << endl;
        if (exec_inst_access[i] > 0){
        
            //load misses per kilo-instructions (MPKI)
            uint64_t miss = cache.getIL1(i)->Misses() + cache.getDL1(i)->Misses();
            std::cerr << "\tL1 MPKI: " << miss * 1.0 / (exec_inst_access[i]/1000) << endl;
        
            //AVG Delay per Data Access (ADPA)
            std::cerr << "\tDPKA: " << exec_time[i] * 1.0 / exec_data_access[i] << endl;
        }

    }
    std::cerr << endl;
    
    std::cerr << "Total Acces Delay Sum: " << humanize(total_delay) << " Cycles" << endl;
    std::cerr << endl;
    
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
