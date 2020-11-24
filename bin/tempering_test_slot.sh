echo "building.."

buildpath="../src/engine/"
OUTNAME="redconiglio.out"
gcc ${buildpath}redconiglio_tempering.c ${buildpath}loadfile.c ${buildpath}annealing.c ${buildpath}simulator.c -lm -o $OUTNAME
echo "done?"

STARTTIME=$(date +%s)
common_path="../test/levels/TestCase_"
extension=".redlevel"
logfile="red_rabbit_run_log.log"

#./redconiglio.out $common_path"001"".redlevel" # SOLVED EZ
#./redconiglio.out $common_path"002"".redlevel" # SOLVED EZ
#./redconiglio.out $common_path"003"".redlevel" # SOLVED EZ
./redconiglio.out $common_path"004"".redlevel" # SOLVED IN 6371968 ex13516104, ex54377749 ITERATIONS
#./redconiglio.out $common_path"005"".redlevel" # SOLVED EZ
#./redconiglio.out $common_path"006"".redlevel" # SOLVED EZ
#./redconiglio.out $common_path"007"".redlevel" # SOLVED IN 17909713, ex25687813 ITERATIONS
#./redconiglio.out $common_path"008"".redlevel" # SOLVED EZ
#./redconiglio.out $common_path"009"".redlevel" # SOLVED EZ
#./redconiglio.out $common_path"010"".redlevel" # SOLVED EZ

#echo "cleanup"
rm $OUTNAME


ENDTIME=$(date +%s)
echo "It took $(($ENDTIME - $STARTTIME)) seconds to complete this task..."
echo "running..."
