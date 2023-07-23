/**
* Title: Listing.h - Project 2
* Class: CMSC430
* Name: Cameron Hayes
* Date: 14NOV2022
* Description: Header file used for listing.cc
*/

enum ErrorCategories {LEXICAL, SYNTAX, GENERAL_SEMANTIC, DUPLICATE_IDENTIFIER,
	UNDECLARED};

void firstLine();
void nextLine();
void lastLine(double result);
void appendError(ErrorCategories errorCategory, string message);

