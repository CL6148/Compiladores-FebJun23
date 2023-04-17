%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "virtualMachine.h"

extern FILE *yyin;
extern FILE *yyout;
extern int yylex();

void yyerror();

int dataType = -1;

struct label {
	int for_goto;
	int for_jmp_false;
};
struct label *newlabel() {
	return (struct label *) malloc(sizeof(struct label));
};
void install(char *name) {
	list_t *l = getsym(name);
	if (l == 0) {
		putsym(name, dataType, 1, 1);
	}
	else {
		fprintf(stderr, "ERR: %s is already defined\n", name);
		exit(1);
	}

	list_t *temp = getsym(name);
	printf(" %2d:%d:%d\n", temp->offset, temp->st_id, temp->st_type);
}
void install_arr(char *name, int dim1) {
	list_t *l = getsym(name);
	if (l == 0) {
		if (dim1 < 1) {
			fprintf(stderr, "ERR: Must initialize array with 1 or more slots\n");
			exit(1);
		}
		else {
			putsym(name, 2, 1, dim1);
			incr_offset(dim1-1);
		}
	}
	else {
		fprintf(stderr, "ERR: %s is already defined\n", name);
		exit(1);
	}
	
	list_t *temp = getsym(name);
	printf(" %2d:%d:%d\n", temp->offset, temp->st_id, temp->st_type);
}
void install_mat(char *name, int dim1, int dim2) {
	list_t *l = getsym(name);
	if (l == 0) {
		if (dim1 < 1 || dim2 < 1) {
			fprintf(stderr, "ERR: Must initialize matrix with 1 or more slots for each dimension\n");
		}
		else {
			putsym(name, 3, dim1, dim2);
			incr_offset((dim1*dim2)-1);
		}
	}
	else {
		fprintf(stderr, "ERR, %s is already defined\n", name);
		exit(1);
	}

	list_t *temp = getsym(name);
	printf(" %2d:%d:%d\n", temp->offset, temp->st_id, temp->st_type);
}

void context_check(enum code_ops operation, char *name) {
	list_t *l = getsym(name);
	if (l == 0) {
		fprintf(stderr, "ERR: %s is not defined\n", name);
	}
	else {
		gen_code(operation, l->offset);
	}
}
void context_arr(enum code_ops operation, char *name, int dim1) {
	list_t *l = getsym(name);
	if (l == 0) {
		fprintf(stderr, "ERR: %s not found\n", name);
		exit(1);
	}
	else {
		if (dim1 < l->dim2) {
			int temp = l->offset + dim1;
			gen_code(operation, temp);
		}
		else {
			fprintf(stderr, "ERR: Out of Bounds\n");
			exit(1);
		}
	}
}
void context_mat(enum code_ops operation, char *name, int dim1, int dim2) {
	list_t *l = getsym(name);
	if (l == 0) {
		fprintf(stderr, "ERR: %s not found\n", name);
	}
	else {
		if (dim1 < l->dim1 && dim2 < l->dim2) {
			int aux = l->offset + (l->dim2 * dim1) + dim2;
			gen_code(operation, aux);
		}
		else {
			fprintf(stderr, "ERR: Out of Bounds\n");
			exit(1);
		}
	}
}

%}

%union semrec{
	int ival;
	float fval;
	char *id;
	struct label *lbl;
}

%start	program
%token <ival>	CTEI
%token <fval>	CTEF
%token <id>		ID
%token <lbl>	IF WHILE ELSE

%token	INT FLOAT VOID
%token	DO SKIP READ WRITE
%token	NOTOP ANDOP OROP EQUOP LESSTH GREATH LESSTH_E GREATH_E
%token	LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE COLON SEMI DOT COMMA ASSIGN
%token	VAR PROG MAIN FUNC RET
%token	CLASS PUB PRIV

%left	ADDOP SUBOP
%left	MULOP DIVOP

%%

program: PROG ID SEMI { printf(" >----- Successful read of header\n"); }
	variables { printf(" >----- Successful read of variables block\n"); }
	functions MAIN { printf(" >----- Successful entry into MAIN block\n"); }
	block { printf(" >----- Successful exit of MAIN block\n"); };

variables: VAR vars;
vars: vars type vars1 vars2 SEMI | /* empty */;
vars1: vars1 vars2 COMMA | /* empty */;
vars2: ID { install($1); }
	| ID LBRACK CTEI RBRACK { install_arr($1, $3); }
	| ID LBRACK CTEI RBRACK LBRACK CTEI RBRACK { install_mat($1, $3, $6); };

functions: FUNC func;
func: func func1 | /* empty */;
func1: funcHead funcTail;
funcHead: type ID LPAREN parameter RPAREN;
funcTail: LBRACE variables funcStmt RBRACE;
funcStmt: funcStmt statement | /* empty */;

parameter: par | /* empty */;
par: par1 | par COMMA par1;
par1: type ID;

type: INT { dataType = 0; }
	| FLOAT { dataType = 1; }
	| VOID;

block: LBRACE block1 RBRACE;
block1: block1 statement | /* empty */;

statement: assignment
	| condition
	| cycle
	| readStmt
	| writeStmt
	| callFunc
	| SKIP SEMI;

assignment: ID ASSIGN expression SEMI { context_check(STORE, $1); }
	| ID LBRACK CTEI RBRACK ASSIGN expression SEMI { context_arr(STORE, $1, $3); }
	| ID LBRACK CTEI RBRACK LBRACK CTEI RBRACK ASSIGN expression SEMI { context_mat(STORE, $1, $3, $6); };

condition:IF LPAREN expression RPAREN { $1 = (struct label *) newlabel();
										$1->for_jmp_false = reserve_loc(); }
	block { $1->for_goto = reserve_loc(); }
	ELSE { back_patch($1->for_jmp_false, JMP_FALSE, gen_label()); }
	block SEMI { back_patch($1->for_goto, GOTO, gen_label()); };

cycle: WHILE { $1 = (struct label *) newlabel();
			   $1->for_goto = gen_label(); }
	LPAREN expression RPAREN { $1->for_jmp_false = reserve_loc(); }
	DO block SEMI { gen_code(GOTO, $1->for_goto);
					back_patch($1->for_jmp_false, JMP_FALSE, gen_label()); };

readStmt: READ read1 SEMI;
read1: ID { context_check(READ_INT, $1); }
	| ID LBRACK read2 RBRACK { context_arr(READ_INT, $1, $3); }
	| ID LBRACK CTEI RBRACK LBRACK CTEI RBRACK { context_mat(READ_INT, $1, $3, $6); };

writeStmt: WRITE expression { gen_code(WRITE_INT, 0); } SEMI;

callFunc: ID LPAREN callParam RPAREN;
callParam: callPm1 | /* empty */;
callPm1: callPm1 COMMA varcte | varcte;

expression: rel expr1;
expr1: ANDOP rel { gen_code(AND, 0); }
	| OROP rel { gen_code(OR, 0); }
	| /* empty */;

rel: exp rel1;
rel1: GREATH exp { gen_code(GT, 0); }
	| GREATH_E exp { gen_code(GTE, 0); }
	| LESSTH exp { gen_code(LT, 0); }
	| LESSTH_E exp { gen_code(LTE, 0); }
	| EQUOP exp { gen_code(EQ, 0); }
	| NOTOP exp { gen_code(NOT, 0); }
	| /* empty */;

exp: term exp1;
exp1: ADDOP term { gen_code(ADD, 0); }
	| SUBOP term { gen_code(SUB, 0); }
	| /* empty */;

term: factor term1;
term1: MULOP factor { gen_code(MULT, 0); }
	| DIVOP factor { gen_code(DIV, 0); }
	| /* empty */;

factor: LPAREN exp RPAREN
	| varcte;

varcte: CTEI { gen_code(LD_INT, $1); }
	| SUBOP CTEI { gen_code(LD_INT, $2 * -1); }
	| CTEF { gen_code(LD_INT, $1); }
	| SUBOP CTEF { gen_code(LD_INT, $2 * -1); }
	| ID { context_check(LD_VAR, $1); }
	| ID LBRACK CTEI RBRACK { context_arr(LD_VAR, $1, $3); }
	| ID LBRACK CTEI RBRACK LBRACK CTEI RBRACK { context_mat(LD_VAR, $1, $3, $6); };

%%

void yyerror(){
	fprintf(stderr, "Syntax error\n");
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

	print_code();
	fetch_execute_cycle();

	return flag;
}