FROM ubuntu:latest

COPY . usr/src/template
WORKDIR usr/src/template


RUN apt-get update
RUN apt-get install -y python3-pip
RUN pip install conan cmake
RUN apt install -y build-essential

WORKDIR build-release
RUN conan install .. --build=missing
RUN cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTING=OFF -DENABLE_BENCHMARKS=OFF ..
RUN cmake --build .

ENTRYPOINT ["./TEMPLATE"]

