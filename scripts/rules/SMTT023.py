#!/usr/bin/python
"""
@file SMTT023

@brief Unnecessary spaces or comments in the operator declaration.

@author   richard.bastos

@date     Jan 28, 2014
@revision $Revision: 1$

@copyright 2013 Smartmatic Corp. All Rights Reserved.

@section LICENSE

Confidential Information of Smartmatic Labs. Not for disclosure or
distribution prior written consent. This software contains code, techniques
and know-how which is confidential and proprietary to Smartmatic

Use of this software is subject to the terms of an end user license
agreement.
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
    hasSpace = False
    for item in branch.statements:
      token = item.getToken()
      if self.isValid(item) == False:
        hasSpace = True
        continue
      
      if hasSpace == True and self.isValid(item) == True and\
        (token.type == "minus" or
        token.type == "minusminus" or 
        token.type == "plus" or
        token.type == "plusplus" or
        token.type == "assign" or
        token.type == "leftparen" or
        token.type == "leftbracket" or
        token.type == "dot" or 
        token.type == "dotstart" or 
        token.type == "arrow" or
        token.type == "arrowstart" or
        token.type == "andand" or
        token.type == "and" or
        token.type == "andassign" or
        token.type == "or" or
        token.type == "orassign" or
        token.type == "divide" or
        token.type == "divideassign" or
        token.type == "minusassign" or
        token.type == "plusassign" or
        token.type == "question_mark" or
        token.type == "lessassign" or
        token.type == "greatherassign" or
        token.type == "less" or
        token.type == "not" or
        token.type == "notequal" or
        token.type == "compl" or
        token.type == "xorassign" or
        token.type == "percentassign" or
        token.type == "xor" or
        token.type == "percent" or
        token.type == "shiftleftassign" or
        token.type == "shiftrightassign" or
        token.type == "shiftleft" or
        token.type == "shiftright" or
        token.type == "compl" or
        token.type == "greather"):
        vera.report(self.fileName, token.line, "Unnecessary spaces or comments in the operator declaration.")
        continue
        
    
  def run (self, branch):
    
    for item in branch.statements:
      if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_OPERATOR:
        self.rule(item)
        continue
        
      if self.isBranch(item):
        self.run(item)
     
  """
  @brief Initializes an instance of the Error class.
  """
  def __init__(self, fileName):
    self.fileName = fileName
    self.doc = vera.Document(fileName)
    root = self.doc.getRoot()
    self.run(root)         


for f in vera.getSourceFileNames():
  Inspector(f)
