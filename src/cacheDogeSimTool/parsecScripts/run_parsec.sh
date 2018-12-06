#! /bin/bash

PIN="/root/pin-3.7-97619-g0d0c92f4f-gcc-linux/pin"
PARSEC_DIR="/root/parsec-2.1"
CACHEDOGE_TOOL="obj-intel64/MyPinTool.so"
PATH_TO_TOOL=/root/src/cacheDogeSimTool/$CACHEDOGE_TOOL
HERE=$(pwd)
SIM=simdev #test simsmall simmedium simlarge
EXE="bodytrack"
DATE=$(date "+%HH_%MM_%Ss__%d.%m.%Y")
ALGO="alg1"

#Compile Tool
cd ..; ./compile.sh 2>&1 >> /dev/null; cd $HERE

# Set Parsec Env
cd $PARSEC_DIR; source env.sh; cd $HERE

ROUNDS=10
MIG=''              # Migrate Cores (use "-mig" if yes nothing otherwise)
CI='10000'          # Check Interval
IRT='0.333'         # Invalidation Ratio Treshold
MT='0.0'            # Precentage of Memory between Invalidations

mkdir -p "logs"/"$EXE"_"$SIM"
LOG_FILE="logs"/"$EXE"_"$SIM"/"$EXE"_"$SIM"_"$MIG"_"$CI"_"$IRT"_"$MT"_"$ALGO"_"$DATE.log"

# # BASELINE
# echo "$EXE $SIM migration: $MIG Interval:$CI Invld%:$IRT Memory:$MT" > $LOG_FILE # Create New File
# for i in $(seq 1 $ROUNDS);
# do
#     echo "#########################################################################" | tee -a $LOG_FILE
#     echo "Round $i $EXE $SIM migration: $MIG Interval:$CI Invld%:$IRT Memory:$MT" | tee -a $LOG_FILE
#     #parsecmgmt -a run -p $EXE -i $SIM -n 4 -x pre -s "$PIN -t $PATH_TO_TOOL $MIG -ci $CI -irt $IRT -mt $MT --" | tee -a $LOG_FILE
#     echo "#########################################################################" | tee -a $LOG_FILE
# done


# TEST Diff IRT
for irt in $(seq -f "%.3f" .05 0.05 .95);
do
    
    IRT=$irt
    LOG_FILE="logs"/"$EXE"_"$SIM"/"$EXE"_"$SIM"_"$MIG"_"$CI"_"$IRT"_"$MT"_"$ALGO"_"$DATE.log"
    
    echo "$EXE $SIM migration: $MIG Interval:$CI Invld%:$IRT Memory:$MT" > $LOG_FILE # Create New File
    for i in $(seq 1 $ROUNDS);
    do
        echo "#########################################################################" | tee -a $LOG_FILE
        echo "Round $i $EXE $SIM migration: $MIG Interval:$CI Invld%:$IRT Memory:$MT" | tee -a $LOG_FILE
        #parsecmgmt -a run -p $EXE -i $SIM -n 4 -x pre -s "$PIN -t $PATH_TO_TOOL $MIG -ci $CI -irt $IRT -mt $MT --" | tee -a $LOG_FILE
        echo "#########################################################################" | tee -a $LOG_FILE
    done

done
