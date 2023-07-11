"""
File: scanner.py
Author: Ken Lambert, copyright 2015, 2020
Used by permission.

Language processing scanner.

Source: Textbook chapter 10 Parsing and Expression Trees case study.
Adapted by Derrf Seitz.

The following files must be in the same folder:
  tokens.py
"""

from tokens import Token

class Scanner(object):

    EOE = ';'   # end-of-expression
    TAB = '\t'  # tab

    def __init__(self, sourceStr):
        self.sourceStr = sourceStr
        self.getFirstToken()

    # Returns Token.EOE when the string has been scanned.
    def get(self):
        return self.currentToken

    def getFirstToken(self):
        self.index = 0
        self.currentChar = self.sourceStr[0]
        self.getNextToken()
    
    def getInteger(self):
        num = 0
        while True:
            num = num * 10 + int(self.currentChar)
            self.nextChar()
            if not self.currentChar.isdigit():
                break
        return num
    
    def getNextToken(self):
        self.skipWhiteSpace()
        if self.currentChar.isdigit():
            self.currentToken = Token(self.getInteger())
        elif self.currentChar == Scanner.EOE:
            self.currentToken = Token(';')
        else:
            self.currentToken = Token(self.currentChar)
            self.nextChar()

    def hasNext(self):
        return self.currentToken != Token.EOE

    def next(self):
        temp = self.currentToken
        self.getNextToken()
        return temp
    
    def nextChar(self):
        if self.index >= len(self.sourceStr) - 1:
            self.currentChar = Scanner.EOE
        else:
            self.index += 1
            self.currentChar = self.sourceStr[self.index]
    
    def skipWhiteSpace(self):
        while self.currentChar in (' ', Scanner.TAB):
            self.nextChar()

    def stringUpToCurrentToken(self):
        return self.sourceStr[0:self.index + 1]


