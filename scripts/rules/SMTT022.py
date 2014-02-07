#!/usr/bin/python
"""
@file SMTT022

@brief Verifies if there' an empty line is required to separate both sentences.

@author   richard.bastos

@date     Feb 04, 2014
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

class Element:
  unknown, comment, branch, statement = range(4)
    
  def __init__(self, type, firstLine, lastLine, typeOfStatement):
    self.type = type
    self.firstLine = firstLine
    self.lastLine = lastLine
    self.typeOfStatement = typeOfStatement  
    
  def getType(self):
    return self.type
  
  def getFirstLine(self):
    return self.firstLine
  
  def getLastLine(self):
    return self.lastLine
  
  def getTypeOfStatement(self):
    return self.typeOfStatement

class Inspector:
    
  def isBranch(self, statement):
    token = statement.getToken()
    return token.type == "unknown" and token.value == "branch"

  def lastToken (self, branch):
    if self.isBranch(branch):
      item = branch.statements[-1]
      if self.isBranch(item):
        scope = item.statements[-1]
        return self.lastToken(scope)
      else:
        return item.getToken();
    
    return branch.getToken();
  
  def firstToken (self, branch):
    
    if self.isBranch(branch):
      item = branch.statements[0] 
      
      if self.isBranch(item):
        return self.firstToken(item)
      else:
        return item.getToken();
      
      return branch.getToken();
    
  def isComment(self, item):
    token = item.getToken();
      
    return token.type == "ccomment" or token.type == "cppcomment"
    
  def rule (self, line):
    vera.report(self.fileName, line, "One empty line is required to separate both sentences.")   
    
  def verify(self, elements):
    last = Element(Element.unknown, 0, 0, vera.TypeItem.TYPE_ITEM_TOKEN)
    
    for item in elements:
     
      if item.getType() == Element.statement and (last.getType() == Element.statement or last.getType() == Element.branch):
        diff = item.getFirstLine() - last.getLastLine()
        
        if diff != 2:
          if (last.getTypeOfStatement() == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_TEMPLATEPARAMETERS and
            (item.getTypeOfStatement() == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_CLASS or
            item.getTypeOfStatement() == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_STRUCT or
            item.getTypeOfStatement() == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_FUNCTION)):
            continue
          
          if (last.getTypeOfStatement() == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ACCESSMODIFIERS and
              item.getTypeOfStatement() == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ACCESSMODIFIERS):
            continue
          
          if (last.getTypeOfStatement() == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_PREPROCESSORLINE or
              item.getTypeOfStatement() == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_PREPROCESSORLINE):
            continue
          
          # line = "%d %d %s %s" % (last.getFirstLine(), last.getLastLine(), last.getTypeOfStatement(), item.getTypeOfStatement())
          # print line
          self.rule(item.getFirstLine())
          
      if item.getType() == Element.branch and last.getType() == Element.statement:
        diff = item.getFirstLine() - last.getLastLine()
        
        if diff != 2:
          if (last.getTypeOfStatement() == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ACCESSMODIFIERS and
              item.getTypeOfStatement() == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ACCESSMODIFIERS):
            continue
          
          if (last.getTypeOfStatement() == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_PREPROCESSORLINE or
              item.getTypeOfStatement() == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_PREPROCESSORLINE):
            continue
          
          # line = "%d %d %s %s" % (last.getFirstLine(), last.getLastLine(), last.getTypeOfStatement(), item.getTypeOfStatement())
          # print line
          self.rule(item.getFirstLine())
      
      last = item
       
  def isValid(self, item): 
      return (item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_IF or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_FORLOOP or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_WHILELOOP or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_DOWHILELOOP or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_CLASS or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_TRYCATCHES or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_EXTERN or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_NAMESPACE or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ENUM or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_SWITCH or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_FUNCTION or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_TEMPLATE or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ACCESSMODIFIERS or
        item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_IF)
      
  def linesOfComment(self, token):
    content = token.value
    lines = content.count('\n')
    return token.line + lines
  
  def run (self, branch):
    # statements = (item for item in branch.statements if self.isValid(item))
    # branchs = (item for item in branch.statements if self.isBranch(item) and self.isValid(item) == False)    
    # comments = (item for item in branch.statements if self.isComment(item))
    
    elements = []
    
    for item in branch.statements:
      if self.isComment(item) == True:
        token = item.getToken();
        first = token.line
        last = self.linesOfComment(token)
        elements.append(Element(Element.comment, first, last, item.type))
        
      if self.isValid(item) == True:
        first = self.firstToken(item)
        last = self.lastToken(item)
        elements.append(Element(Element.statement, first.line, last.line, item.type))
        
      if self.isBranch(item) == True and self.isValid(item) == False:
        first = self.firstToken(item)
        last = self.lastToken(item)
        elements.append(Element(Element.branch, first.line, last.line, item.type))
    
    self.verify(elements)
    
    for item in branch.statements: 
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
