"""
File: testExpressions.py
Author: Derrf Seitz

Test expression trees.

The following files must be in the same folder:
  expressiontree.py
  parsers.py
  scanner.py
  tokens.py
"""

from parsers import Parser

testList = [
"7 - 2 * 5",              # -3
"(7 - 2) * 5",            # 25
"9 * 5 + 10",             # 55
"6 + 5 - 2 + 3",          # 12
"5 * 7 ^ 2",              # 245
"(4 * (2 + 3) ^ (2 + 3)", # Parsing error -- ')' expected
"4 * (2 + 3) ^ (2 + 3)",  # 12500
"10 / 5 * 5",             # 10.0
"9 / 4",                  # 2.25
"9 \\ 4"                  # 2 ("\\" is a single \)
]

parser = Parser()
for expression in testList:
    try:
        print("Expression:", expression)
        tree = parser.parse(expression)
        print("Prefix:", tree.prefix())
        print("Infix:", tree.infix())
        print("Postfix:", tree.postfix())
        print("Value:", tree.value())
    except Exception as e:
        print("Error:")
        print(e)
    print() # blank line to separate test cases

