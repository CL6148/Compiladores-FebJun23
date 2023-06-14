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
%type<ival> index

%token	INT FLOAT VOID
%token	DO SKIP READ WRITE IF WHILE ELSE
%token	NOTOP ANDOP OROP EQUOP LESSTH GREATH LESSTH_E GREATH_E
%token	LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE SEMI COMMA ASSIGN
%token	VAR PROG MAIN FUNC
%token	CLASS

%left	ADDOP SUBOP
%left	MULOP DIVOP

%right	UMINUS

%%

program: PROG ID SEMI	{ printf("\t>----- Successful read of header\n"); }
	variables			{ printSymtab();
						  printf("\t>----- Successful read of variables block\n"); }
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
	| readStmt
	| writeStmt
	| condition
	| cycle
	| SKIP SEMI;

// ---------------------------------------------------------------------

assignment: varcteid ASSIGN { iPush(&operator, 13); }
			expression SEMI	{ genQuadLin(1); };

readStmt: READ				{ iPush(&operator, 14); }
		  varcteid SEMI		{ genQuadLin(2); };

writeStmt: WRITE			{ iPush(&operator, 15); }
		   expression SEMI	{ genQuadLin(2); };

// ---------------------------------------------------------------------

condition: IF LPAREN expression RPAREN	{ int expType = iPeek(types);
										  if (expType != 3) {
										      yyerror(4);
										  }
										  else {
											  iPush(&operator, 16);
											  genQuadLin(1);
										  } }
		   block ELSE					{ int fls = iPeek(jumps);
										  pop(&jumps);
										  iPush(&operator, 17);
										  genQuadLin(2);
										  qRes[fls-1] = quadCounter;
										  printf("\nQuad Rewrite (JUMP):\n");
										  printf("%d- %d , %d , %d , %d\n", fls, op[fls-1], opL[fls-1], opR[fls-1], qRes[fls-1]); }
		   block SEMI					{ int end = iPeek(jumps);
		   								  pop(&jumps);
										  qRes[end-1] = quadCounter;
										  printf("\nQuad Rewrite (JUMP):\n");
										  printf("%d- %d , %d , %d , %d\n", end, op[end-1], opL[end-1], opR[end-1], qRes[end-1]); };

cycle: WHILE							{ iPush(&jumps, quadCounter); }
	   LPAREN expression RPAREN			{ int expType = iPeek(types);
	   									  if (expType != 3) {
											  yyerror(4);
										  }
										  else {
											  iPush(&operator, 16);
											  genQuadLin(1);
										  } }
	   DO block SEMI					{ int end = iPeek(jumps);
	   									  pop(&jumps);
										  int resJ = iPeek(jumps);
										  iPush(&operator, 17);
										  genQuadLin(2);
										  qRes[quadCounter-2] = resJ;
										  qRes[end-1] = quadCounter;
										  printf("\nQuad Rewrite (JUMP):\n");
										  printf("%d- %d , %d , %d , %d\n", end, op[end-1], opL[end-1], opR[end-1], qRes[end-1]);

										  printf("\nQuad Rewrite (JUMP):\n");
										  printf("%d- %d , %d , %d , %d\n", quadCounter-1, op[quadCounter-2], opL[quadCounter-2], opR[quadCounter-2], qRes[quadCounter-2]); };

// ---------------------------------------------------------------------

expression: rel expr1;
expr1: expr2 rel	{ genQuad(); }
	| /* empty */;
expr2: ANDOP		{ iPush(&operator, 5); }
	| OROP			{ iPush(&operator, 6); };

rel: exp rel1;
rel1: rel2 exp		{ genQuad(); }
	| /* empty */;
rel2: GREATH		{ iPush(&operator, 10); }
	| GREATH_E		{ iPush(&operator, 12); }
	| LESSTH		{ iPush(&operator, 9); }
	| LESSTH_E		{ iPush(&operator, 11); }
	| EQUOP			{ iPush(&operator, 8); }
	| NOTOP			{ iPush(&operator, 7); };

exp: term exp1;
exp1: exp2 term		{ genQuad(); }
	  exp1
	| /* empty */;
exp2: ADDOP			{ iPush(&operator, 1); }
	| SUBOP			{ iPush(&operator, 2); };

term: factor term1;
term1: term2 factor	{ genQuad(); }
	   term1
	| /* empty */;
term2: MULOP		{ iPush(&operator, 3); }
	| DIVOP			{ iPush(&operator, 4); };

factor: LPAREN exp RPAREN
	| varctei		{ iConstants[iconst++] = $1;
					  iPush(&operand, iconst + 15000);
					  iPush(&types, 1); }
	| varctef		{ fConstants[fconst++] = $1;
					  iPush(&operand, fconst + 17000);
					  iPush(&types, 2); }
	| varcteid;

varctei: CTEI
	| SUBOP CTEI %prec UMINUS { $$ = - $2; };

varctef: CTEF
	| SUBOP CTEF %prec UMINUS { $$ = - $2; };

varcteid: ID										{ int exists = searchVar($1);
													  if ((exists != -1) && (vars[exists].dimSize == 0)) {
														  iPush(&operand, vars[exists].dirVir);
													  	  iPush(&types, vars[exists].data_type);
													  }
													  else {
													  	  yyerror(2);
													  } }
/*
	| ID LBRACK CTEI RBRACK						{ int exists = searchVar($1);
												  if ($3 < 0 || $3 >= vars[exists].dim[0]) {
													yyerror(3);
												  }
												  if ((exists != -1) && (vars[exists].dimSize == 1)) {
												  	  iPush(&operand, (vars[exists].dirVir + $3));
												  	  iPush(&types, vars[exists].data_type);
												  }
												  else {
												  	  yyerror(2);
												  } }
	| ID LBRACK CTEI RBRACK LBRACK CTEI RBRACK	{ int exists = searchVar($1);
												  if ($3 < 0 || $3 >= vars[exists].dim[0] || $6 < 0 || $6 >= vars[exists].dim[1]) {
													yyerror(3);
												  }
												  if ((exists != -1) && (vars[exists].dimSize == 2)) {
												  	  iPush(&operand, (vars[exists].dirVir + (vars[exists].dim[1] * $3 + $6)));
												  	  iPush(&types, vars[exists].data_type);
												  }
												  else {
												  	  yyerror(2);
												  } }
*/
	| ID LBRACK index RBRACK						{ int exists = searchVar($1);
													  // genQuadVER(S3, lSup)		lInf is always 0
													  // genQuad (VER, exp, 0, lSup(A))
													  
													  iPush(&operand, vars[exists].dirVir);
													  iPush(&types, vars[exists].data_type);
													  iPush(&operator, 1);
													  genQuad();
													  
													  int aux = iPeek(operand);
													  pop(&operand);
													  iPush(&operand, aux-10000);
													  qRes[quadCounter-2] -= 10000; }
	| ID LBRACK index RBRACK LBRACK index RBRACK	{ int exists = searchVar($1);

													  // printf("INDEX2: %d\n", iPeek(operand));
													  // pop(&operand);
													  // printf("INDEX1: %d\n", iPeek(operand));

													  iPush(&operand, vars[exists].dirVir);
													  iPush(&types, vars[exists].data_type);
													  iPush(&operator, 1);
													  iPush(&operator, 1);
													  genQuad();
													  
													  int aux1 = iPeek(operand);
													  pop(&operand);
													  iPush(&operand, aux1-10000);
													  qRes[quadCounter-2] -= 10000;
													  genQuad();

													  int aux2 = iPeek(operand);
													  pop(&operand);
													  iPush(&operand, aux2-10000);
													  qRes[quadCounter-2] -= 10000; };

index: exp										{ if (iPeek(types) != 1) {
													  yyerror(4);
												  } };


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
		fprintf(stderr, "Error indexing array near line %d\n", yylineno);
		break;
	case 4:
		fprintf(stderr, "Type Mismatch\n");
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

	FILE *f = fopen("instructions.txt", "w");
	fprintf(f, "");
	fclose(f);

	FILE *g = fopen("variables.txt", "w");
	fprintf(g, "");
	fclose(g);

	flag = yyparse();
	fclose(yyin);

	printf("DEBUG:\tSuccessful parse of file\n");
	printf("END: Compiled without error\n\n");

	executeCode(iConstants, fConstants);

	printf("\n");

	return flag;
}