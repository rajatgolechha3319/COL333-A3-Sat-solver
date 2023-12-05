start_time=$(date +%s.%3N)
g++ part1.cpp -o part1A -std=c++11
g++ part2.cpp -o part1B -std=c++11
./part1A $1
minisat $1.satinput $1.satoutput
./part1B $1
end_time=$(date +%s.%3N)
# Calculate execution time in milliseconds
execution_time=$(echo "($end_time - $start_time)" | bc)

echo "Execution time was $execution_time seconds."
