//
// Created by Youyou Tu on 2019-12-04.
//
#include "intervalFolding.h"

Interval *IntervalFoldInstruction(Instruction *I, DenseMap<Instruction*, Interval> *intervalMap){

    // check if terminator (ret),
    // first check phi node



    if( auto *SI = dyn_cast<StoreInst>(I)){
        // there are two cases of store instruction
        // case 1: store instruction i32 %1, one integer, one variable
        // case 2: store instruction %2 %3, two variables


        auto value = SI->getValueOperand();
        Instruction* pointer = cast<Instruction>(SI->getPointerOperand());
        Interval i;
        if(isa<ConstantInt>(value)){ //case
            auto* CI = cast<ConstantInt>(value);
            int64_t num = CI->getSExtValue();
            i = {num, num};
            intervalMap->insert(make_pair(pointer, i));

            
        }else{  //case 2
            // store the value of previous variable into current variable
            const Instruction* valueInst = cast<Instruction>(value);

            if(intervalMap->count(valueInst)){
                i = intervalMap->lookup(valueInst);
                intervalMap->insert(make_pair(pointer, i));
            }
        }
    }



    // second calculate operands

    SmallVector<Use*, 8> Ops;

    for(Use &OpU : I->operands()){
//        Instruction *s = dyn_cast<Instruction>(OpU);
        errs() << "Use " << *OpU << "\n";
        Ops.push_back(&OpU);
    }

    unsigned oc = I->getOpcode();
    errs() << "opcode :" << oc << "\n";
    // binary op
    if(Instruction::isBinaryOp(oc)){
        errs() << "Binary Opcode" << "\n";

        int low0; int high0; int low1; int high1;
        // handle the case that operand 0 is constant
        if (isa<ConstantInt>(Ops[0])) {
            if (auto *CI = dyn_cast<ConstantInt>(Ops[0])) {
                errs() << "value" << "\n";
                int64_t v = CI->getSExtValue();
                errs() << v << "\n";

                low0 = v;
                high0 = v;

            }
        }
        else {
            Instruction *instruction = dyn_cast<Instruction>(Ops[0]);
            auto pair0 = intervalMap->find(instruction);
            low0 = pair0->second.low;
            high0 = pair0->second.high;
        }


        // handle the case that operand 1 is constant
        if (isa<ConstantInt>(Ops[1])) {
            if (auto *CI = dyn_cast<ConstantInt>(Ops[1])) {
                errs() << "value" << "\n";
                int64_t v = CI->getSExtValue();
                errs() << v << "\n";

                low1 = v;
                high1 = v;
            }
        }
        else {
            Instruction *instruction1 = dyn_cast<Instruction>(Ops[1]);
            auto pair1 = intervalMap->find(instruction1);
            low1 = pair1->second.low;
            high1 = pair1->second.high;
        }

        // Add Instruction
        if (oc == Instruction::Add) {

            Interval interval_new = plusInterval(low0, high0, low1, high1);
            intervalMap->insert(make_pair(I, interval_new));
        }

        // Multiply Instruction
        if (oc == Instruction::Mul) {

            Interval interval_new = mulInterval(low0, high0, low1, high1);
            intervalMap->insert(make_pair(I, interval_new));

        }



        //

        return nullptr;
    }


//    for(const Use &OpU : I->operands()){
//        // *some instruction has only one operand
//        unsigned oc = I->getOpcode();
//        if(Instruction::isUnaryOp(oc)){
//            return nullptr;
//        }
//
//    // unary op
//    if(Instruction::isUnaryOp(oc)){
//        return nullptr;
//    }




    // store instruction


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

int mulBounded(int op1, int op2) {
    int r1;
    if (op1 > INT_MAX/op2) {
        r1 = INT_MAX;
    }
    else if (op1 < INT_MIN/op2) {
        r1 = INT_MIN;
    }
    else {
        r1 = op1 * op2;
    }
    return r1;
}

Interval mulInterval(int low0, int high0, int low1, int high1) {
    errs() << "Mul Instruction" << "\n";
    int low;
    int high;
    int r1 = mulBounded(low0, low1);
    int r2 = mulBounded(high0, low1);
    int r3 = mulBounded(low0, high1);
    int r4 = mulBounded(high0, high1);
    low = min({r1, r2, r3, r4});
    high = max({r1, r2, r3, r4});
    Interval interval_new = {low, high};
    errs() << "new low " << low << "\n";
    errs() << "new high " << high << "\n";
    return interval_new;
}

Interval plusInterval(int low0, int high0, int low1, int high1) {
    errs() << "Add Instruction" << "\n";

    int low; int high;
    // calculate lower bound
    if (low0 == INT_MIN || low1 == INT_MIN) {
        low = INT_MIN;
    }
    else if (low0 == INT_MAX || low1 == INT_MAX) {
        low = INT_MAX;
    }
    else {
        low = low0 + low1;
    }

    errs() << "New low " << low << "\n";

    // calculate upper bound
    if (high0 == INT_MAX || high1 == INT_MAX) {
        high = INT_MAX;
    }
    else if (high0 == INT_MIN || low0 == INT_MIN) {
        high = INT_MIN;
    }
    else {
        high = high0 + high1;
    }

    errs() << "New high " << high << "\n";

    Interval interval_new = {low, high};
    return interval_new;

}
