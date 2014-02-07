#!/usr/bin/python
"""
@file SMTT026

@brief Spaces or comments not allowed between template parameters and its parent.

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
