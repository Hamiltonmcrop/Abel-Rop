// CMSC 430
// Duane J. Jarc

// This file contains the bodies of the evaluation functions

#include <string>
#include <vector>
#include <cmath>
#include <math.h>
#include <iostream>

using namespace std;

#include "values.h"
#include "listing.h"

// Evaluate reduction operations
double evaluateReduction(Operators operator_, double head, double tail) {
	if (operator_ == ADD)
		return head + tail;
	return head * tail;
}	// end of evaluateRelational()

// Evaluate relational operations
double evaluateRelational(double left, Operators operator_, double right) {
	// Variables
	double result;

	switch (operator_) {
		case LESS:
			result = left < right;
			break;
		case LESS_EQUAL:
			result = left <= right;
			break;
		case GREATER:
			result = left > right;
			break;
		case GREATER_EQUAL:
			result = left >= right;
			break;
		case EQUAL:
			result = left == right;
			break;
		case NOT_EQUAL:
			result = left /= right;
			break;
	}
	return result;
}	// End of evaluateRelational()

// Evaluate arithmetic operations
double evaluateArithmetic(double left, Operators operator_, double right) {
	// Variables
	double result;

	switch (operator_) {
		case ADD:
			result = left + right;
			break;
		case SUB:
			result = left - right;
			break;
		case MULTIPLY:
			result = left * right;
			break;
		case DIVIDE:
			if (right != 0) {
				result = left / right;
			}
			break;
		case REM:
			result = fmod(left, right);
			break;
		case EXPO:
			result = pow(left * 1.0, right * 1.0);
			break;
	}
	return result;
}	// End of evaluateArithmetic()

