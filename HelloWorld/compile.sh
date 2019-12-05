echo $LLVM_DIR

$LLVM_DIR/bin/clang -S -emit-llvm ../inputs/loop_phi.cpp -o loop_phi.ll

$LLVM_DIR/bin/opt -load-pass-plugin ../build/libHelloWorld.dylib -passes=hello-world -disable-output loop_phi.ll
