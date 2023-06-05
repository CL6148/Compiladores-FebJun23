#include <stdio.h>
#include <stdlib.h>

/*
1   int
2   float
3   bool        represented as an integer value [0,1] along the specific data type bool
--------------------------------
-1 TypeMismatch
-2 Undef Type
-3 Undef Operator
--------------------------------
1   +
2   -
3   *
4   /
5   &&
6   ||
7   !=
8   ==
9   <
10  >
11  <=
12  >=

13  =
14  read
15  write

*/

int semantic_cube(int operation, int type1, int type2) {
    if (operation > 15 || operation < 1) {
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
                    return -1;
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
        case 13:
            switch(type1) {
            case 1:
                switch(type2) {
                case 1:
                    return 1;
                    break;
                default:
                    return -1;
                    break;
                }
            case 2:
                switch(type2) {
                case 2:
                    return 2;
                    break;
                default:
                    return -1;
                    break;
                }
            }
    }
}
