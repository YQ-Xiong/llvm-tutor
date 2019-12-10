LLVM Interval Analysis
=========
**CS6620 Compilers Course Project -- LLVM Interval Analysis**

**Team Members: Youyou Tu, Yingqiao Xiong**


### 1. Explaination of the project structure
* IntervalAnalysis: a directory for source code

* inputs: a directory that contains test cases 


### 2. To run our program
#### Setting up the environment
In order to build **llvm-tutor** you will need:
  * LLVM 9
  * C++ compiler that supports C++14
  * CMake 3.4.3 or higher

In order to run the passes, you will need:
  * **clang-9** (to generate input LLVM files)
  * **opt** (to run the passes)
 
#### Building and Testing
IntervalAnalysis will print out intervals for instructions. You can build it like this:
```bash
export LLVM_DIR=<installation/dir/of/llvm/9>
mkdir build
cd build
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR <source/dir/llvm/tutor>/IntervalAnalysis/
make
```
Before you can test it, you need to prepare an input file:
```bash
# Generate an llvm test file
$LLVM_DIR/bin/clang -S -emit-llvm <source/dir/llvm/tutor/>inputs/input.cpp -o input.ll
```

Finally, run IntervalAnalysis with opt:
```bash
# Run the pass on the llvm file
$LLVM_DIR/bin/opt -load-pass-plugin libIntervalAnalysis.dylib -passes=interval -disable-output input.ll
```
