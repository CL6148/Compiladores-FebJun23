#include <stdio.h>
#include <stdlib.h>

/*
1   int
2   float
3   bool

-1 TypeMismatch
-2 Undef Type
-3 Undef Operator
*/

int semantic_cube(int operation, int type1, int type2) {
    if (operation > 12 || operation < 1) {
        return -3;
    }
    if (type1 > 3 || type1 < 1 || type2 > 3 || type2 < 1) {
        return -2;
    }

    switch(operation) {
        // + - * /
        case 1:
        case 2:
        case 3:
        case 4:
            switch(type1) {
                case 1:
                    switch(type2) {
                        case 1:
                            return 1;
                            break;
                        case 2:
                            return 2;
                            break;
                        case 3:
                            return -1;
                            break;
                    }
                    break;
                case 2:
                    switch(type2) {
                        case 1:
                            return 2;
                            break;
                        case 2:
                            return 2;
                            break;
                        case 3:
                            return -1;
                            break;
                    }
                    break;
                case 3:
                    return -1;
                    break;
            }
            break;

        // && ||
        case 5:
        case 6:
            switch(type1) {
                case 1:
                case 2:
                    switch(type2) {
                        case 1:
                        case 2:
                        case 3:
                            return -1;
                            break;
                    }
                    break;
                case 3:
                    switch(type2) {
                        case 1:
                        case 2:
                            return -1;
                            break;
                        case 3:
                            return 3;
                            break;
                    }
                    break;
            }
            break;

        // != ==
        case 7:
        case 8:
            switch(type1) {
                case 1:
                case 2:
                    switch(type2) {
                        case 1:
                        case 2:
                            return 3;
                            break;
                        case 3:
                            return -1;
                            break;
                    }
                    break;
                case 3:
                    switch(type2) {
                        case 1:
                        case 2:
                            return -1;
                            break;
                        case 3:
                            return 3;
                            break;
                    }
                    break;
            }
            break;

        // < > <= >=
        case 9:
        case 10:
        case 11:
        case 12:
            switch(type1) {
                case 1:
                case 2:
                    switch(type2) {
                        case 1:
                        case 2:
                            return 3;
                            break;
                        case 3:
                            return -1;
                            break;
                    }
                case 3:
                    return -1;
                    break;
            }
            break;
    }
}
/*
int main(void) {
    for (int i = 0; i <= 3; i++) {
        for (int j = 0; j <= 3; j++) {
            printf("+\t%d\t%d\t:\t%d\n", i, j, semantic_cube(1,i,j));
            printf("-\t%d\t%d\t:\t%d\n", i, j, semantic_cube(2,i,j));
            printf("*\t%d\t%d\t:\t%d\n", i, j, semantic_cube(3,i,j));
            printf("/\t%d\t%d\t:\t%d\n", i, j, semantic_cube(4,i,j));
            printf("&&\t%d\t%d\t:\t%d\n", i, j, semantic_cube(5,i,j));
            printf("||\t%d\t%d\t:\t%d\n", i, j, semantic_cube(6,i,j));
            printf("!=\t%d\t%d\t:\t%d\n", i, j, semantic_cube(7,i,j));
            printf("==\t%d\t%d\t:\t%d\n", i, j, semantic_cube(8,i,j));
            printf("<\t%d\t%d\t:\t%d\n", i, j, semantic_cube(9,i,j));
            printf(">\t%d\t%d\t:\t%d\n", i, j, semantic_cube(10,i,j));
            printf("<=\t%d\t%d\t:\t%d\n", i, j, semantic_cube(11,i,j));
            printf(">=\t%d\t%d\t:\t%d\n", i, j, semantic_cube(12,i,j));
            printf("Undef\t\t\t:\t%d\n\n", semantic_cube(13,i,j));
        }
        printf("\n**********************************\n");
    }
    
}
*/