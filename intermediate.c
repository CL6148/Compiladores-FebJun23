#include <stdio.h>
#include <stdlib.h>
#include "stack.c"

int quadCounter = 1;
int tempCounter = 1;

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
    
    FILE *f = fopen("instructions.txt", "a");

    fprintf(f, "%3d-\t%s\t", quadCounter, operToChar(oper));

    iROper = iPeek(operand);
    pop(&operand);

    iLOper = iPeek(operand);
    pop(&operand);

    fprintf(f, "%d\t", iLOper);
    fprintf(f, "%d\t", iROper);

    fprintf(f, "%d\n", tempCounter);

    fclose(f);

    // push quad to stack
    
    iPush(&operand, tempCounter++);
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

    FILE *f = fopen("instructions.txt", "a");

    fprintf(f, "%3d-\t%s\t", quadCounter, operToChar(oper));

    if (spaces == 1) {
        if (cPeek(operand) == "temp") {
            fprintf(f, "temp\t");
        }
        else {
            iOper = iPeek(operand);
            fprintf(f, "%d\t", iOper);
        }
        pop(&operand);
    }
    else {
        fprintf(f, "\t");
    }
    fprintf(f, "\t");

    iRes = iPeek(operand);
    fprintf(f, "%d\n", iRes);
    pop(&operand);
    
    fclose(f);

    // push quad to stack
    quadCounter++;
}
