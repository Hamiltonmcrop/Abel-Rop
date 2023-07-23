/**
* Title: Listing.cc - Project 2
* Class: CMSC430
* Name: Cameron Hayes
* Date: 14NOV2022
* Description: Error listing for flex scanner
*/

#include <cstdio>
#include <string>

using namespace std;

#include "listing.h"

/* Global Variables */
static int lineNumber;
static string error = "";
static string allErrors = "";
static int totalErrors = 0;
static int lexicalErrors = 0;
static int syntaxErrors = 0;
static int semanticErrors = 0;

static void displayErrors();

/* Displays line numbers for first line */
void firstLine() {
	lineNumber = 1;
	printf("\n%4d  ",lineNumber);
}	// End of firstLine()

/* Displays line numbers for each subsequent line, 
prints errors if any exist for that previous line */
void nextLine() {
	displayErrors();
	lineNumber++;
	printf("%4d  ",lineNumber);
}	// End of nextLine()

/* Displays errors for last line and prints total 
errors for the entirety of compilation */
void lastLine(double result) {
	printf("\r");
	displayErrors();
	printf("     \n");

	if (totalErrors != 0) {
		printf("Total Errors: %i\n", totalErrors);			// Print total number of errors
		printf("Lexical Errors: %i\n", lexicalErrors);		// Print total number of lexical errors
		printf("Syntax Errors: %i\n", syntaxErrors);		// Print total number of syntax errors
		printf("Semantic Errors: %i\n", semanticErrors);	// Print total number of semantic errors
	}
	else {
		printf("Compiled Successfully\n");
		if ((result == 1) || (result == 0)) {
			printf("Result = %.0lf\n", result);
		}
		else {
			printf("Result = %.2lf\n", result);
		}
	}
}	// End of lastLine();

/* Builds error message strings for output */
void appendError(ErrorCategories errorCategory, string message) {
	string messages[] = { "Lexical Error, Invalid Character ", "",
		"Semantic Error, ", "Semantic Error, Duplicate Identifier: ",
		"Semantic Error, Undeclared " };

	error = messages[errorCategory] + message;
	allErrors += error + "\n";
	totalErrors++;

	switch (errorCategory) {
		case LEXICAL:
			lexicalErrors++;
			break;
		case SYNTAX:
			syntaxErrors++;
			break;
		default:
			semanticErrors++;
			break; 
	}
}	// End of appendError()

/* Displays errors, if any */
void displayErrors() {
	if (allErrors != "") {
		printf("%s", allErrors.c_str());
	}
	allErrors = "";
}	// End of displayErrors()
