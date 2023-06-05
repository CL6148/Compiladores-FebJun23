#include <stdio.h>
#include <stdlib.h>
#include "stack.c"

int quadCounter = 1;

struct StackNode* operator = NULL;
struct StackNode* operand = NULL;
struct StackNode* types = NULL;

char *operToChar(int oper) {
    char *operation;
    switch(oper) {
    case 1:
        operation = "+";
        break;
    case 2:
        operation = "-";
        break;
    case 3:
        operation = "*";
        break;
    case 4:
        operation = "/";
        break;
    case 5:
        operation = "&&";
        break;
    case 6:
        operation = "||";
        break;
    case 7:
        operation = "!=";
        break;
    case 8:
        operation = "==";
        break;
    case 9:
        operation = "<";
        break;
    case 10:
        operation = ">";
        break;
    case 11:
        operation = "<=";
        break;
    case 12:
        operation = ">=";
        break;
    case 13:
        operation = "=";
        break;
    case 14:
        operation = "read";
        break;
    case 15:
        operation = "write";
        break;
    default:
        operation = "Undef";
        break;
    }
    return operation;
}

void genQuad() {
    int oper, rType, lType;
    int iLOper, iROper;
    float fLOper, fROper;

    rType = iPeek(types);
    pop(&types);
    lType = iPeek(types);
    pop(&types);

    oper = iPeek(operator);
    pop(&operator);

    int resType = semantic_cube(oper, lType, rType);
    if (resType == -1) {
        printf("Type Mismatch\n");
        exit(1);
    }
    else if (resType == -2) {
        printf("Undefined Type\n");
        exit(1);
    }
    else if (resType == -3) {
        printf("Undefined Operator\n");
        exit(1);
    }
    printf("%3d-\t%s\t", quadCounter, operToChar(oper));

    if (cPeek(operand) == "temp") {
        rType = 0;
    }
    else if (rType == 1) {
        iROper = iPeek(operand);
    }
    else if (rType == 2) {
        fROper = fPeek(operand);
    }
    else if (rType == 3) {
        iROper = iPeek(operand);
    }
    pop(&operand);

    if (cPeek(operand) == "temp") {
        lType = 0;
    }
    else if (lType == 1) {
        iLOper = iPeek(operand);
        
    }
    else if (lType == 2) {
        fLOper = fPeek(operand);
    }
    else if (lType == 3) {
        fROper = iPeek(operand);
    }
    pop(&operand);

    if (lType == 0) {
        printf("temp\t");
    }
    else if (lType == 1 || lType == 3) {
        printf("%d\t", iLOper);
    }
    else {
        printf("%.2f\t", fLOper);
    }

    if (rType == 0) {
        printf("temp\t");
    }
    else if (rType == 1 || rType == 3) {
        printf("%d\t", iROper);
    }
    else {
        printf("%.2f\t", fROper);
    }

    char *result = "temp";
    printf("%s\n", result);

    // push quad to stack
    
    cPush(&operand, result);
    iPush(&types, resType);
    quadCounter++;
}

void genQuadLin(int spaces) {
    int oper, opType, resType;
    int iOper, iRes;
    float fOper, fRes;

    if (spaces == 1) {
        opType = iPeek(types);
        pop(&types);
    }
    resType = iPeek(types);
    pop(&types);

    oper = iPeek(operator);
    pop(&operator);

    if (spaces == 1) {
        resType = semantic_cube(oper, opType, resType);
        if (resType == -1) {
            printf("Type Mismatch\n");
            exit(1);
        }
        else if (resType == -2) {
            printf("Undefined Type\n");
            exit(1);
        }
        else if (resType == -3) {
            printf("Undefined Operator\n");
            exit(1);
        }
    }
    printf("%3d-\t%s\t", quadCounter, operToChar(oper));

    if (spaces == 1) {
        if (cPeek(operand) == "temp") {
            printf("temp\t");
        }
        else if (opType == 1) {
            iOper = iPeek(operand);
            printf("%d\t", iOper);
        }
        else if (opType == 2) {
            fOper = fPeek(operand);
            printf("%.2f\t", fOper);
        }
        pop(&operand);
    }
    else {
        printf("\t");
    }
    printf("\t");

    if (resType == 1) {
        iRes = iPeek(operand);
        printf("%d\n", iRes);
    }
    else {
        fRes = fPeek(operand);
        printf("%.2f\n", fRes);
    }
    pop(&operand);
    
    // push quad to stack
    quadCounter++;
}
