%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semantics.c>

extern FILE *yyin;
extern FILE *yyout;
extern int yylex();
extern int yylineno;

void yyerror();

%}

%union semrec{
	int ival;
	float fval;
	char* id;
}

%start	program
%token <ival>	CTEI
%token <fval>	CTEF
%token <id>		ID

%type<ival> varctei
%type<fval> varctef
%type<id> varcteid

%token	INT FLOAT VOID
%token	DO SKIP READ WRITE IF WHILE ELSE
%token	NOTOP ANDOP OROP EQUOP LESSTH GREATH LESSTH_E GREATH_E
%token	LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE COLON SEMI DOT COMMA ASSIGN
%token	VAR PROG MAIN FUNC RET
%token	CLASS PUB PRIV

%left	ADDOP SUBOP
%left	MULOP DIVOP

%right	UMINUS

%%

program: PROG ID SEMI { printf(" >----- Successful read of header\n"); }
	variables { printf(" >----- Successful read of variables block\n"); }
	functions MAIN { printf(" >----- Successful entry into MAIN block\n"); }
	block { printf(" >----- Successful exit of MAIN block\n"); };

variables: VAR vars;
vars: vars type vars1 vars2 SEMI | /* empty */;
vars1: vars1 vars2 COMMA | /* empty */;
vars2: ID
	| ID LBRACK CTEI RBRACK
	| ID LBRACK CTEI RBRACK LBRACK CTEI RBRACK;

functions: FUNC func;
func: func func1 | /* empty */;
func1: funcHead funcTail;
funcHead: type ID LPAREN parameter RPAREN;
funcTail: LBRACE variables funcStmt RBRACE;
funcStmt: funcStmt statement | /* empty */;

parameter: par | /* empty */;
par: par1 | par COMMA par1;
par1: type ID;

type: INT
	| FLOAT
	| VOID;

block: LBRACE block1 RBRACE;
block1: block1 statement | /* empty */;

statement: assignment
	| condition
	| cycle
	| readStmt
	| writeStmt
	| SKIP SEMI;

assignment: ID ASSIGN expression SEMI
	| ID LBRACK CTEI RBRACK ASSIGN expression SEMI 
	| ID LBRACK CTEI RBRACK LBRACK CTEI RBRACK ASSIGN expression SEMI;

condition:IF LPAREN expression RPAREN block ELSE block SEMI;

cycle: WHILE LPAREN expression RPAREN DO block SEMI;

readStmt: READ read1 SEMI;
read1: ID
	| ID LBRACK CTEI RBRACK
	| ID LBRACK CTEI RBRACK LBRACK CTEI RBRACK;

writeStmt: WRITE expression SEMI;

expression: rel expr1;
expr1: ANDOP rel
	| OROP rel
	| /* empty */;

rel: exp rel1;
rel1: GREATH exp
	| GREATH_E exp
	| LESSTH exp
	| LESSTH_E exp
	| EQUOP exp
	| NOTOP exp
	| /* empty */;

exp: term exp1;
exp1: ADDOP term
	| SUBOP term
	| /* empty */;

term: factor term1;
term1: MULOP factor
	| DIVOP factor
	| /* empty */;

factor: LPAREN exp RPAREN
	| varctei
	| varctef
	| varcteid;

varctei: CTEI
	| SUBOP CTEI %prec UMINUS { $$ = - $2; };

varctef: CTEF
	| SUBOP CTEF %prec UMINUS { $$ = - $2; };

varcteid: ID
	| ID LBRACK CTEI RBRACK
	| ID LBRACK CTEI RBRACK LBRACK CTEI RBRACK;

%%

void yyerror(){
	fprintf(stderr, "Syntax error near line %d\n", yylineno);
	exit(1);
}

int main (int argc, char *argv[]){
	int flag;
	yyin = fopen(argv[1], "r");

	printf("DEBUG:\tBeginning of PARSE\n");

	flag = yyparse();
	fclose(yyin);
	
	printf("DEBUG:\tSuccessful parse of file\n");
	printf("END: Compiled without error\n");

	printf("\n");

	return flag;
}