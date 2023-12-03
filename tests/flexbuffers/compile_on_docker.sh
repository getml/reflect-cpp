cd reflect-cpp/tests/flexbuffers
mkdir build
cd build
cmake .. -DCMAKE_C_COMPILER=gcc-11 -DCMAKE_CXX_COMPILER=g++-11
make -j4
