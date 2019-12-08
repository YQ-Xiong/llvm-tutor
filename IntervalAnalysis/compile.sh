export LLVM_DIR=/usr/local/opt/llvm

$LLVM_DIR/bin/clang -S -emit-llvm ../inputs/eqq.cpp -o eqq.ll

$LLVM_DIR/bin/opt -load-pass-plugin ../build/libIntervalAnalysis.dylib -passes=interval -disable-output eqq.ll