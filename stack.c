#include <stdio.h>
#include <stdlib.h>

struct StackNode {
    int iData;
    float fData;
    char *cData;
    struct StackNode* next;
};

struct StackNode* newNodeI(int data) {
    struct StackNode* node = (struct StackNode*)malloc(sizeof(struct StackNode));
    node->iData = data;
    node->fData = -0.0;
    node->cData = '\0';
    node->next = NULL;
    return node;
}

struct StackNode* newNodeF(float data) {
    struct StackNode* node = (struct StackNode*)malloc(sizeof(struct StackNode));
    node->iData = INT_MIN;
    node->fData = data;
    node->cData = '\0';
    node->next = NULL;
    return node;
}

struct StackNode* newNodeC(char *data) {
    struct StackNode* node = (struct StackNode*)malloc(sizeof(struct StackNode));
    node->iData = INT_MIN;
    node->fData = -0.0;
    node->cData = data;
    node->next = NULL;
    return node;
}

int isEmpty(struct StackNode* root) {
    return !root;
}

void pop(struct StackNode** root) {
    if (isEmpty(*root)) {
        exit(1);
    }
    struct StackNode* temp = *root;
    *root = (*root)->next;
    free(temp);
}

void iPush(struct StackNode** root, int data) {
    struct StackNode* node = newNodeI(data);
    node->next = *root;
    *root = node;
}

void fPush(struct StackNode** root, float data) {
    struct StackNode* node = newNodeF(data);
    node->next = *root;
    *root = node;
}

void cPush(struct StackNode** root, char *data) {
    struct StackNode* node = newNodeC(data);
    node->next = *root;
    *root = node;
}

int iPeek(struct StackNode* root) {
    if (isEmpty(root)) {
        exit(1);
    }
    return root->iData;
}

float fPeek(struct StackNode* root) {
    if (isEmpty(root)) {
        exit(1);
    }
    return root->fData;
}

char *cPeek(struct StackNode* root) {
    if (isEmpty(root)) {
        exit(1);
    }
    return root->cData;
}

void printStack(struct StackNode* root) {
    int iTemp;
    float fTemp;
    char *cTemp;

    while (!isEmpty(root)) {
        iTemp = INT_MIN;
        fTemp = -0.0;
        cTemp = NULL;

        if (iPeek(root) != INT_MIN) {
            iTemp = iPeek(root);
            printf("%12d", iTemp);
        }
        else if (fPeek(root) != -0.0) {
            fTemp = fPeek(root);
            printf("%12.2f", fTemp);
        }
        else {
            cTemp = cPeek(root);
            printf("%12s", cTemp);
        }
        pop(&root);
    }
    printf("\n");
}
