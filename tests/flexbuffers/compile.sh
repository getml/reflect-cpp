#!/bin/bash

docker run -v $PWD/../../:"/home/reflectcpp/reflect-cpp" --rm -it -t reflectcpp_test_flexbuffers bash compile_on_docker.sh 
