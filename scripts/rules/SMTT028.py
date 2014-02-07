#!/usr/bin/python
"""
@file SMTT028

@brief Verifies if there's unnecessary comment between the statement and the scope.

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

def rule (token, fileName):
  if token.type == "ccomment" or token.type == "cppcomment":
    vera.report(fileName, token.line, "Comments should not exist between the statement and the scope.")
  
def isBranch (statement):
  token = statement.getToken()
  return token.type == "unknown" and token.value == "branch"
 
def StatementVerifier (branch, fileName):
  
  for item in branch.statements:
    token = item.getToken()
    rule(token, fileName)
    
def RecursiveVerifier (branch, fileName):
  
  for item in branch.statements:
    token = item.getToken()
    rule(token, fileName)
    
    if isBranch(item):
      RecursiveVerifier(item, fileName);
      
def VerifierOfStatementWithConditional (branch, fileName):
  for item in branch.statements:
    token = item.getToken()
    if isBranch(item) and item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_PARENSARGUMENTS or\
       item.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_WHILE:
      RecursiveVerifier(item, fileName)
    else:
      rule(token, fileName)

def verifyStatement (statement, fileName):
    
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_NAMESPACE:
    StatementVerifier(statement, fileName)
    
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_EXTERN:
    StatementVerifier(statement, fileName)
    
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_SWITCH:
    StatementVerifier(statement, fileName)
    
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_IF:
     VerifierOfStatementWithConditional(statement, fileName)
     
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_FORLOOP:
     VerifierOfStatementWithConditional(statement, fileName)
     
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_TEMPLATE:
    StatementVerifier(statement, fileName)
     
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_WHILELOOP:
     VerifierOfStatementWithConditional(statement, fileName)
     
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_DOWHILELOOP:
    VerifierOfStatementWithConditional(statement, fileName)
     
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ELSE:
    StatementVerifier(statement, fileName)
    
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_STRUCT:
    StatementVerifier(statement, fileName)
    
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_CLASS:
    StatementVerifier(statement, fileName)
    
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_ENUM:
    StatementVerifier(statement, fileName)
    
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_UNION:
    StatementVerifier(statement, fileName)
    
  if statement.type == vera.TypeItem.TYPE_ITEM_STATEMENT_OF_HERITAGE:
    RecursiveVerifier(statement, fileName)
    
  for item in statement.statements:
    if isBranch(item): 
       verifyStatement(item, fileName)
  
for f in vera.getSourceFileNames():
  doc = vera.Document(f)
  verifyStatement(doc.getRoot(), f)


