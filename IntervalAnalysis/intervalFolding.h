//
// Created by Youyou Tu on 2019-12-04.
//

#ifndef LLVM_TUTOR_INTERVALFOLDING_H
#define LLVM_TUTOR_INTERVALFOLDING_H

#endif //LLVM_TUTOR_INTERVALFOLDING_H

#include "llvm/IR/Function.h"
//#include "llvm/IR/I "
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;
using namespace std;


struct Interval{
    int64_t low;
    int64_t high;
};

Interval *IntervalFoldInstruction(Instruction *I, DenseMap<Instruction*, Interval> *intervalMap);

Interval *foldInstOperands(Instruction *I, SmallVector<Use*, 8> Ops, DenseMap<Instruction*, Interval> *intervalMap);

Interval getIntervalFromOperand(Value* value,DenseMap<Instruction*, Interval> *intervalMap );

int mulBounded(int op1, int op2);

Interval *mulInterval(int low0, int high0, int low1, int high1);

Interval *plusInterval(int low0, int high0, int low1, int high1);

Interval *eqqInterval(Interval a, Interval b);
