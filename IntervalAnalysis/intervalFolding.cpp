//
// Created by Youyou Tu on 2019-12-04.
//
#include "intervalFolding.h"

Interval *IntervalFoldInstruction(Instruction *I, DenseMap<Instruction*, Interval> *intervalMap){

    // check if terminator (ret),
    // first check phi node



    if(const auto *SI = dyn_cast<StoreInst>(I)){
        //Value* v =  &SI->getPointerOperand();
    }



    // second calculate operands
//
//    SmallVector<Use *, 8> Ops;
//
//    for(const Use &OpU : I->operands()){
//
//        Ops.push_back(OpU)
//    }

    auto oc = I->getOpcode();
    for(const Use &OpU : I->operands()){
        // *some instruction has only one operand
        unsigned oc = I->getOpcode();
        if(Instruction::isUnaryOp(oc)){
            return nullptr;
        }

    // unary op
    if(Instruction::isUnaryOp(oc)){
        return nullptr;
    }

    // binary op
//    if(Instruction::isBinaryOp(oc)){
//        // (1,4) + ()
//        if (oc == Instruction::And) {
//            Interval interval0 = intervalMap.find(dyn_cast<Instruction>(Ops[0]));
//            errs() << "Interval 0" << interval0 << "\n";
//            Interval interval1 = intervalMap.find(Ops[1]);
//        }
//
//        return nullptr;
//    }


    // store instruction

    if (const auto *SI = dyn_cast<StoreInst>(I)) {

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
