#! /bin/bash

#- blackscholes (apps)
#- bodytrack (apps)
#- facesim (apps)
#- ferret (apps)
#- freqmine (apps)
#- raytrace (apps)
#- swaptions (apps)
#- fluidanimate (apps)
#- vips (apps)
#- x264 (apps)
#- canneal (kernels)
#- dedup (kernels)
#- streamcluster (kernels)

list="blackscholes bodytrack facesim ferret freqmine raytrace swaptions fluidanimate vips canneal dedup streamcluster"

EXE=$1

if [[ $list =~ (^|[[:space:]])$EXE($|[[:space:]]) ]]
then
    echo $EXE
else
    echo 'Wrong test! '
    echo "Select one of: "$list
    echo ""
    exit
fi

PIN="/root/pin-3.7-97619-g0d0c92f4f-gcc-linux/pin"
PARSEC_DIR="/root/parsec-2.1"
CACHEDOGE_TOOL="obj-intel64/MyPinTool.so"
PATH_TO_TOOL=/root/src/cacheDogeSimTool/$CACHEDOGE_TOOL
HERE=$(pwd)
SIM=simdev #test simsmall simmedium simlarge

DATE=$(date "+%HH_%MM_%Ss__%d.%m.%Y")
ALGO="alg2"

#Compile Tool
cd ..; ./compile.sh 2>&1 >> /dev/null; cd $HERE

# Set Parsec Env
cd $PARSEC_DIR; source env.sh; cd $HERE

ROUNDS=1
#MIG=''              # Migrate Cores (use "-mig" if yes nothing otherwise)
CI='1000'           # Check Interval
#IRT='0.333'         # Invalidation Ratio Treshold
MT='0.0'            # Precentage of Memory between Invalidations

mkdir -p "logs"/"$EXE"_"$SIM"
LOG_FILE="logs"/"$EXE"_"$SIM"/"$EXE"_"$SIM"_"Baseline"_"$ROUNDS"_"$ALGO"_"$DATE.log"
BASE=$LOG_FILE

# BASELINE
echo "$EXE $SIM migration: $MIG Interval:$CI Invld%:$IRT Memory:$MT" > $LOG_FILE # Create New File
for i in $(seq -f "%02g" 1 $ROUNDS);
do
    echo "#########################################################################" | tee -a $LOG_FILE
    echo "Round $i $EXE $SIM Baseline" | tee -a $LOG_FILE
    parsecmgmt -a run -p $EXE -i $SIM -n 4 -x pre -s "$PIN -t $PATH_TO_TOOL --" | tee -a $LOG_FILE
    echo "#########################################################################" | tee -a $LOG_FILE
done


# Testing different IRT
for irt in $(seq -f "%.3f" .05 0.05 .95);
do
    
    IRT=$irt
    LOG_FILE="logs"/"$EXE"_"$SIM"/"$EXE"_"$SIM"_"CacheDoge"_"$ROUNDS"_"$CI"_"$IRT"_"$MT"_"$ALGO"_"$DATE.log"
    
    echo "$EXE $SIM migration: $MIG Interval:$CI Invld%:$IRT Memory:$MT" > $LOG_FILE # Create New File
    for i in $(seq 1 $ROUNDS);
    do
        echo "#########################################################################" | tee -a $LOG_FILE
        echo "Round $i $EXE $SIM migration: $MIG Interval:$CI Invld%:$IRT Memory:$MT" | tee -a $LOG_FILE
        parsecmgmt -a run -p $EXE -i $SIM -n 4 -x pre -s "$PIN -t $PATH_TO_TOOL -mig -ci $CI -irt $IRT -mt $MT --" | tee -a $LOG_FILE
        echo "#########################################################################" | tee -a $LOG_FILE
    done

done
