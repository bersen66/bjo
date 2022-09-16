# project-template
Template for cmake c++ projects.

## Installation:
Requires [Conan](https://conan.io/center/) and CMake. 
They can be installed by:
```shell
pip install conan
pip install cmake
```
### Build your app in debug mode:

```shell
mkdir cmake-build-debug
cd cmake-build-debug
conan install .. --build=missing
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTING=ON -DENABLE_BENCHMARKS=ON ..
cmake --build .  
```

### Build your app in release mode:

```shell
mkdir cmake-build-release
cd cmake-build-release
conan install .. --build=missing
cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTING=OFF -DENABLE_BENCHMARKS=OFF ..
cmake --build .  
```

### Run template:

```shell
cd <build folder>
./TEMPLATE <program_options>
```

## Usage: 

* Add external dependencies via [conan package manager](https://conan.io/center/)
* All unit tests must be in tests folder
* All benchmarks must be in benchmarks folder
* All headers must be in include folder
* All sources must be in src folder

