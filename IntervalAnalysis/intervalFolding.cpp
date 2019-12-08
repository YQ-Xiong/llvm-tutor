//
// Created by Youyou Tu on 2019-12-04.
//
#include "intervalFolding.h"

Interval *IntervalFoldInstruction(Instruction *I, DenseMap<Instruction*, Interval> *intervalMap){

    // check if terminator (ret),
    // first check phi node
    errs() << I << "\n";



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
            if(intervalMap->count(pointer)) {
                intervalMap->erase(pointer);
                intervalMap->insert(make_pair(pointer, i));
            }else{
                intervalMap->insert(make_pair(pointer, i));
            }
            errs() << pointer << "\t" << "value:"<< num << "\n";

            Interval* res = (Interval * )&i;
            return res;
            
        }else {  //case 2
            // store the value of previous variable into current variable
            const Instruction *valueInst = cast<Instruction>(value);

            if (intervalMap->count(valueInst)) {
                i = intervalMap->lookup(valueInst);
                if(intervalMap->count(pointer)) {
                    intervalMap->erase(pointer);
                    intervalMap->insert(make_pair(pointer, i));
                }else{
                    intervalMap->insert(make_pair(pointer, i));
                }
                errs() << pointer << "\t" << "value:" << i.high << "\n";
            }else{
                errs() << "error in store instruction";
            }
            Interval* res = (Interval * )&i;
            return res;

        }
    }

    if(auto *LI = dyn_cast<LoadInst>(I)) {
        Instruction *pointer = cast<Instruction>(LI->getPointerOperand());
        Interval i;
        if (intervalMap->count(pointer)) {
            i = intervalMap->lookup(pointer);
            intervalMap->insert(make_pair(LI, i));
            if (intervalMap->count(LI)) {
                intervalMap->erase(LI);
                intervalMap->insert(make_pair(LI, i));
            } else {
                intervalMap->insert(make_pair(LI, i));
            }

            errs() << LI << "\t" << "load value:" << intervalMap->lookup(LI).high << "\n";
        } else {
            errs() << "error in load instruction";
        }

        Interval* res = (Interval * )&i;
        return res;

    }




    // second calculate operands

    SmallVector<Use*, 8> Ops;

    for(Use &OpU : I->operands()){
//        Instruction *s = dyn_cast<Instruction>(OpU);
        //errs() << "Use " << *OpU << "\n";
        Ops.push_back(&OpU);
    }



    // compare instruction

    if(const auto *CI = dyn_cast<CmpInst>(I)){
//        ICMP_EQ    = 32,  ///< equal
//        ICMP_NE    = 33,  ///< not equal
//        ICMP_UGT   = 34,  ///< unsigned greater than
//        ICMP_UGE   = 35,  ///< unsigned greater or equal
//        ICMP_ULT   = 36,  ///< unsigned less than
//        ICMP_ULE   = 37,  ///< unsigned less or equal
//        ICMP_SGT   = 38,  ///< signed greater than
//        ICMP_SGE   = 39,  ///< signed greater or equal
//        ICMP_SLT   = 40,  ///< signed less than
//        ICMP_SLE   = 41,  ///< signed less or equal
        auto p = CI->getPredicate();
        errs()<< "predicate" << p << "\n";

        for (const Use &OpU : I->operands()) {
            // Fold the Instruction's operands.
                        errs() << "operand";
                        errs() << *OpU << "\n";
        }

        if(p == CmpInst::ICMP_EQ){
            //Interval i1=
            auto value1 = CI->getOperand(0);
            auto value2 = CI->getOperand(1);
            errs() << *value1 << "\n";
            errs() << *value2 << "\n";


            Interval interval1 = getIntervalFromOperand(value1, intervalMap);
            Interval interval2 = getIntervalFromOperand(value2, intervalMap);
            errs() << "get interval";
            return eqqInterval(interval1, interval2);
        }

//        if(p == CmpInst::ICMP_SGT){
//            auto value1 = CI->getOperand(0);
//            auto value2 = CI->getOperand(1);
//
//            pair<Interval, Interval> pr = getIntervalFromOperands(value1, value2, intervalMap);
//            Interval interval1 = pr.first;
//            Interval interval2 = pr.second;
//        }




    }


    // Insert
    if(auto *IVI = dyn_cast<InsertValueInst>(I)){

    }

    // extract

    if(auto *EVI = dyn_cast<ExtractValueInst>(I)){

    }

    // finally return foldInstOperands helper
    return foldInstOperands(I, Ops, intervalMap);

}


Interval getIntervalFromOperand(Value* value ,DenseMap<Instruction*, Interval> *intervalMap ){
    if(isa<ConstantInt>(value)){

        auto c = dyn_cast<ConstantInt>(value);
        int64_t v = c->getSExtValue();
        Interval interval = {v, v};
        return interval;

   }else{
        errs() << "reach 1\n";
        errs() << "cast instruction\n";
        Instruction* inst = cast<Instruction>(value);
        Interval interval = intervalMap->lookup(inst);

        return interval;
    }
}



Interval *foldInstOperands(Instruction *I, SmallVector<Use*, 8> Ops, DenseMap<Instruction*, Interval> *intervalMap) {

    unsigned oc = I->getOpcode();
    //errs() << "opcode :" << oc << "\n";

    if (Instruction::isUnaryOp(oc)) {
        int low0; int high0;
        Interval interval0;
        // handle the case that operand 0 is constant
        if (isa<ConstantInt>(Ops[0])) {
            auto *CI = dyn_cast<ConstantInt>(Ops[0]);
            errs() << "value" << "\n";
            int64_t v = CI->getSExtValue();
            errs() << v << "\n";

            interval0 = {v, v};
        }
        else {
            Instruction *instruction = dyn_cast<Instruction>(Ops[0]);
            auto pair0 = intervalMap->find(instruction);
            interval0 = pair0->second;
        }

        if (oc == Instruction::FNeg) {
            Interval *interval_new = invInterval(interval0);
            intervalMap->erase(I);
            intervalMap->insert(make_pair(I, *interval_new));
            return interval_new;
        }
        return nullptr;
    }

    // binary op
    if (Instruction::isBinaryOp(oc)) {


        int low0; int high0; int low1; int high1;
        Interval interval0;
        // handle the case that operand 0 is constant
        if (isa<ConstantInt>(Ops[0])) {
            auto *CI = dyn_cast<ConstantInt>(Ops[0]);
            errs() << "value" << "\n";
            int64_t v = CI->getSExtValue();
            errs() << v << "\n";

            interval0 = {v, v};
        }
        else {
            Instruction *instruction = dyn_cast<Instruction>(Ops[0]);
            auto pair0 = intervalMap->find(instruction);
            interval0 = pair0->second;
        }


        // handle the case that operand 1 is constant
        Interval interval1;
        if (isa<ConstantInt>(Ops[1])) {
            auto *CI = dyn_cast<ConstantInt>(Ops[1]);
            //errs() << "value" << "\n";
            int64_t v = CI->getSExtValue();
            errs() << v << "\n";

            interval1 = {v, v};
        }
        else {
            Instruction *instruction1 = dyn_cast<Instruction>(Ops[1]);
            auto pair1 = intervalMap->find(instruction1);
            interval1 = pair1->second;
        }

        // Add Instruction
        if (oc == Instruction::Add) {

            errs() << "Interval 0 low" << interval0.low << "\n";
            Interval *interval_new = plusInterval(interval0, interval1);
            intervalMap->erase(I);
            intervalMap->insert(make_pair(I, *interval_new));

            return interval_new;
        }

        // Subtract Instruction
        if (oc == Instruction::Sub) {
            Interval *interval1_inv = invInterval(interval1);
            Interval *interval_new = plusInterval(interval0, *interval1_inv);
            intervalMap->erase(I);
            intervalMap->insert(make_pair(I, *interval_new));

            return interval_new;
        }

        // Multiply Instruction
        if (oc == Instruction::Mul) {

            Interval *interval_new = mulInterval(interval0, interval1);
            intervalMap->erase(I);
            intervalMap->insert(make_pair(I, *interval_new));

            return interval_new;

        }

        // Divide Instruction
        if (oc == Instruction::SDiv) {
            Interval *interval_new = divInterval(interval0, interval1);
            intervalMap->erase(I);
            intervalMap->insert(make_pair(I, *interval_new));

            errs() << "after insert ";

            return interval_new;
        }



        return nullptr;

    }

    return nullptr;
}

int mulBounded(int op1, int op2) {
    int r1;
    if (op1 > INT_MAX / op2) {
        r1 = INT_MAX;
    } else if (op1 < INT_MIN / op2) {
        r1 = INT_MIN;
    } else {
        r1 = op1 * op2;
    }
    return r1;
}

SmallSetVector<int, 8> getSigns(Interval interval) {
    SmallSetVector<int, 8> vector;
    if (interval.low < -1 && -1 < interval.high) {
        vector.insert(-1);
    }
    else if (interval.low < 1 && 1 < interval.high) {
        vector.insert(1);
    }
    vector.insert(interval.low);
    vector.insert(interval.high);

    return vector;
}

Interval *invInterval(Interval interval0) {
    int low0 = interval0.low; int high0 = interval0.high;
    errs() << "FNeg Instruction" << "\n";
    int low; int high;
    if (low0 == INT_MIN && high0 == INT_MAX) {
        low = INT_MIN;
        high = INT_MAX;
    }
    else if (low0 == INT_MIN) {
        low = -high0;
        high = INT_MAX;
    }
    else if (high0 == INT_MAX) {
        low = INT_MIN;
        high = -low0;
    }
    else {
        low = -high0;
        high = -low0;
    }
    Interval *interval_new = new Interval;
    interval_new->low = low;
    interval_new->high = high;
    return interval_new;
}

Interval *mulInterval(Interval interval0, Interval interval1) {
    int low0 = interval0.low; int high0 = interval0.high; int low1 = interval1.low; int high1 = interval1.high;
    errs() << "Mul Instruction" << "\n";
    int low;
    int high;
    int r1 = mulBounded(low0, low1);
    int r2 = mulBounded(high0, low1);
    int r3 = mulBounded(low0, high1);
    int r4 = mulBounded(high0, high1);
    low = min({r1, r2, r3, r4});
    high = max({r1, r2, r3, r4});
    Interval *interval_new = new Interval;
    interval_new->low = low,
    interval_new->high = high;
    errs() << "new low " << low << "\n";
    errs() << "new high " << high << "\n";
    return interval_new;
}

Interval *plusInterval(Interval interval0, Interval interval1) {
    int low0 = interval0.low; int high0 = interval0.high; int low1 = interval1.low; int high1 = interval1.high;
    errs() << "Add Instruction" << "\n";
    errs() << "low 0 " << low0;
    errs() << "low 1 " << low1;
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

    Interval *interval_new = new Interval;
    interval_new->low = low;
    interval_new->high = high;
    return interval_new;

}

Interval *divInterval(Interval interval0, Interval interval1) {
    errs() << "Divide Instruction" << "\n";
    int low0 = interval0.low;
    int high0 = interval0.high;
    int low1 = interval1.low;
    int high1 = interval1.high;
    int low = INT_MAX;
    int high = INT_MIN;

    errs() << "Get signs \n";
    SmallSetVector<int, 8> signs = getSigns(interval1);

    while (!signs.empty()) {
        int value = signs.pop_back_val();
        low = min({low, low0 / value, high0 / value});
        high = max({high, low0 / value, high0 / value});
        errs() << "value " << value << "\n";
    }

    errs() << "new low " << low << "\n";
    errs() << "new high " << high << "\n";
    Interval *interval_new = new Interval;
    interval_new->low = low;
    interval_new->high = high;
    return interval_new;
}

Interval* eqqInterval(Interval a, Interval b){
    Interval* i = new Interval;
    if(a.low == a.high && a.high == b.low && b.low == b.high) {
        i->low = 1;
        i->high = 1;
        return i;
    }
    else{
        i->low = 0;
        i->high = 1;
        return i;
    }
}
