flex scanner.l
bison -d parser.y
gcc -o main parser.tab.c lex.yy.c