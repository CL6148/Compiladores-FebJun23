%option noyywrap

%{
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
%}

digit	[0-9]
id	[a-z][a-z0-9]*

ctei	{digit}+
ctef	{digit}*"."{digit}+

%%

"int"	{return INT; }
"float"	{return FLOAT; }
"void"	{return VOID; }

"if"	{return IF; }
"else"	{return ELSE; }
"skip"	{return SKIP; }
"do"	{return DO; }
"while"	{return WHILE; }
"read"	{return READ; }
"write"	{return WRITE; }

"+"	{return ADDOP; }
"-"	{return SUBOP; }
"*"	{return MULOP; }
"/"	{return DIVOP; }
"&&"	{return ANDOP; }
"||"	{return OROP; }
"!="	{return NOTOP; }
"=="	{return EQUOP; }
"<"	{return LESSTH; }
"<="	{return LESSTH_E; }
">"	{return GREATH; }
">="	{return GREATH_E; }

"("	{return LPAREN; }
")"	{return RPAREN; }
"["	{return LBRACK; }
"]"	{return RBRACK; }
"{"	{return LBRACE; }
"}"	{return RBRACE; }
":"	{return COLON; }
";"	{return SEMI; }
"."	{return DOT; }
","	{return COMMA; }
"="	{return ASSIGN; }

"var"	{return VAR; }
"program"	{return PROG; }
"main"	{return MAIN; }
"func"	{return FUNC; }
"return"	{return RET; }

"class"	{return CLASS; }
"public"	{return PUB; }
"private"	{return PRIV; }

{id}	{
		yylval.id = (char*) strdup(yytext);
		return ID; }
{ctei}	{
		yylval.ival = atoi(yytext);
		return CTEI; }
{ctef}	{ 
		yylval.ival = atof(yytext);
		return CTEF; }


[ \n\t\r\f]+	/* empty */

%%