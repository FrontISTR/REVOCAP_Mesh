# docker build ./ -t revo_dev
# docker run -v C:\Users\XXX:/workspace -it revo_dev /bin/bash

FROM ubuntu:20.04

RUN set -x \
 && apt-get -y update \
 && apt-get -y install gcc \
 && apt-get -y install g++ \
 && apt-get -y install gfortran \
 && apt-get -y install libgl-dev \
 && apt-get -y install libglu-dev \
 && apt-get -y install libglew-dev \
 && apt-get -y install libocct-modeling-algorithms-dev \
 && apt-get -y install libboost-dev \
 && apt-get -y install cmake



WORKDIR /workspace

