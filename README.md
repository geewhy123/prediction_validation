Coding project: Prediction and Validation of Stock Prices
August 27, 2018 by Gary Yan

- See Instructions.md for original project description
- C++ code: driver and functions are included in src/main.cpp
- ./run.sh to compile and run the program
- I/O files should be in input/ and output/ directories

# Overview of approach

- The approach taken is to read in the actual and predicted data from the provided files, line by line, and build a vector of dictionaries (maps in C++), indexed by the time parameter

- The averaged errors over the window size is then written out to a file comparison.txt

- This approach should work for reasonable data sizes, but it is recognized that further optimization can be made without storing all the data (via streaming the data so that only variables on the order of the window size is needed), but this is not done at this time because the data is assumed to fit in memory, as per the instructions.

- Edge cases of empties, some tested and addressed.

- Tested to within +/- 0.01, as update to project describes.    

