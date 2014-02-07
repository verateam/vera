#!/usr/bin/python
"""
@file SMTT025

@brief Verifies if space is required after/before starting/ending the template parameters declaration.

@author   richard.bastos

@date     Jan 28, 2014
"""
import sys
import re
import vera

def printToken (item, spaces):
  tokenString = "%s%s_(%d, %d); // %s" % (spaces, item.type, item.line, item.column, item.value)
  print tokenString


def printStatement (statement, spaces):
  indexToken = 0
  indexStatement = 0

  for item in statement.statements:
    token = item.getToken()
    if token.type == "unknown":
       line = "%s{ /* id = %d */" % (spaces, item.id)
       print line
       printStatement(item, spaces + "  ")
       print spaces + "}"

    else:
       printToken(token, spaces)

class Inspector:
    
  def isBranch(self, statement):
    token = statement.getToken()
    return token.type == "unknown" and token.value == "branch"

  def isValid(self, statement):
    token = statement.getToken()
  
    return token.type != "newline" and token.type != "ccomment" and  token.type != "cppcomment" and token.type != "space"

  def isInvalidLastToken(self, branch):
    if len(branch.statements) < 1:
      return False

    last = branch.statements[-1]
    
    if self.isBranch(last):
      return self.isInvalidLastToken(last)

    lastToken = branch.statements[-1].getToken()
    
    if lastToken.type == "space" or lastToken.type == "newline":
       return True

    return False

  def isAllowed (self, branch):
    if len(branch.statements) <= 1:
      return False

    last = branch.statements[-1]
    
    if self.isBranch(last):
      return self.isAllowed(last)
   
    item = branch.statements[-2]
    lastToken = branch.statements[-1].getToken()
    if  self.isBranch(item) == False:
      return False

    if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_TEMPLATEPARAMETERS and\
       lastToken.type == "space":
       
       return True

    return False

  def rule (self, branch):
    length = len(branch.statements)
    if length == 2:
      return
    
    token = branch.statements[0].getToken()
    secondItem = branch.statements[1]
    isError = False
  
    if self.isValid(secondItem) == False:
      isError = self.isAllowed(branch.statements[-2]) == False;
    else:
      isError = self.isInvalidLastToken(branch.statements[-2])

    if isError:
      vera.report(self.fileName, token.line, "Only if the last parameter of the template is a template, a space is required at the start and the end of the statement.")
    
  def run (self, branch):
    
    for item in branch.statements:
      if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_TEMPLATEPARAMETERS:
        self.rule(item)
        
      if self.isBranch(item):
        self.run(item)
     
  """
  @brief Initializes an instance of the Error class.
  """
  def __init__(self, fileName):
    self.fileName = fileName
    self.doc = vera.Document(fileName)
    root = self.doc.getRoot()
    self.last = None
    self.run(root)         
    
for f in vera.getSourceFileNames():
  Inspector(f)
