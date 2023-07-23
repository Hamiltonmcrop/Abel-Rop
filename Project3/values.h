// CMSC 430
// Duane J. Jarc

// This file contains function definitions for the evaluation functions

typedef char* CharPtr;
enum Operators {LESS, ADD, MULTIPLY, LESS_EQUAL, 
                GREATER, GREATER_EQUAL, EQUAL, NOT_EQUAL,
                DIVIDE, REM, EXPO, SUB};

double evaluateReduction(Operators operator_, double head, double tail);
double evaluateRelational(double left, Operators operator_, double right);
double evaluateArithmetic(double left, Operators operator_, double right);

