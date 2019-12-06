//
// Created by Youyou Tu on 2019-12-04.
//
#include "intervalFolding.h"
using namespace llvm;

Interval* IntervalFoldInstruction(Instruction *I ){
    // check if terminator (ret),
    // first check phi node



    if(const auto *SI = dyn_cast<StoreInst>(I)){
        //Value* v =  &SI->getPointerOperand();
    }



    // second calculate operands

    for(const Use &OpU : I->operands()){
        // *some instruction has only one operand
        unsigned oc = I->getOpcode();
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
    if(auto *IVI = dyn_cast<InsertValueInst>(I)){

    }

    // extract

    if(auto *EVI = dyn_cast<ExtractValueInst>(I)){

    }

    // finally return foldInstOperands helper
    return nullptr;
}

//Interval plus(Interval a, Interval b){
//
//}
