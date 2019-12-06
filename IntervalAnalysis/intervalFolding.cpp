//
// Created by Youyou Tu on 2019-12-04.
//
#include "intervalFolding.h"
using namespace llvm;

Interval *llvm::IntervalFoldInstruction(Instruction *I ){
    // check if terminator (ret),
    // first check phi node



    // second calculate operands

    for(const Use &OpU : I->operands()){
        // *some instruction has only one operand
        Opcode oc = I.getOpcode();
        if(Instruction::isUnaryOp(oc)){
            return nullptr;
        }

        if(Instruction::isBinaryOp(oc)){
            return nullptr;
        }
    }

    // compare instruction

    if(const auto *CI = dyn_cast<CmpInst>(I)){

    }

    // load

    if(const auto *LI = dyn_cast<LoadInst>(I)){

    }

    // Insert
    if(auto *IVI = dyn_cast<InsertValue>(I)){

    }

    // extract

    if(auto *EVI = dyn_cast<ExtractValueInst>(I)){

    }

    // finally return foldInstOperands helper
    return nullptr;
}


