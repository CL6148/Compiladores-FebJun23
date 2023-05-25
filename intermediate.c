#include <stdio.h>
#include <stdlib.h>
#include "stack.c"

struct StackNode* operator = NULL;
struct StackNode* operand = NULL;
struct StackNode* types = NULL;

void generateQuad() {
    int rightOperand;
    int rightType;

    int leftOperand;
    int leftType;

    int result;

/*
    int resultType =  semanticCheck (rType, lType, operator);
    if (resultType != ERR) {
        result = [next available temp];
        [generate quad]     [operator, lOperand, rOperand, result]
        [push quad to operation structure]      (print directly to file?)
        push(operandStack, result);
        push(typesStack, resultType);
    }
    else {
        ERR [Error code returned by semantic check]
    }

*/
}


/*
    1. PilaO.push(id.name) and PTypes.push(id.type)		[After reading ID]
    2. POper.push(+ or -)			            		[After reading + or -]
    3. POper.push(* or /)					            [After reading * or /]
    4. if POper.top() == + or -				            [After finishing reading TERM]
        rightOperand = PilaO.pop()
        rightType = PTypes.pop()
        leftOperand = PilaO.pop()
        leftType = PTypes.pop()
        operator = POper.pop()
        resultType = Semantics[leftType, rightType, operator]
        if (resultType != ERR)
            result <- AVAIL.next
            generateQuad(operator, left_operand, rightOperand, result)
            quad.push(quad)
            PilaO.push(result)
            PTypes.push(resultType)
            if any operands were a temporal space, return it to AVAIL
        else
            ERR TypeMismatch
    5. [Same as 4. , but using *,/ instead of +,-]		[After finishing reading FACTOR]
*/