#!/usr/bin/python
"""
@file SMTT026

@brief Spaces or comments not allowed between template parameters and its parent.

@author   richard.bastos

@date     Jan 28, 2014
"""
import sys
import re
import vera

class Inspector:
    
  def isBranch(self, statement):
    token = statement.getToken()
    return token.type == "unknown" and token.value == "branch"

  def isValid(self, statement):
    token = statement.getToken()
  
    return token.type != "newline" and token.type != "ccomment" and  token.type != "cppcomment" and token.type != "space"

  def rule (self, branch):
    token = branch.getToken()
    vera.report(self.fileName, token.line, "Spaces or comments not allowed between template parameters and its parent.")
    
  def run (self, branch):
    
    for item in branch.statements:
      if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_TEMPLATEPARAMETERS:
  
        if self.last != None and self.isValid(self.last) == False:
          self.rule(self.last)
        
      if self.isBranch(item):
        self.run(item)
      else:
        self.last = item
     
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
