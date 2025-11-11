#!/bin/bash

# There are no graded tests for HW 9 Tic Tac Toe 
# Student Tests are optional but highly encouraged
# Pull the graded tests
# wget "https://drive.google.com/uc?export=download&id=1rTt7oH7pQIW53bPUsFjWl33yhiuyenu5" -O hw09.tar.gz
# tar xzf hw09.tar.gz

echo "Compiling..."
mkdir build
cd build
RELEASE=ON CC=clang CXX=clang++ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. || exit 1
make || exit 1

#echo "Running clang-tidy..."
#../run-clang-tidy.py -quiet -header-filter=.*/src/.* || exit 1

# Return to root folder (so cwd is correct)
cd ..

# Run student tests
echo "Running student tests..."
timeout 30 build/tests/tests [student] || exit 1

# Run graded tests
# echo "Running graded tests..."
# timeout 30 build/tests/tests [graded] -r=github || exit 1
