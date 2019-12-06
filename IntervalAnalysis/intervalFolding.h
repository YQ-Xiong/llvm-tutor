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
    int low;
    int high;
};


map<Instruction*, Interval> intervalMap;


Interval IntervalFoldInstruction(Instruction *I );