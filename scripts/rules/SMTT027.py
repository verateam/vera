#!/usr/bin/python
"""
@file SMTT029

@brief One line and not more than one between the statement and the scope is required.

@author   richard.bastos

@date     Jan 28, 2014
"""
import sys
import re
import vera

def push(branch):
  return  branch.statements[0]

def isBranch(statement):
  token = statement.getToken()
  return token.type == "unknown" and token.value == "branch"

def rule (branch, fileName):
  scope = (item for item in branch.statements if item.type != vera.TypeItem.TYPE_ITEM_TOKEN)
   
  scope = scope.next()
  
  start = branch.statements[0]
  tokenA = start.getToken()
  
  tokenB = push(scope).getToken();
  
  diff = tokenB.line - tokenA.line
  
  if branch.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ELSE and\
     scope.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_IF and\
     diff == 0:
    return
   
  if diff != 1:
    vera.report(fileName, tokenB.line, "One line and not more than one between the statement and the scope is required.")

def verifyStatement (statement, fileName):
  
  lastToken = statement.getToken()
  for item in statement.statements:
   
    if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_NAMESPACE:
      rule(item, fileName)
 
    if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_EXTERN:
      rule(item, fileName)
      
    if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_DOWHILELOOP:
      rule(item, fileName)
            
    if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_UNION:
      rule(item, fileName) 
      
    if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ENUM:
      rule(item, fileName) 
      
    if item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ELSE:
      rule(item, fileName) 
      
    if isBranch(item): 
       verifyStatement(item, fileName)
    
for f in vera.getSourceFileNames():
  doc = vera.Document(f)
  verifyStatement(doc.getRoot(), f)

