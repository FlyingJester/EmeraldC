#include "optimizer.hpp"
#include <list>
#include <cstdlib>

namespace Compiler {

struct Emitter{
    std::list<Op> operations;
};

/////
// SP changes and accesses

template<typename T, int op>
inline bool AccessesSPAt(const T &that){
    return that->operands[op]=="$sp";
}

template<typename T, int op>
inline bool ReadsSPAt(const T &that){
    return (that->operands[op].find("$sp")!=std::string::npos);
}

template<typename T, int op>
inline bool UsesSPAt(const T &that){
    return (that->operands[op].find("($sp)")!=std::string::npos);
}

template<typename T, int op>
inline int GetSPOffsetAt(const T &that){
    unsigned long p_sp = that->operands[op].find("($sp)");
    std::string number = that->operands[op].substr(0, p_sp);
    return strtol(number.c_str(), nullptr, 10);
}

template<typename T>
inline bool AccessesSP(const T &that){
    return AccessesSPAt<T, 0>(that) || 
        AccessesSPAt<T, 1>(that) ||
        AccessesSPAt<T, 2>(that);
}

template<typename T>
inline bool ReadsSP(const T &that){
    return ReadsSPAt<T, 0>(that) || 
        ReadsSPAt<T, 1>(that) ||
        ReadsSPAt<T, 2>(that);
}

template<typename T>
inline int GetSPChange(const T &that){
    return std::strtol(that->operands[2].c_str(), nullptr, 10);
}

template<typename T>
inline bool IsSPChange(const T that){
    return (that->code=="addiu") &&
        AccessesSPAt<T, 0>(that) &&
        AccessesSPAt<T, 1>(that) &&
        (!that->operands[2].empty());
}

/////
// Load/Store info

template<typename T>
inline bool IsStackLoad(const T &that){
    return (that->code=="lw") &&
        UsesSPAt<T, 1>(that);
}

template<typename T>
inline bool IsStackStore(const T &that){
    return (that->code=="sw") &&
        UsesSPAt<T, 1>(that);
}

template<typename T>
inline const char *StackLoadStoreRegister(const T &that){
    return that->operands[0].c_str();
}

template<typename T, typename S>
inline bool UsesRegister(const T &that, const S &reg){
    return (that->operands[0].find(reg)!=std::string::npos) ||
        (that->operands[1].find(reg)!=std::string::npos) ||
        (that->operands[2].find(reg)!=std::string::npos);
}

template<typename T>
inline int GetStackLoadStoreOffset(const T &that){
    return GetSPOffsetAt<T, 1>(that);
}

/////
// Optimizations

int RemoveDeadCodeAfterJump(std::list<Op> &operations){
// Any code after an undonditional jump and before the next label is unreachable.
// This isn't true of ASM. But it is true of our generated code.
    int removals = 0;
    for(std::list<Op>::const_iterator first = operations.cbegin(); first!=operations.cend(); first++){
        if(first->code!="j") continue;
        for(std::list<Op>::const_iterator second = first; second!=operations.cend(); second++){
            if(second->code.back()==':'){
                 first++;
                 if(first!=second)
                     removals++;
                 first = operations.erase(first, second);
                 break;
            }
        }
    }
    return removals;
}

bool CanCombineMoves(const std::list<Op>::iterator &first, const std::list<Op>::iterator &second){

    std::list<Op>::iterator i = first;
    i++;

    if((first==second) || (i==second)) return true;

    for(; i!=second; i++){
        if(UsesRegister(i, first->operands[0]) || UsesRegister(i, second->operands[0]))
            return false;
    }
    return true;
}

int CombineImmediateMoves(std::list<Op> &operations){
// Often we will load a value into a register (usually $t1), and then immediately move it.
// This optimization looks for a li, lw, add, or sub, checks it destination,  looks for a
// move that uses it as a source, and changes the destination of the first instruction to the
// target of the second. For this to be valid, all instructions in between must not reference
// either the first or second instruction's target.

    unsigned removals = 0;
    for(std::list<Op>::iterator first = operations.begin(); first!=operations.end(); first++){

        if((first->code!="add") && 
            (first->code!="addu") && 
            (first->code!="addi") && 
            (first->code!="addiu") && 
            (first->code!="sub") && 
            (first->code!="lw") &&
            (first->code!="li") && 
            (first->code!="mflo") && 
            (first->code!="mfhi"))
            continue;

        for(std::list<Op>::iterator second = first; second!=operations.end(); second++){
            if((second->code=="move") && (second->operands[1]==first->operands[0])){
                *first = {first->code, {second->operands[0], first->operands[1], first->operands[2]}};
                operations.erase(second);
                removals++;
                break;
            }
            else if(!CanCombineMoves(first, second)) break;
        }
    }
    return removals;
}

bool CanCombineLoadsArithmetic(const std::list<Op>::iterator &first, const std::list<Op>::iterator &second){

    std::list<Op>::iterator i = first;
    i++;

    if((first==second) || (i==second)) return true;

    for(; i!=second; i++){
        if(UsesRegister(i, first->operands[0]) || UsesRegister(i, second->operands[1]) || UsesRegister(i, second->operands[2]))
            return false;
    }
    return true;

}

int CombineLoadsDestructiveArithmetic(std::list<Op> &operations){
// Sometimes we see this:
//
// li  $t1, 23
// add $t1, $t1, $t2
// 
// Assuming no instructions in between use the destination of the load,
// we can collapse this to:
// addi $t1, $t2, 23
//
// Nore that this only works with addition and multiplication. We would
// need to negate or reciprocate with subtraction or multiplication.

    unsigned removals = 0;
    for(std::list<Op>::iterator first = operations.begin(); first!=operations.end(); first++){

        if(first->code!="li")
            continue;

        for(std::list<Op>::iterator second = first; second!=operations.end(); second++){
            if((second->code.front()=='b') || (second->code.back()==':')) break;
            if((second->code=="add") && 
                ((second->operands[1]==first->operands[0]) || second->operands[2]==first->operands[0]) && 
                (second->operands[1]!=second->operands[2])){

                if(second->operands[1]==first->operands[0]){ // add to, imm, other
                    second->operands[1] = second->operands[2];
                    second->operands[2] = first->operands[1];
                }
                else{ // add to, other, imm
                    second->operands[2] = first->operands[1];
                }
                
                second->code = "addi";
                
                first = operations.erase(first);
                removals++;
                break;
            }
            else if(!CanCombineLoadsArithmetic(first, second)) break;
        }
    }
    return removals;

}

int RemoveRedundantStackLoadStores(std::list<Op> &operations){
// We see a lot of:
//
// sw      $t1, 0($sp)
// li      $t1, <something>
// lw      $t2, 0($sp)
//
// What we need to recognize is:
// sw      $t#a, 0($sp)
//  *instructions
// lw      $t#b, 0($sp)
// 
// Where all instructions:
//  * do not use $t#b
//  * do not use $sp
//
// We can then condense the operation to:
// 
// move $t#b, $t#a
// in place of the first instruction.
//
    std::list<Op>::const_iterator first = operations.cbegin(), second;
    int removals = 0;
    // Primer
start:
    while(first!=operations.cend()){
        if(IsStackStore(first)){
                break;
        }
        else{
            first++;
        }
    }

    second = first;

    while(second!=operations.cend()){
        if(IsStackLoad(second)){
            operations.insert(first, {"move", {StackLoadStoreRegister(second), StackLoadStoreRegister(first)}});
            operations.erase(first);
            second = operations.erase(second);
            removals++;
            first = second;
            goto start;
        }
        else if((second->code.front()=='b') || (second->code.back()==':')){
            first++;
            goto start;
        }
        else if(UsesRegister(second, "$sp")){
            for(std::list<Op>::const_iterator i = first; i!=second; i++){
                if(UsesRegister(i, StackLoadStoreRegister(second)) || UsesRegister(i, StackLoadStoreRegister(first))){
                    first++;
                    goto start;
                }
            }
        }
        second++;
    }
    return removals;
}

int RemoveRedundantSPChanges(std::list<Op> &operations){
    // Fairly often, we will see very rapid popping and pushing where one
    // expression ends and another begins.
    // This optimization removes negated SP changes if $sp is not accessed in between.

    std::list<Op>::const_iterator first = operations.cbegin(), second;
    int removals = 0;
    // Primer
start:
    while(first!=operations.cend()){
        if(IsSPChange(first)){
                break;
        }
        else{
            first++;
        }
    }
    if(first==operations.cend()) return removals;

    second = first;

    while(second!=operations.cend()){
        if(IsSPChange(second)){
            if(GetSPChange(second)==-GetSPChange(first)){
                operations.erase(first);
                second = operations.erase(second);
                removals++;
            }
            first = second;
        }
        else if((ReadsSP(second) || (second->code.front()=='b') || (second->code.back()==':'))){ // Reset the whole operation.
            first = second;
            goto start;
        }
        second++;
    }

    return removals;
}

int Optimize(Emitter *emit, unsigned level){
    int p = 0, n1, nt = 0;

    while((n1=RemoveRedundantStackLoadStores(emit->operations)+
        CombineImmediateMoves(emit->operations)+
        CombineLoadsDestructiveArithmetic(emit->operations)+
        RemoveDeadCodeAfterJump(emit->operations))) {nt+=n1; p++;}
    while((n1=RemoveRedundantSPChanges(emit->operations)+RemoveDeadCodeAfterJump(emit->operations))) {nt+=n1; p++;}  
    printf("# Optimizing level %i in %i passes, %i stepping.\n", level, p, nt);

    return 1;

}

}