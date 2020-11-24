echo "building.."

buildpath="../src/engine/"
OUTNAME="minigame.out"
gcc ${buildpath}simulator_standalone.c ${buildpath}loadfile.c ${buildpath}annealing.c ${buildpath}simulator.c -lm -o $OUTNAME
echo "done?"

echo "running... "
./minigame.out "../test/levels/TestCase_002.redlevel"

./minigame.out "../test/levels/TestCase_002.redlevel" "^ g> 0"

#echo "cleanup"
#rm minigame.out
