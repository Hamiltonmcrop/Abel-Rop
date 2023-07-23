/* Title: Parser.y - Project 2					*/
/* Class: CMSC430								*/
/* Name: Cameron Hayes							*/
/* Date: 13NOV2022 								*/
/* Description: Bison input file for parser 	*/
%{

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <math.h>

using namespace std;

#include "values.h"
#include "listing.h"
#include "symbols.h"

int yylex();
void yyerror(const char* message);

Symbols<double> symbols;

double *parameters;
int paramNum = 0;
double paramCount;
double result;

%}

%define parse.error verbose

%union
{
	CharPtr iden;
	Operators oper;
	double value;
}

%token <iden> IDENTIFIER
%token <value> INT_LITERAL REAL_LITERAL BOOL_LITERAL

%token <oper> ADDOP MULOP REMOP EXPOP RELOP
%token ANDOP OROP NOTOP 

%token BEGIN_ BOOLEAN END ENDREDUCE FUNCTION INTEGER IS REDUCE RETURNS REAL

%type <value> body statement_ statement reductions expression relation term
	factor primary and exponent notion case case_
%type <oper> operator

%token IF THEN ELSE ENDIF CASE OTHERS ARROW ENDCASE WHEN 

%%

function:	
	function_header variable_ body {result = $3;} ;
	
function_header:	
	FUNCTION IDENTIFIER parameter_ RETURNS type ';' |
	error ';' ;

variable_:
	variable variable_ | ;

variable:
	IDENTIFIER ':' type IS statement_ {symbols.insert($1, $5);} |
	error ';' ;

parameter_:
	parameter ',' parameter_ |
	parameter ;

parameter:
	IDENTIFIER ':' type{symbols.insert($1, parameters[paramNum++]);} | ;

type:
	INTEGER |
	REAL |
	BOOLEAN ;

body:
	BEGIN_ statement_ END ';' {$$ = $2;} ;
    
statement_:
	statement ';' |
	error ';' {$$ = 0;} ;
	
statement:
	expression |
	REDUCE operator reductions ENDREDUCE {$$ = $3;} | 
	IF expression THEN statement_ ELSE statement_ ENDIF {$$ = ($2) ? $4 : $6 ;} | 
	CASE expression IS case_ OTHERS ARROW statement_ ENDCASE {$$ = isnan($4) ? $7 : $4;} ;

operator:
	ADDOP |
	REMOP |
	EXPOP |
	MULOP ;

reductions:
	reductions statement_ {$$ = evaluateReduction($<oper>0, $1, $2);} | 
	{$$ = $<oper>0 == ADD ? 0 : 1;} ;

case_:
	case_ case {$$ = isnan($1) ? $2 : $1;}; | 
	{$$=NAN;} ;

case: 
	WHEN INT_LITERAL ARROW statement_ {$$ = $<value>-2 == $2 ? $4 : NAN;} |
	error ';' {$$ = 0;} ;
		    
expression:
	expression OROP and {$$ = $1 || $3;} |
	and ;

and:
	and ANDOP relation {$$ = $1 && $3; } | 
	relation ;

relation:
	relation RELOP term {$$ = evaluateRelational($1, $2, $3);} |
	term ;

term:
	term ADDOP factor {$$ = evaluateArithmetic($1, $2, $3);}  |
	factor ;
      
factor:
	factor MULOP exponent {$$ = evaluateArithmetic($1, $2, $3);} |
	factor REMOP exponent {$$ = evaluateArithmetic($1, $2, $3);} |
	exponent ;

exponent:
	notion EXPOP exponent {$$ = evaluateArithmetic($1, $2, $3);} |
	notion;

notion:
	NOTOP primary {$$ = !$2;} |
	primary;

primary:
	'(' expression ')' {$$ = $2;} |
	INT_LITERAL | 
	REAL_LITERAL |
	BOOL_LITERAL |
	IDENTIFIER {if (!symbols.find($1, $$)) appendError(UNDECLARED, $1);}  ;
    
%%

void yyerror(const char* message) {
	appendError(SYNTAX, message);
}

int main(int argc, char *argv[]) {
	parameters = new double[argc - 1];
	paramCount = argc - 1;
	
	for (int i = 1; i < argc; i++) {
		parameters[i - 1] = atof(argv[i]);
	}

	firstLine();
	yyparse();
	lastLine(result);
	return 0;
}