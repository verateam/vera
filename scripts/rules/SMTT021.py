#!/usr/bin/python
"""
@file SMTT021

@brief Verify if all related scopes has braces or not.

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
  
  unknown, no, yes = range(3)
    
  def isBranch(self, statement):
    token = statement.getToken()
    return token.type == "unknown" and token.value == "branch"

  def rule (self, branch):
    token = branch.getToken()
    vera.report(self.fileName, token.line, "All related scopes has braces or not.") 

   # def verifyIf(self, branch):
   #  listBraces =elseBranch (item for item in branch.statements if  self.isBranch(item)  and item.type != vera.TypeItem.TYPE_ITEM_STATEMENT_OF_PARENSARGUMENTS)
     
  def verifyIf(self, branch):
  
    aux = sum(1 for item in branch.statements if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ELSE)
     
    if aux == 0:
      return

    firstScope = (item for item in branch.statements if  self.isBranch(item)  and item.type != vera.TypeItem.TYPE_ITEM_STATEMENT_OF_PARENSARGUMENTS).next()
    items = (item for item in branch.statements if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ELSE)
      
    elseBranch = items.next();
    
    if elseBranch == None:
      return
    
    secondScope = (item for item in elseBranch.statements if  self.isBranch(item)  and item.type != vera.TypeItem.TYPE_ITEM_STATEMENT_OF_PARENSARGUMENTS).next()
    
    if (firstScope.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS and 
       secondScope.type != vera.TypeItem.TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS and
       secondScope.type != vera.TypeItem.TYPE_ITEM_STATEMENT_OF_IF) or\
       (firstScope.type != vera.TypeItem.TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS and 
       secondScope.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS):
      self.rule(branch.statements[0]) 
    
     
  def verifySwitch(self, branch):
    scope = (item for item in branch.statements if  self.isBranch(item)  and item.type != vera.TypeItem.TYPE_ITEM_STATEMENT_OF_PARENSARGUMENTS).next()
    
    hasBraces = self.unknown;
    isError = False
     
    for item in scope.statements:
   
      if item.type == vera.TypeItem.TYPE_ITEM_TOKEN:
        continue
      
      if ((item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS and hasBraces == self.no) or
           (item.type != vera.TypeItem.TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS and hasBraces == self.yes)):
        isError = True
        break;
       
      if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS:
        hasBraces = self.yes;
      else:
        hasBraces = self.no;
         
    if isError == True:
      self.rule(branch.statements[0])  
         
  def run (self, branch):
    
    for item in branch.statements:
      if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_SWITCH:  
        self.verifySwitch(item)
        
      if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_IF:
        self.verifyIf(item)     
        
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
