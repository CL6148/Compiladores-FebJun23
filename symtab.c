#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct variable {
    char *name;
    int data_type;
    int dimSize;
    int dim[2];
    // int dirVir;
};

struct variable vars[500];

int counter = 0;
int dimCounter = 0;
int currType = -1;

void setType(int type) {
    currType = type;
}

int searchVar(char *name) {
    int compare;
    for (int i = 0; i < counter; i++) {
        compare = strcmp(name, vars[i].name);
        if (compare == 0) {
            // return vars[i].dirVir;
            return i;
        }
    }
    return -1;
}

void addVar(char *name, int dimSize, int dim1, int dim2) {
    int exists = searchVar(name);
    if (exists == -1) {
        vars[counter].name = name;
        vars[counter].data_type = currType;
        vars[counter].dimSize = dimSize;
        vars[counter].dim[0] = dim1;
        vars[counter].dim[1] = dim2;
        counter++;
    }
    else {
        printf("Variable already declared\n");
        exit(1);
    }
}

void clearSymtab() {
    for (int i = 0; i < counter; i++) {
        if (vars[i].dimSize > 0) {
            for (int j = 0; j < vars[i].dimSize; j++) {
                vars[i].dim[j] = -1;
            }
        }
        vars[i].dimSize = -1;
        vars[i].data_type = -1;
        vars[i].name = '\0';
    }
    counter = 0;
}

void printSymtab() {
    printf("\n************************************\n");
    printf("ID\tType\tSize\tDim1\tDim2\n");
    for (int i = 0; i < counter; i++) {
        printf("%s\t%d\t%d", vars[i].name, vars[i].data_type, vars[i].dimSize);
        if (vars[i].dimSize > 0) {
            for (int j = 0; j < vars[i].dimSize; j++) {
                printf("\t%d", vars[i].dim[j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}
