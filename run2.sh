start_time=`date +%s`
g++ part3.cpp -o part2 -std=c++11
./part2 $1
end_time=`date +%s`
rm $1.satinput
rm $1.satoutput
echo execution time was `expr $end_time - $start_time` s.