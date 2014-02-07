#!/usr/bin/python
"""
@file SMTT029

@brief One line and not more than one between the statement and the scope is required.

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

