%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.c"
#include "symtab.c"
#include "intermediate.c"

extern FILE *yyin;
extern FILE *yyout;
extern int yylex();
extern int yylineno;

void yyerror();

%}

%union {
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
%token	LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE SEMI COMMA ASSIGN
%token	VAR PROG MAIN FUNC
%token	COLON RET DOT
%token	CLASS PUB PRIV

%left	ADDOP SUBOP
%left	MULOP DIVOP

%right	UMINUS

%%

program: PROG ID SEMI	{ printf("\t>----- Successful read of header\n"); }
	variables			{ printf("\t>----- Successful read of variables block\n"); }
	functions MAIN		{ printf("\t>----- Successful read of functions block\n"); }
	block				{ printf("\t>----- Successful read of MAIN block\n"); };

variables: VAR vars;
vars: vars type vars1 vars2 SEMI
	| /* empty */;
vars1: vars1 vars2 COMMA | /* empty */;
vars2: ID										{ addVar($1, 0, -1, -1); }
	| ID LBRACK CTEI RBRACK						{ $3 < 1 ? yyerror(1) : addVar($1, 1, $3, -1); }
	| ID LBRACK CTEI RBRACK LBRACK CTEI RBRACK	{ ($3 < 1) || ($6 < 1) ? yyerror(1) : addVar($1, 2, $3, $6); };

functions: FUNC func;
func: func func1 | /* empty */;
func1: funcHead funcTail;
funcHead: type ID LPAREN parameter RPAREN;
funcTail: LBRACE variables funcStmt RBRACE;
funcStmt: funcStmt statement | /* empty */;

parameter: par | /* empty */;
par: par1 | par COMMA par1;
par1: type ID;

type: INT		{ setType(1); }
	| FLOAT		{ setType(2); }
	| VOID		{ setType(-2); };

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
expr1: ANDOP rel	{ iPush(&operator, 5); }
	| OROP rel		{ iPush(&operator, 6); }
	| /* empty */;

rel: exp rel1;
rel1: GREATH exp	{ iPush(&operator, 10); }
	| GREATH_E exp	{ iPush(&operator, 12); }
	| LESSTH exp	{ iPush(&operator, 9); }
	| LESSTH_E exp	{ iPush(&operator, 11); }
	| EQUOP exp		{ iPush(&operator, 8); }
	| NOTOP exp		{ iPush(&operator, 7); }
	| /* empty */;

// ---------------------------------------------------------------------

exp: term exp1;
exp1: ADDOP term	{ iPush(&operator, 1); }	// { genQuad sum }
	| SUBOP term	{ iPush(&operator, 2); }	// { genQuad sub }
	| /* empty */;

term: factor term1;
term1: MULOP factor	{ iPush(&operator, 3); }	// { genQuad mul }
	| DIVOP factor	{ iPush(&operator, 4); }	// { genQuad div }
	| /* empty */;

factor: LPAREN exp RPAREN
	| varctei		{ iPush(&operand, $1); }
	| varctef		{ fPush(&operand, $1); }
	| varcteid;

varctei: CTEI
	| SUBOP CTEI %prec UMINUS { $$ = - $2; };

varctef: CTEF
	| SUBOP CTEF %prec UMINUS { $$ = - $2; };

varcteid: ID									{ int exists = searchVar($1);
												  (exists != -1) && (vars[exists].dimSize == 0) ? cPush(&operand, $1) : yyerror(2); }
	| ID LBRACK CTEI RBRACK						{ int exists = searchVar($1);
												  if ($3 < 0 || $3 >= vars[exists].dim[0]) {
													yyerror(3);
												  }
												  (exists != -1) && (vars[exists].dimSize == 1) ? cPush(&operand, $1) : yyerror(2); }
	| ID LBRACK CTEI RBRACK LBRACK CTEI RBRACK	{ int exists = searchVar($1);
												  if ($3 < 0 || $3 >= vars[exists].dim[0] || $6 < 0 || $6 >= vars[exists].dim[1]) {
													yyerror(3);
												  }
												  (exists != -1) && (vars[exists].dimSize == 2) ? cPush(&operand, $1) : yyerror(2); };

%%

void yyerror(int errCode){
	switch(errCode) {
	case 1:
		fprintf(stderr, "Array declaration near line %d must be at least '1'\n", yylineno);
		break;
	case 2:
		fprintf(stderr, "ID indexed near line %d is incorrect\n", yylineno);
		break;
	case 3:
		fprintf(stderr, "Array out of range near line %d\n", yylineno);
		break;
	default:
		fprintf(stderr, "Syntax error near line %d\n", yylineno);
		break;
	}
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


	// printSymtab();
	// printStack(operator);
	// printf("\n");
	// printStack(operand);


	printf("\n");

	return flag;
}