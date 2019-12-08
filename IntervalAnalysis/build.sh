export LLVM_DIR=/usr/local/opt/llvm/
cd /Users/yingqiaoxiong/compiler/llvm-tutor/build/
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../IntervalAnalysis/
make