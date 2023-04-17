bison -d parser.y
flex scanner.l
gcc -o main parser.tab.c lex.yy.c