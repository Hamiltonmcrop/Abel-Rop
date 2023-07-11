"""
File: parser.py
Author: Leigh Stauffer
Project 10

This parsing program also builds and returns an expression tree.
"""

from tokens import Token
from scanner import Scanner
from expressiontree import LeafNode
from expressiontree import InteriorNode

class Parser(object):
    """Represents a parser for arithmetic expressions."""

    def parse(self, sourceStr):
        """Sets up and runs the parser on a source string."""
        self.completionMessage = "No errors"
        self.parseSuccessful = True
        self.scanner = Scanner(sourceStr)
        self.tree = self.expression()
        self.accept(self.scanner.get(), Token.EOE,
                    "symbol after end of expression")
   
    def parseStatus(self):
        """Returns the completion message for the parse."""
        if self.parseSuccessful:
            self.completionMessage += "\nvalue: " + str(self.tree.value()) + \
                                     "\nPreorder: " + self.tree.prefix() + \
                                     "\nInorder: " + self.tree.infix() + \
                                     "\nPostorder: " + self.tree.postfix()
        return self.completionMessage
    
    def accept(self, token, expected, errorMessage):
        """Checks the type of the given token for correctness."""
        if token.getType() != expected:
            self.fatalError(token, errorMessage)

    def fatalError(self, token, errorMessage):
        """Stops the parse with a syntax error messahge."""
        self.parseSuccessful = False
        self.completionMessage = "Parsing error -- " + \
                                 errorMessage + \
                                 "\nExpression so far = " + \
                                 self.scanner.stringUpToCurrentToken()
        raise Exception(self.completionMessage)

    # expression = term { addingOperator term }
    def expression(self):
        tree = self.term()
        token = self.scanner.get()
        while token.getType() in (Token.PLUS, Token.MINUS):
            self.scanner.next()
            tree = InteriorNode(token, tree, self.term())
            token = self.scanner.get()
        return tree

    # term = factor { multiplyingOperator factor }
    def term(self):
        tree = self.factor()
        token = self.scanner.get()
        while token.getType() in (Token.MUL, Token.DIV):
            self.scanner.next()
            tree = InteriorNode(token, tree, self.factor())
            token = self.scanner.get()
        return tree

    # factor = primary [ "^" factor ]
    def factor(self):
        tree = self.primary()
        token = self.scanner.get()
        if token.getType() == Token.EXPO:
            self.scanner.next()
            tree = InteriorNode(token, tree, self.factor())
        return tree
            

    # primary = numericLiteral | "(" expression ")"
    def primary(self):
        token = self.scanner.get()
        if token.getType() == Token.INT:
            tree = LeafNode(token.getValue())
            self.scanner.next()
        elif token.getType() == Token.L_PAR:
            self.scanner.next()
            tree = self.expression()
            self.accept(self.scanner.get(),
                        Token.R_PAR,
                        "')' expected")
            self.scanner.next()
        else:
            tree = LeafNode (token.getValue())
            self.fatalError(token, "bad primary")
        return tree

