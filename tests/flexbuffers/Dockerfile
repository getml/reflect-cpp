FROM ubuntu:22.04

ENV TZ=Europe/Berlin
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN useradd reflectcpp

RUN apt-get update
RUN apt-get install -y git cmake  
RUN apt install gcc-11 g++-11

WORKDIR /home/reflectcpp

RUN git clone https://github.com/google/flatbuffers.git 
WORKDIR /home/reflectcpp/flatbuffers
RUN git checkout v23.5.26
RUN mkdir build
WORKDIR /home/reflectcpp/flatbuffers/build
RUN cmake .. -DCMAKE_C_COMPILER=gcc-11 -DCMAKE_CXX_COMPILER=g++-11
RUN make

WORKDIR /home/reflectcpp
COPY compile_on_docker.sh .

USER reflectcpp
