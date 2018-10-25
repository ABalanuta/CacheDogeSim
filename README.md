# CacheDogeSim

# Todo
  
  Pin Tool Tutorials (Basics)
  
  Brandon CacheSim
  
  Pin Tool Builtin Cache Tools
  
  Brandon Cachesim P2
  
  CacheDogeSim Implementation
  

# References
  [Pintool](https://software.intel.com/en-us/articles/pin-a-binary-instrumentation-tool-downloads)

  [Pintool Manual](https://software.intel.com/sites/landingpage/pintool/docs/97619/Pin/html/)
  
  [PinCRT](https://software.intel.com/sites/landingpage/pintool/docs/97619/PinCRT/html/)


# SPEC and Pintool
```
 # from https://software.intel.com/sites/landingpage/pintool/docs/97619/Pin/html/index.html#MEMORY

General
Tools are restricted from linking with any system libraries and/or calling any system calls. See PinCRT for more information.

There are several things that a Pintool writer must be aware of.

IARG_REG_VALUE cannot be used to pass floating point register values to an analysis routine.
Also, see the OS-specific restrictions below. Windows OS or Linux OS
Often, a Pintool writer wants to run the SPEC benchmarks to see the results of their research. There are many ways one can update the scripts to invoke Pin on the SPEC tests; this is one. In your $SPEC/config file, add the following two lines:

   submit=$PIN_HOME/intel64/bin/pin -t /my/pin/tool -- $command
   use_submit_for_speed=yes
Now the SPEC harness will automatically run Pin with whatever benchmarks it runs. Note that you need the full path name for Pin and Pintool binaries. Replace "intel64" with "ia32" if you are using a 32-bit system.
```