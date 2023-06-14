#include <stdio.h>
#include <stdlib.h>
#include "stack.c"

int quadCounter = 1;
int itempCounter = 30001;
int ftempCounter = 31001;
int btempCounter = 32001;

int op[500];
int opL[500];
int opR[500];
int qRes[500];

int ivalArr[500];
float fvalArr[500];
int iTemp[500];
float fTemp[500];
int bTemp[500];

struct StackNode* operator = NULL;
struct StackNode* operand = NULL;
struct StackNode* types = NULL;
struct StackNode* jumps = NULL;

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
    case 16:
        operation = "gotoF";
        break;
    case 17:
        operation = "goto";
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

    op[quadCounter-1] = oper;
    fprintf(f, "%3d-\t%s\t", quadCounter, operToChar(oper));

    iROper = iPeek(operand);
    pop(&operand);
    opR[quadCounter-1] = iROper;

    iLOper = iPeek(operand);
    pop(&operand);
    opL[quadCounter-1] = iLOper;

    fprintf(f, "%d\t", iLOper);
    fprintf(f, "%d\t", iROper);

    if (resType == 1) {
        fprintf(f, "%d\n", itempCounter);
        qRes[quadCounter-1] = itempCounter;
        iPush(&operand, itempCounter++);
    }
    else if (resType == 2) {
        fprintf(f, "%d\n", ftempCounter);
        qRes[quadCounter-1] = ftempCounter;
        iPush(&operand, ftempCounter++);
    }
    else if (resType == 3) {
        fprintf(f, "%d\n", btempCounter);
        qRes[quadCounter-1] = btempCounter;
        iPush(&operand, btempCounter++);
    }

    fclose(f);
    
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

    oper = iPeek(operator);
    pop(&operator);

    if (oper == 16 || oper == 17) {
        resType = 4;
        iPush(&jumps, quadCounter);
    }
    else {
        resType = iPeek(types);
        pop(&types);
    }

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

    op[quadCounter-1] = oper;
    fprintf(f, "%3d-\t%s\t", quadCounter, operToChar(oper));

    if (spaces == 1) {
        iOper = iPeek(operand);
        opL[quadCounter-1] = iOper;
        fprintf(f, "%d\t", iOper);
        pop(&operand);
    }
    else {
        fprintf(f, "\t");
    }
    fprintf(f, "\t");

    if (oper == 16 || oper == 17) {
        iRes = -999;
    }
    else {
        iRes = iPeek(operand);
        pop(&operand);
    }

    qRes[quadCounter-1] = iRes;
    fprintf(f, "%d\n", iRes);

    fclose(f);

    quadCounter++;
}

void executeCode(int *iConstants, float *fConstants) {
    for (int i = 0; i < quadCounter; i++) {
        switch (op[i]) {
        case 1:
            if (qRes[i] > 20000 && qRes[i] < 21000) {
                if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                }
                if (opL[i] > 11000 && opL[i] < 13000) {
                    iTemp[qRes[i]-20001] = ivalArr[opL[i]-11001] + opR[i];
                }
                else if (opL[i] > 15000 && opL[i] < 17000) {
                    iTemp[qRes[i]-20001] = iConstants[opL[i]-15001] + opR[i];
                }
                else if (opL[i] > 30000 && opL[i] < 31000) {
                    iTemp[qRes[i]-20001] = iTemp[opL[i]-30001] + opR[i];
                }
            }
            else if (qRes[i] > 21000 && qRes[i] < 22000) {
                if (opL[i] > 11000 && opL[i] < 13000) {
                    iTemp[qRes[i]-21001] = ivalArr[opL[i]-11001] + opR[i];
                }
                else if (opL[i] > 15000 && opL[i] < 17000) {
                    iTemp[qRes[i]-21001] = iConstants[opL[i]-15001] + opR[i];
                }
                else if (opL[i] > 30000 && opL[i] < 31000) {
                    iTemp[qRes[i]-21001] = iTemp[opL[i]-30001] + opR[i];
                }
            }
            else {
                if (opL[i] > 11000 && opL[i] < 13000) {
                    if (opR[i] > 11000 && opR[i] < 13000) {
                        iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] + ivalArr[opR[i]-11001];
                    }
                    else if (opR[i] > 13000 && opR[i] < 15000) {
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] + fvalArr[opR[i]-13001];
                    }
                    else if (opR[i] > 15000 && opR[i] < 17000) {
                        iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] + iConstants[opR[i]-15001];
                    }
                    else if (opR[i] > 17000 && opR[i] < 19000) {
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] + fConstants[opR[i]-17001];
                    }
                    else if (opR[i] > 30000 && opR[i] < 31000) {
                        iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] + iTemp[opR[i]-30001];
                    }
                    else if (opR[i] > 31000 && opR[i] < 32000) {
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] + fTemp[opR[i]-31001];
                    }
                    else if (opR[i] > 20000 && opR[i] < 21000) {
                        opR[i] = opR[i] - 9000;
                        iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] + ivalArr[opR[i]-11000];
                    }
                    else if (opR[i] > 21000 && opR[i] < 22000) {
                        opR[i] = opR[i] - 8000;
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] + fvalArr[opR[i]-13000];
                    }
                }
                else if (opL[i] > 13000 && opL[i] < 15000) {
                    if (opR[i] > 11000 && opR[i] < 13000) {
                        fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] + ivalArr[opR[i]-11001];
                    }
                    else if (opR[i] > 13000 && opR[i] < 15000) {
                        fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] + fvalArr[opR[i]-13001];
                    }
                    else if (opR[i] > 15000 && opR[i] < 17000) {
                        fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] + iConstants[opR[i]-15001];
                    }
                    else if (opR[i] > 17000 && opR[i] < 19000) {
                        fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] + fConstants[opR[i]-17001];
                    }
                    else if (opR[i] > 30000 && opR[i] < 31000) {
                        fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] + iTemp[opR[i]-30001];
                    }
                    else if (opR[i] > 31000 && opR[i] < 32000) {
                        fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] + fTemp[opR[i]-31001];
                    }
                    else if (opR[i] > 20000 && opR[i] < 21000) {
                        opR[i] = opR[i] - 9000;
                        fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] + ivalArr[opR[i]-11000];
                    }
                    else if (opR[i] > 21000 && opR[i] < 22000) {
                        opR[i] = opR[i] - 8000;
                        fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] + fvalArr[opR[i]-13000];
                    }
                }
                else if (opL[i] > 15000 && opL[i] < 17000) {
                    if (opR[i] > 11000 && opR[i] < 13000) {
                        iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] + ivalArr[opR[i]-11001];
                    }
                    else if (opR[i] > 13000 && opR[i] < 15000) {
                        fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] + fvalArr[opR[i]-13001];
                    }
                    else if (opR[i] > 15000 && opR[i] < 17000) {
                        iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] + iConstants[opR[i]-15001];
                    }
                    else if (opR[i] > 17000 && opR[i] < 19000) {
                        fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] + fConstants[opR[i]-17001];
                    }
                    else if (opR[i] > 30000 && opR[i] < 31000) {
                        iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] + iTemp[opR[i]-30001];
                    }
                    else if (opR[i] > 31000 && opR[i] < 32000) {
                        fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] + fTemp[opR[i]-31001];
                    }
                    else if (opR[i] > 20000 && opR[i] < 21000) {
                        opR[i] = opR[i] - 9000;
                        iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] + ivalArr[opR[i]-11000];
                    }
                    else if (opR[i] > 21000 && opR[i] < 22000) {
                        opR[i] = opR[i] - 8000;
                        fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] + fvalArr[opR[i]-13000];
                    }
                }
                else if (opL[i] > 17000 && opL[i] < 19000) {
                    if (opR[i] > 11000 && opR[i] < 13000) {
                        fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] + ivalArr[opR[i]-11001];
                    }
                    else if (opR[i] > 13000 && opR[i] < 15000) {
                        fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] + fvalArr[opR[i]-13001];
                    }
                    else if (opR[i] > 15000 && opR[i] < 17000) {
                        fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] + iConstants[opR[i]-15001];
                    }
                    else if (opR[i] > 17000 && opR[i] < 19000) {
                        fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] + fConstants[opR[i]-17001];
                    }
                    else if (opR[i] > 30000 && opR[i] < 31000) {
                        fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] + iTemp[opR[i]-30001];
                    }
                    else if (opR[i] > 31000 && opR[i] < 32000) {
                        fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] + fTemp[opR[i]-31001];
                    }
                    else if (opR[i] > 20000 && opR[i] < 21000) {
                        opR[i] = opR[i] - 9000;
                        fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] + ivalArr[opR[i]-11000];
                    }
                    else if (opR[i] > 21000 && opR[i] < 22000) {
                        opR[i] = opR[i] - 8000;
                        fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] + fvalArr[opR[i]-13000];
                    }
                }
                else if (opL[i] > 30000 && opL[i] < 31000) {
                    if (opR[i] > 11000 && opR[i] < 13000) {
                        iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] + ivalArr[opR[i]-11001];
                    }
                    else if (opR[i] > 13000 && opR[i] < 15000) {
                        fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] + fvalArr[opR[i]-13001];
                    }
                    else if (opR[i] > 15000 && opR[i] < 17000) {
                        iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] + iConstants[opR[i]-15001];
                    }
                    else if (opR[i] > 17000 && opR[i] < 19000) {
                        fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] + fConstants[opR[i]-17001];
                    }
                    else if (opR[i] > 30000 && opR[i] < 31000) {
                        iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] + iTemp[opR[i]-30001];
                    }
                    else if (opR[i] > 31000 && opR[i] < 32000) {
                        fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] + fTemp[opR[i]-31001];
                    }
                    else if (opR[i] > 20000 && opR[i] < 21000) {
                        opR[i] = opR[i] - 9000;
                        iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] + ivalArr[opR[i]-11000];
                    }
                    else if (opR[i] > 21000 && opR[i] < 22000) {
                        opR[i] = opR[i] - 8000;
                        fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] + fvalArr[opR[i]-13000];
                    }
                }
                else if (opL[i] > 31000 && opL[i] < 32000) {
                    if (opR[i] > 11000 && opR[i] < 13000) {
                        fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] + ivalArr[opR[i]-11001];
                    }
                    else if (opR[i] > 13000 && opR[i] < 15000) {
                        fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] + fvalArr[opR[i]-13001];
                    }
                    else if (opR[i] > 15000 && opR[i] < 17000) {
                        fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] + iConstants[opR[i]-15001];
                    }
                    else if (opR[i] > 17000 && opR[i] < 19000) {
                        fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] + fConstants[opR[i]-17001];
                    }
                    else if (opR[i] > 30000 && opR[i] < 31000) {
                        fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] + iTemp[opR[i]-30001];
                    }
                    else if (opR[i] > 31000 && opR[i] < 32000) {
                        fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] + fTemp[opR[i]-31001];
                    }
                    else if (opR[i] > 20000 && opR[i] < 21000) {
                        opR[i] = opR[i] - 9000;
                        fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] + ivalArr[opR[i]-11000];
                    }
                    else if (opR[i] > 21000 && opR[i] < 22000) {
                        opR[i] = opR[i] - 8000;
                        fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] + fvalArr[opR[i]-13000];
                    }
                }
                else if (opL[i] > 20000 && opL[i] < 21000) {
                    opL[i] = opL[i] - 9000;
                    if (opR[i] > 11000 && opR[i] < 13000) {
                        iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] + ivalArr[opR[i]-11001];
                    }
                    else if (opR[i] > 13000 && opR[i] < 15000) {
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] + fvalArr[opR[i]-13001];
                    }
                    else if (opR[i] > 15000 && opR[i] < 17000) {
                        iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] + iConstants[opR[i]-15001];
                    }
                    else if (opR[i] > 17000 && opR[i] < 19000) {
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] + fConstants[opR[i]-17001];
                    }
                    else if (opR[i] > 30000 && opR[i] < 31000) {
                        iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] + iTemp[opR[i]-30001];
                    }
                    else if (opR[i] > 31000 && opR[i] < 32000) {
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] + fTemp[opR[i]-31001];
                    }
                    else if (opR[i] > 20000 && opR[i] < 21000) {
                        opR[i] = opR[i] - 9000;
                        iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] + ivalArr[opR[i]-11000];
                    }
                    else if (opR[i] > 21000 && opR[i] < 22000) {
                        opR[i] = opR[i] - 9000;
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] + fvalArr[opR[i]-13000];
                    }
                }
                else if (opL[i] > 21000 && opL[i] < 22000) {
                    opL[i] = opL[i] - 8000;
                    if (opR[i] > 11000 && opR[i] < 13000) {
                        iTemp[qRes[i]-30001] = ivalArr[opL[i]-13000] + ivalArr[opR[i]-11001];
                    }
                    else if (opR[i] > 13000 && opR[i] < 15000) {
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] + fvalArr[opR[i]-13001];
                    }
                    else if (opR[i] > 15000 && opR[i] < 17000) {
                        iTemp[qRes[i]-30001] = ivalArr[opL[i]-13000] + iConstants[opR[i]-15001];
                    }
                    else if (opR[i] > 17000 && opR[i] < 19000) {
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] + fConstants[opR[i]-17001];
                    }
                    else if (opR[i] > 30000 && opR[i] < 31000) {
                        iTemp[qRes[i]-30001] = ivalArr[opL[i]-13000] + iTemp[opR[i]-30001];
                    }
                    else if (opR[i] > 31000 && opR[i] < 32000) {
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] + fTemp[opR[i]-31001];
                    }
                    else if (opR[i] > 20000 && opR[i] < 21000) {
                        opR[i] = opR[i] - 9000;
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] + ivalArr[opR[i]-11000];
                    }
                    else if (opR[i] > 21000 && opR[i] < 22000) {
                        opR[i] = opR[i] - 9000;
                        fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] + fvalArr[opR[i]-13000];
                    }
                }
            }
            break;
        case 2:
            if (opL[i] > 11000 && opL[i] < 13000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] - ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] - fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] - iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] - fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] - iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] - fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] - ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] - fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 13000 && opL[i] < 15000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] - ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] - fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] - iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] - fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] - iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] - fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] - ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] - fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 15000 && opL[i] < 17000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] - ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] - fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] - iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] - fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] - iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] - fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] - ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] - fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 17000 && opL[i] < 19000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] - ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] - fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] - iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] - fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] - iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] - fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] - ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] - fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 30000 && opL[i] < 31000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] - ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] - fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] - iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] - fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] - iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] - fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] - ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] - fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 31000 && opL[i] < 32000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] - ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] - fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] - iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] - fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] - iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] - fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] - ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] - fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 20000 && opL[i] < 21000) {
                opL[i] = opL[i] - 9000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] - ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] - fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] - iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] - fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] - iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] - fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] - ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] - fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 21000 && opL[i] < 22000) {
                opL[i] = opL[i] - 8000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-13000] - ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] - fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-13000] - iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] - fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-13000] - iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] - fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] - ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] - fvalArr[opR[i]-13000];
                }
            }
            break;
        case 3:
            if (opL[i] > 11000 && opL[i] < 13000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] * ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] * fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] * iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] * fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] * iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] * fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] * ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] * fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 13000 && opL[i] < 15000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] * ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] * fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] * iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] * fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] * iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] * fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] * ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] * fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 15000 && opL[i] < 17000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] * ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] * fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] * iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] * fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] * iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] * fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] * ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] * fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 17000 && opL[i] < 19000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] * ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] * fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] * iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] * fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] * iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] * fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] * ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] * fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 30000 && opL[i] < 31000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] * ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] * fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] * iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] * fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] * iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] * fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] * ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] * fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 31000 && opL[i] < 32000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] * ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] * fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] * iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] * fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] * iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] * fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] * ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] * fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 20000 && opL[i] < 21000) {
                opL[i] = opL[i] - 9000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] * ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] * fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] * iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] * fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] * iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] * fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] * ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] * fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 21000 && opL[i] < 22000) {
                opL[i] = opL[i] - 8000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-13000] * ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] * fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-13000] * iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] * fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-13000] * iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] * fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] * ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] * fvalArr[opR[i]-13000];
                }
            }
            break;
        case 4:
            if (opL[i] > 11000 && opL[i] < 13000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] / ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] / fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] / iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] / fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] / iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] / fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11001] / ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11001] / fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 13000 && opL[i] < 15000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] / ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] / fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] / iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] / fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] / iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] / fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] / ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = fvalArr[opL[i]-13001] / fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 15000 && opL[i] < 17000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] / ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] / fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] / iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] / fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] / iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] / fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    iTemp[qRes[i]-30001] = iConstants[opL[i]-15001] / ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = iConstants[opL[i]-15001] / fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 17000 && opL[i] < 19000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] / ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] / fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] / iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] / fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] / iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] / fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] / ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = fConstants[opL[i]-17001] / fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 30000 && opL[i] < 31000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] / ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] / fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] / iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] / fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] / iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] / fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    iTemp[qRes[i]-30001] = iTemp[opL[i]-30001] / ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = iTemp[opL[i]-30001] / fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 31000 && opL[i] < 32000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] / ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] / fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] / iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] / fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] / iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] / fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] / ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    fTemp[qRes[i]-31001] = fTemp[opL[i]-31001] / fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 20000 && opL[i] < 21000) {
                opL[i] = opL[i] - 9000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] / ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] / fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] / iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] / fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] / iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] / fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-11000] / ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-11000] / fvalArr[opR[i]-13000];
                }
            }
            else if (opL[i] > 21000 && opL[i] < 22000) {
                opL[i] = opL[i] - 8000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-13000] / ivalArr[opR[i]-11001];
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] / fvalArr[opR[i]-13001];
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-13000] / iConstants[opR[i]-15001];
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] / fConstants[opR[i]-17001];
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    iTemp[qRes[i]-30001] = ivalArr[opL[i]-13000] / iTemp[opR[i]-30001];
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] / fTemp[opR[i]-31001];
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] / ivalArr[opR[i]-11000];
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 9000;
                    fTemp[qRes[i]-31001] = ivalArr[opL[i]-13000] / fvalArr[opR[i]-13000];
                }
            }
            break;
        case 5:
            if (bTemp[opL[i]-32001] == 0) {
                if (bTemp[opR[i]-32001] == 0) {
                    bTemp[qRes[i]-32001] = 0;
                }
                else if (bTemp[opR[i]-32001] == 1) {
                    bTemp[qRes[i]-32001] = 0;
                }
            }
            else if (bTemp[opL[i]-32001] == 1) {
                if (bTemp[opR[i]-32001] == 0) {
                    bTemp[qRes[i]-32001] = 0;
                }
                else if (bTemp[opR[i]-32001] == 1) {
                    bTemp[qRes[i]-32001] = 1;
                }
            }
            break;
        case 6:
            if (bTemp[opL[i]-32001] == 0) {
                if (bTemp[opR[i]-32001] == 0) {
                    bTemp[qRes[i]-32001] = 0;
                }
                else if (bTemp[opR[i]-32001] == 1) {
                    bTemp[qRes[i]-32001] = 1;
                }
            }
            else if (bTemp[opL[i]-32001] == 1) {
                if (bTemp[opR[i]-32001] == 0) {
                    bTemp[qRes[i]-32001] = 1;
                }
                else if (bTemp[opR[i]-32001] == 1) {
                    bTemp[qRes[i]-32001] = 1;
                }
            }
            break;
        case 7:
            if (opL[i] > 11000 && opL[i] < 13000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (ivalArr[opL[i]-11001] != ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (ivalArr[opL[i]-11001] != fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (ivalArr[opL[i]-11001] != iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (ivalArr[opL[i]-11001] != fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (ivalArr[opL[i]-11001] != iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (ivalArr[opL[i]-11001] != fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (ivalArr[opL[i]-11001] != ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (ivalArr[opL[i]-11001] != ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 13000 && opL[i] < 15000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fvalArr[opL[i]-13001] != ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fvalArr[opL[i]-13001] != fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fvalArr[opL[i]-13001] != iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fvalArr[opL[i]-13001] != fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fvalArr[opL[i]-13001] != iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fvalArr[opL[i]-13001] != fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fvalArr[opL[i]-13001] != ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fvalArr[opL[i]-13001] != ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 15000 && opL[i] < 17000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (iConstants[opL[i]-15001] != ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (iConstants[opL[i]-15001] != fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (iConstants[opL[i]-15001] != iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (iConstants[opL[i]-15001] != fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (iConstants[opL[i]-15001] != iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (iConstants[opL[i]-15001] != fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (iConstants[opL[i]-15001] != ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (iConstants[opL[i]-15001] != ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 17000 && opL[i] < 19000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fConstants[opL[i]-17001] != ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fConstants[opL[i]-17001] != fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fConstants[opL[i]-17001] != iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fConstants[opL[i]-17001] != fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fConstants[opL[i]-17001] != iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fConstants[opL[i]-17001] != fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fConstants[opL[i]-17001] != ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fConstants[opL[i]-17001] != ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 30000 && opL[i] < 31000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (iTemp[opL[i]-30001] != ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (iTemp[opL[i]-30001] != fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (iTemp[opL[i]-30001] != iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (iTemp[opL[i]-30001] != fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (iTemp[opL[i]-30001] != iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (iTemp[opL[i]-30001] != fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (iTemp[opL[i]-30001] != ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (iTemp[opL[i]-30001] != ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 31000 && opL[i] < 32000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fTemp[opL[i]-31001] != ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fTemp[opL[i]-31001] != fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fTemp[opL[i]-31001] != iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fTemp[opL[i]-31001] != fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fTemp[opL[i]-31001] != iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fTemp[opL[i]-31001] != fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fTemp[opL[i]-31001] != ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fTemp[opL[i]-31001] != ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 20000 && opL[i] < 21000) {
                opL[i] = opL[i] - 9000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (ivalArr[opL[i]-11000] != ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (ivalArr[opL[i]-11000] != fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (ivalArr[opL[i]-11000] != iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (ivalArr[opL[i]-11000] != fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (ivalArr[opL[i]-11000] != iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (ivalArr[opL[i]-11000] != fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (ivalArr[opL[i]-11000] != ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (ivalArr[opL[i]-11000] != ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 21000 && opL[i] < 22000) {
                opL[i] = opL[i] - 8000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fvalArr[opL[i]-13000] != ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fvalArr[opL[i]-13000] != fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fvalArr[opL[i]-13000] != iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fvalArr[opL[i]-13000] != fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fvalArr[opL[i]-13000] != iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fvalArr[opL[i]-13000] != fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fvalArr[opL[i]-13000] != ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fvalArr[opL[i]-13000] != ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            break;
        case 8:
            if (opL[i] > 11000 && opL[i] < 13000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (ivalArr[opL[i]-11001] == ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (ivalArr[opL[i]-11001] == fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (ivalArr[opL[i]-11001] == iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (ivalArr[opL[i]-11001] == fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (ivalArr[opL[i]-11001] == iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (ivalArr[opL[i]-11001] == fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (ivalArr[opL[i]-11001] == ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (ivalArr[opL[i]-11001] == ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 13000 && opL[i] < 15000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fvalArr[opL[i]-13001] == ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fvalArr[opL[i]-13001] == fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fvalArr[opL[i]-13001] == iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fvalArr[opL[i]-13001] == fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fvalArr[opL[i]-13001] == iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fvalArr[opL[i]-13001] == fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fvalArr[opL[i]-13001] == ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fvalArr[opL[i]-13001] == ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 15000 && opL[i] < 17000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (iConstants[opL[i]-15001] == ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (iConstants[opL[i]-15001] == fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (iConstants[opL[i]-15001] == iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (iConstants[opL[i]-15001] == fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (iConstants[opL[i]-15001] == iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (iConstants[opL[i]-15001] == fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (iConstants[opL[i]-15001] == ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (iConstants[opL[i]-15001] == ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 17000 && opL[i] < 19000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fConstants[opL[i]-17001] == ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fConstants[opL[i]-17001] == fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fConstants[opL[i]-17001] == iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fConstants[opL[i]-17001] == fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fConstants[opL[i]-17001] == iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fConstants[opL[i]-17001] == fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fConstants[opL[i]-17001] == ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fConstants[opL[i]-17001] == ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 30000 && opL[i] < 31000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (iTemp[opL[i]-30001] == ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (iTemp[opL[i]-30001] == fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (iTemp[opL[i]-30001] == iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (iTemp[opL[i]-30001] == fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (iTemp[opL[i]-30001] == iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (iTemp[opL[i]-30001] == fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (iTemp[opL[i]-30001] == ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (iTemp[opL[i]-30001] == ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 31000 && opL[i] < 32000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fTemp[opL[i]-31001] == ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fTemp[opL[i]-31001] == fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fTemp[opL[i]-31001] == iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fTemp[opL[i]-31001] == fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fTemp[opL[i]-31001] == iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fTemp[opL[i]-31001] == fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fTemp[opL[i]-31001] == ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fTemp[opL[i]-31001] == ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 20000 && opL[i] < 21000) {
                opL[i] = opL[i] - 9000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (ivalArr[opL[i]-11000] == ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (ivalArr[opL[i]-11000] == fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (ivalArr[opL[i]-11000] == iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (ivalArr[opL[i]-11000] == fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (ivalArr[opL[i]-11000] == iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (ivalArr[opL[i]-11000] == fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (ivalArr[opL[i]-11000] == ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (ivalArr[opL[i]-11000] == ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 21000 && opL[i] < 22000) {
                opL[i] = opL[i] - 8000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fvalArr[opL[i]-13000] == ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fvalArr[opL[i]-13000] == fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fvalArr[opL[i]-13000] == iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fvalArr[opL[i]-13000] == fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fvalArr[opL[i]-13000] == iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fvalArr[opL[i]-13000] == fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fvalArr[opL[i]-13000] == ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fvalArr[opL[i]-13000] == ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            break;
        case 9:
            if (opL[i] > 11000 && opL[i] < 13000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (ivalArr[opL[i]-11001] < ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (ivalArr[opL[i]-11001] < fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (ivalArr[opL[i]-11001] < iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (ivalArr[opL[i]-11001] < fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (ivalArr[opL[i]-11001] < iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (ivalArr[opL[i]-11001] < fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (ivalArr[opL[i]-11001] < ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (ivalArr[opL[i]-11001] < ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 13000 && opL[i] < 15000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fvalArr[opL[i]-13001] < ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fvalArr[opL[i]-13001] < fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fvalArr[opL[i]-13001] < iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fvalArr[opL[i]-13001] < fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fvalArr[opL[i]-13001] < iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fvalArr[opL[i]-13001] < fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fvalArr[opL[i]-13001] < ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fvalArr[opL[i]-13001] < ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 15000 && opL[i] < 17000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (iConstants[opL[i]-15001] < ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (iConstants[opL[i]-15001] < fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (iConstants[opL[i]-15001] < iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (iConstants[opL[i]-15001] < fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (iConstants[opL[i]-15001] < iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (iConstants[opL[i]-15001] < fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (iConstants[opL[i]-15001] < ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (iConstants[opL[i]-15001] < ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 17000 && opL[i] < 19000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fConstants[opL[i]-17001] < ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fConstants[opL[i]-17001] < fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fConstants[opL[i]-17001] < iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fConstants[opL[i]-17001] < fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fConstants[opL[i]-17001] < iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fConstants[opL[i]-17001] < fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fConstants[opL[i]-17001] < ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fConstants[opL[i]-17001] < ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 30000 && opL[i] < 31000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (iTemp[opL[i]-30001] < ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (iTemp[opL[i]-30001] < fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (iTemp[opL[i]-30001] < iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (iTemp[opL[i]-30001] < fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (iTemp[opL[i]-30001] < iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (iTemp[opL[i]-30001] < fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (iTemp[opL[i]-30001] < ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (iTemp[opL[i]-30001] < ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 31000 && opL[i] < 32000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fTemp[opL[i]-31001] < ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fTemp[opL[i]-31001] < fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fTemp[opL[i]-31001] < iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fTemp[opL[i]-31001] < fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fTemp[opL[i]-31001] < iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fTemp[opL[i]-31001] < fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fTemp[opL[i]-31001] < ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fTemp[opL[i]-31001] < ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 20000 && opL[i] < 21000) {
                opL[i] = opL[i] - 9000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (ivalArr[opL[i]-11000] < ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (ivalArr[opL[i]-11000] < fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (ivalArr[opL[i]-11000] < iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (ivalArr[opL[i]-11000] < fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (ivalArr[opL[i]-11000] < iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (ivalArr[opL[i]-11000] < fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (ivalArr[opL[i]-11000] < ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (ivalArr[opL[i]-11000] < ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 21000 && opL[i] < 22000) {
                opL[i] = opL[i] - 8000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fvalArr[opL[i]-13000] < ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fvalArr[opL[i]-13000] < fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fvalArr[opL[i]-13000] < iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fvalArr[opL[i]-13000] < fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fvalArr[opL[i]-13000] < iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fvalArr[opL[i]-13000] < fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fvalArr[opL[i]-13000] < ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fvalArr[opL[i]-13000] < ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            break;
        case 10:
            if (opL[i] > 11000 && opL[i] < 13000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (ivalArr[opL[i]-11001] > ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (ivalArr[opL[i]-11001] > fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (ivalArr[opL[i]-11001] > iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (ivalArr[opL[i]-11001] > fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (ivalArr[opL[i]-11001] > iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (ivalArr[opL[i]-11001] > fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (ivalArr[opL[i]-11001] > ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (ivalArr[opL[i]-11001] > ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 13000 && opL[i] < 15000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fvalArr[opL[i]-13001] > ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fvalArr[opL[i]-13001] > fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fvalArr[opL[i]-13001] > iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fvalArr[opL[i]-13001] > fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fvalArr[opL[i]-13001] > iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fvalArr[opL[i]-13001] > fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fvalArr[opL[i]-13001] > ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fvalArr[opL[i]-13001] > ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 15000 && opL[i] < 17000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (iConstants[opL[i]-15001] > ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (iConstants[opL[i]-15001] > fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (iConstants[opL[i]-15001] > iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (iConstants[opL[i]-15001] > fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (iConstants[opL[i]-15001] > iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (iConstants[opL[i]-15001] > fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (iConstants[opL[i]-15001] > ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (iConstants[opL[i]-15001] > ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 17000 && opL[i] < 19000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fConstants[opL[i]-17001] > ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fConstants[opL[i]-17001] > fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fConstants[opL[i]-17001] > iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fConstants[opL[i]-17001] > fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fConstants[opL[i]-17001] > iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fConstants[opL[i]-17001] > fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fConstants[opL[i]-17001] > ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fConstants[opL[i]-17001] > ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 30000 && opL[i] < 31000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (iTemp[opL[i]-30001] > ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (iTemp[opL[i]-30001] > fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (iTemp[opL[i]-30001] > iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (iTemp[opL[i]-30001] > fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (iTemp[opL[i]-30001] > iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (iTemp[opL[i]-30001] > fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (iTemp[opL[i]-30001] > ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (iTemp[opL[i]-30001] > ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 31000 && opL[i] < 32000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fTemp[opL[i]-31001] > ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fTemp[opL[i]-31001] > fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fTemp[opL[i]-31001] > iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fTemp[opL[i]-31001] > fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fTemp[opL[i]-31001] > iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fTemp[opL[i]-31001] > fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fTemp[opL[i]-31001] > ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fTemp[opL[i]-31001] > ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 20000 && opL[i] < 21000) {
                opL[i] = opL[i] - 9000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (ivalArr[opL[i]-11000] > ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (ivalArr[opL[i]-11000] > fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (ivalArr[opL[i]-11000] > iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (ivalArr[opL[i]-11000] > fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (ivalArr[opL[i]-11000] > iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (ivalArr[opL[i]-11000] > fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (ivalArr[opL[i]-11000] > ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (ivalArr[opL[i]-11000] > ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 21000 && opL[i] < 22000) {
                opL[i] = opL[i] - 8000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fvalArr[opL[i]-13000] > ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fvalArr[opL[i]-13000] > fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fvalArr[opL[i]-13000] > iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fvalArr[opL[i]-13000] > fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fvalArr[opL[i]-13000] > iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fvalArr[opL[i]-13000] > fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fvalArr[opL[i]-13000] > ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fvalArr[opL[i]-13000] > ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            break;
        case 11:
            if (opL[i] > 11000 && opL[i] < 13000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (ivalArr[opL[i]-11001] <= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (ivalArr[opL[i]-11001] <= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (ivalArr[opL[i]-11001] <= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (ivalArr[opL[i]-11001] <= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (ivalArr[opL[i]-11001] <= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (ivalArr[opL[i]-11001] <= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (ivalArr[opL[i]-11001] <= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (ivalArr[opL[i]-11001] <= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 13000 && opL[i] < 15000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fvalArr[opL[i]-13001] <= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fvalArr[opL[i]-13001] <= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fvalArr[opL[i]-13001] <= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fvalArr[opL[i]-13001] <= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fvalArr[opL[i]-13001] <= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fvalArr[opL[i]-13001] <= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fvalArr[opL[i]-13001] <= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fvalArr[opL[i]-13001] <= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 15000 && opL[i] < 17000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (iConstants[opL[i]-15001] <= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (iConstants[opL[i]-15001] <= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (iConstants[opL[i]-15001] <= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (iConstants[opL[i]-15001] <= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (iConstants[opL[i]-15001] <= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (iConstants[opL[i]-15001] <= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (iConstants[opL[i]-15001] <= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (iConstants[opL[i]-15001] <= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 17000 && opL[i] < 19000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fConstants[opL[i]-17001] <= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fConstants[opL[i]-17001] <= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fConstants[opL[i]-17001] <= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fConstants[opL[i]-17001] <= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fConstants[opL[i]-17001] <= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fConstants[opL[i]-17001] <= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fConstants[opL[i]-17001] <= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fConstants[opL[i]-17001] <= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 30000 && opL[i] < 31000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (iTemp[opL[i]-30001] <= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (iTemp[opL[i]-30001] <= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (iTemp[opL[i]-30001] <= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (iTemp[opL[i]-30001] <= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (iTemp[opL[i]-30001] <= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (iTemp[opL[i]-30001] <= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (iTemp[opL[i]-30001] <= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (iTemp[opL[i]-30001] <= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 31000 && opL[i] < 32000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fTemp[opL[i]-31001] <= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fTemp[opL[i]-31001] <= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fTemp[opL[i]-31001] <= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fTemp[opL[i]-31001] <= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fTemp[opL[i]-31001] <= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fTemp[opL[i]-31001] <= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fTemp[opL[i]-31001] <= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fTemp[opL[i]-31001] <= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 20000 && opL[i] < 21000) {
                opL[i] = opL[i] - 9000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (ivalArr[opL[i]-11000] <= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (ivalArr[opL[i]-11000] <= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (ivalArr[opL[i]-11000] <= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (ivalArr[opL[i]-11000] <= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (ivalArr[opL[i]-11000] <= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (ivalArr[opL[i]-11000] <= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (ivalArr[opL[i]-11000] <= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (ivalArr[opL[i]-11000] <= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 21000 && opL[i] < 22000) {
                opL[i] = opL[i] - 8000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fvalArr[opL[i]-13000] <= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fvalArr[opL[i]-13000] <= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fvalArr[opL[i]-13000] <= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fvalArr[opL[i]-13000] <= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fvalArr[opL[i]-13000] <= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fvalArr[opL[i]-13000] <= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fvalArr[opL[i]-13000] <= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fvalArr[opL[i]-13000] <= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            break;
        case 12:
            if (opL[i] > 11000 && opL[i] < 13000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (ivalArr[opL[i]-11001] >= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (ivalArr[opL[i]-11001] >= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (ivalArr[opL[i]-11001] >= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (ivalArr[opL[i]-11001] >= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (ivalArr[opL[i]-11001] >= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (ivalArr[opL[i]-11001] >= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (ivalArr[opL[i]-11001] >= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (ivalArr[opL[i]-11001] >= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 13000 && opL[i] < 15000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fvalArr[opL[i]-13001] >= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fvalArr[opL[i]-13001] >= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fvalArr[opL[i]-13001] >= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fvalArr[opL[i]-13001] >= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fvalArr[opL[i]-13001] >= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fvalArr[opL[i]-13001] >= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fvalArr[opL[i]-13001] >= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fvalArr[opL[i]-13001] >= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 15000 && opL[i] < 17000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (iConstants[opL[i]-15001] >= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (iConstants[opL[i]-15001] >= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (iConstants[opL[i]-15001] >= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (iConstants[opL[i]-15001] >= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (iConstants[opL[i]-15001] >= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (iConstants[opL[i]-15001] >= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (iConstants[opL[i]-15001] >= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (iConstants[opL[i]-15001] >= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 17000 && opL[i] < 19000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fConstants[opL[i]-17001] >= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fConstants[opL[i]-17001] >= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fConstants[opL[i]-17001] >= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fConstants[opL[i]-17001] >= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fConstants[opL[i]-17001] >= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fConstants[opL[i]-17001] >= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fConstants[opL[i]-17001] >= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fConstants[opL[i]-17001] >= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 30000 && opL[i] < 31000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (iTemp[opL[i]-30001] >= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (iTemp[opL[i]-30001] >= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (iTemp[opL[i]-30001] >= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (iTemp[opL[i]-30001] >= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (iTemp[opL[i]-30001] >= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (iTemp[opL[i]-30001] >= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (iTemp[opL[i]-30001] >= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (iTemp[opL[i]-30001] >= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 31000 && opL[i] < 32000) {
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fTemp[opL[i]-31001] >= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fTemp[opL[i]-31001] >= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fTemp[opL[i]-31001] >= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fTemp[opL[i]-31001] >= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fTemp[opL[i]-31001] >= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fTemp[opL[i]-31001] >= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fTemp[opL[i]-31001] >= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fTemp[opL[i]-31001] >= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 20000 && opL[i] < 21000) {
                opL[i] = opL[i] - 9000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (ivalArr[opL[i]-11000] >= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (ivalArr[opL[i]-11000] >= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (ivalArr[opL[i]-11000] >= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (ivalArr[opL[i]-11000] >= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (ivalArr[opL[i]-11000] >= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (ivalArr[opL[i]-11000] >= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (ivalArr[opL[i]-11000] >= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (ivalArr[opL[i]-11000] >= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            else if (opL[i] > 21000 && opL[i] < 22000) {
                opL[i] = opL[i] - 8000;
                if (opR[i] > 11000 && opR[i] < 13000) {
                    if (fvalArr[opL[i]-13000] >= ivalArr[opR[i]-11001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 13000 && opR[i] < 15000) {
                    if (fvalArr[opL[i]-13000] >= fvalArr[opR[i]-13001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 15000 && opR[i] < 17000) {
                    if (fvalArr[opL[i]-13000] >= iConstants[opR[i]-15001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 17000 && opR[i] < 19000) {
                    if (fvalArr[opL[i]-13000] >= fConstants[opR[i]-17001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 30000 && opR[i] < 31000) {
                    if (fvalArr[opL[i]-13000] >= iTemp[opR[i]-30001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 31000 && opR[i] < 32000) {
                    if (fvalArr[opL[i]-13000] >= fTemp[opR[i]-31001]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 20000 && opR[i] < 21000) {
                    opR[i] = opR[i] - 9000;
                    if (fvalArr[opL[i]-13000] >= ivalArr[opR[i]-11000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
                else if (opR[i] > 21000 && opR[i] < 22000) {
                    opR[i] = opR[i] - 8000;
                    if (fvalArr[opL[i]-13000] >= ivalArr[opR[i]-13000]) {
                        bTemp[qRes[i]-32001] = 1;
                    }
                    else {
                        bTemp[qRes[i]-32001] = 0;
                    }
                }
            }
            break;
        case 13:
            if (qRes[i] > 20000 && qRes[i] < 21000) {
                int iAux = iTemp[qRes[i]-20001];
                iAux -= 11001;
                if (opL[i] > 11000 && opL[i] < 13000) {
                    ivalArr[iAux] = ivalArr[opL[i]-11001];
                }
                else if (opL[i] > 15000 && opL[i] < 17000) {
                    ivalArr[iAux] = iConstants[opL[i]-15001];
                }
                else if (opL[i] > 30000 && opL[i] < 31000) {
                    ivalArr[iAux] = iTemp[opL[i]-30001];
                }
            }
            else if (qRes[i] > 21000 && qRes[i] < 22000) {
                int fAux = iTemp[qRes[i]-21001];
                fAux -= 13001;
                if (opL[i] > 13000 && opL[i] < 15000) {
                    fvalArr[fAux] = fvalArr[opL[i]-13001];
                }
                else if (opL[i] > 17000 && opL[i] < 19000) {
                    fvalArr[fAux] = ivalArr[opL[i]-17001];
                }
                else if (opL[i] > 31000 && opL[i] < 32000) {
                    fvalArr[fAux] = fvalArr[opL[i]-31001];
                }
            }
            else {
                if (opL[i] > 11000 && opL[i] < 13000) {
                    ivalArr[qRes[i]-11001] = ivalArr[opL[i]-11001];
                }
                else if (opL[i] > 13000 && opL[i] < 15000) {
                    fvalArr[qRes[i]-13001] = fvalArr[opL[i]-13001];
                }
                else if (opL[i] > 15000 && opL[i] < 17000) {
                    ivalArr[qRes[i]-11001] = iConstants[opL[i]-15001];
                }
                else if (opL[i] > 17000 && opL[i] < 19000) {
                    fvalArr[qRes[i]-13001] = fConstants[opL[i]-17001];
                }
                else if (opL[i] > 30000 && opL[i] < 31000) {
                    ivalArr[qRes[i]-11001] = iTemp[opL[i]-30001];
                }
                else if (opL[i] > 31000 && opL[i] < 32000) {
                    fvalArr[qRes[i]-13001] = fTemp[opL[i]-31001];
                }
                else if (opL[i] > 20000 && opL[i] < 21000) {
                    int iAux = iTemp[opL[i]-20001];
                    iAux -= 11001;
                    ivalArr[qRes[i]-11001] = ivalArr[iAux];
                }
                else if (opL[i] > 21000 && opL[i] < 22000) {
                    int fAux = fTemp[opL[i]-21001];
                    fAux -= 13001;
                    fvalArr[qRes[i]-13001] = fvalArr[fAux];
                }
            }
            break;
        case 14:
            if (qRes[i] > 20000 && qRes[i] < 21000) {
                int aux = iTemp[qRes[i]-20001] - 11001;
                printf("READ INT:\t");
                scanf("%d", &ivalArr[aux]);
            }
            else if (qRes[i] > 21000 && qRes[i] < 22000) {
                int aux = iTemp[qRes[i]-21001] - 13001;
                printf("READ FLOAT:\t");
                scanf("%f", &fvalArr[aux]);
            }
            else if (qRes[i] > 11000 && qRes[i] < 13000) {
                printf("READ INT:\t");
                scanf("%d", &ivalArr[qRes[i]-11001]);
            }
            else if (qRes[i] > 13000 && qRes[i] < 15000) {
                printf("READ FLOAT:\t");
                scanf("%f", &fvalArr[qRes[i]-13001]);
            }
            break;
        case 15:
            if (qRes[i] > 20000 && qRes[i] < 21000) {
                int aux = iTemp[qRes[i]-20001] - 11001;
                printf("WRITE INT\t%d\n", ivalArr[aux]);
            }
            else if (qRes[i] > 21000 && qRes[i] < 22000) {
                int aux = iTemp[qRes[i]-21001] - 13001;
                printf("WRITE FLOAT\t%d\n", fvalArr[aux]);
            }
            else if (qRes[i] > 11000 && qRes[i] < 13000) {
                printf("WRITE INT\t%d\n", ivalArr[qRes[i]-11001]);
            }
            else if (qRes[i] > 13000 && qRes[i] < 15000) {
                printf("WRITE FLOAT\t%.2f\n", fvalArr[qRes[i]-13001]);
            }
            break;
        case 16:
            if (bTemp[opL[i]-32001] == 0) {
                i = qRes[i]-2;
            }
            break;
        case 17:
            i = qRes[i]-2;
            break;
        }
    }
}
