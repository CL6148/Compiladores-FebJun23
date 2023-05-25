#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Current design utilizes arrays for information, change into stacks to utilize
    the push() and pop() functions allowing for proper cleaning of these
    temporary structures when moving to the global/local structures before
    calling the virtual memory functions
    
    addrID  ->  address is defined in virtual machine after sending the list
                of global/local variables and corresponding types

    When reading variables, add info to temporary arrays/stacks to hold.
    Once the variables block is done reading, transfer temporary info into
    permanent stack (global and function locals) and delete the temporary
    structure (this structure will be reused in subsequent readings of
    new variable blocks).

    Working on the permanent structure after everything has been added properly
    will call the virtual machine to allocate the memory needed for dimensions
    and give variables the address to memory over ID for quadruples generation.
*/

int counter = 0;
int dimCounter = 0;
int currType = -1;

char *nameID[99][1];
int type[99];
int dimSize[99];
int dims[99];

int verifyVar(char *name) {
    int compare;
    for (int i = 0; i < counter; i++) {
        compare = strcmp(name, nameID[i][0]);
        if (compare == 0) {
            return i;
        }
    }
    return -1;
}

// no variables with the same name allowed even if the types are different
void addVar(char *name, int dSize) {
    int verify = verifyVar(name);
    if (verify == -1) {
        nameID[counter][0] = name;
        type[counter] = currType;
        dimSize[counter] = dSize;
        counter++;
    }
    else {
        printf("Variable with name '%s' is already defined.\n", name);
        exit(1);
    }
}

void addDims(int dimension) {
    dims[dimCounter] = dimension;
    dimCounter++;
}

void setType(int type) {
    currType = type;
}

void clearSymtab() {
    // delete structure contents            ** This does not function with arrays, must implement stacks
    currType = -1;
}

void printSymtab() {
    int aux = 0;

    printf("************************************\n");
    printf("ID\tType\tSize\tDim1\tDim2\n");
    for (int i = 0; i < counter; i++) {
        printf("%s\t%d\t%d", nameID[i][0], type[i], dimSize[i]);
        if (dimSize[i] > 0) {
            for (int j = 0; j < dimSize[i]; j++) {
                printf("\t%d", dims[aux]);
                aux++;
            }
        }
        printf("\n");
    }
}