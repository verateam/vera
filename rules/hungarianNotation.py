#todo: add better error handling
#todo: handle mutiple same line varible dec ie int a,b,c;
#todo: templated type names
#todo: proper mappings of names to prefix
#todo: mutiple template argumetns
#todo: templates in templates
#todo: finish mapping var anems to prefix
#todo: check for prefix on string for types in mapping
#todo: parathense intialisaiton like string abc("abc");
#todo: template<typename>

#known bug list/incomplete feature list
#The m_ prefix is allowed but there is no forcing to appear only for class members
#The _ prefix is allowed but there is no forcing to appear only for parameters (ie int a(7), Cat kitty("meow")) are never evaluated for following hungarian notation
#Variables whose type is a tempalted type are never tested for hungarian notation. Usually this is not big because they are a custom type (see below) but if they are a pointer to a type the pointers needing to have the p prefix is not enforeced
#Variables of classes and other data types 
#Not all data types (especially those who are custom) do not a defined prefix so they are checked for prefix and pointers but can not be checked for having a proper type prefix. So "Cat dog;", "Cat cat;", "Cat cMyCat", and "Cat* pMyCat", will all pass but "Cat* mycat;" will not
#The type char* is used for alot of different types and some of these have doccumented prefixes ("sz", "str") but some do not
#Declaring a type using a typedef will make the type be treated as a custom class and thus script. So typedef int Number would make Number coolNumber valid.
#Declaring a variable as auto will skip hungrarian notation validation. So auto coolThing = (int)5 is valid. 

#char vars can be prefixed with str even if tehy arent chars.
import string

anyErrors = False

class dbcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def setErrors(val):
    global anyErrors
    anyErrors = val

def main():
    for f in vera.getSourceFileNames():
        list = []
        print(f)
        for t in vera.getTokens(f, 1, 0, -1, -1, []):
            #print(t.type + " " + t.value)
            list.append(t);
        FileTokenProccessor(list, f)
        print("====================================================================");
    if anyErrors:
        raise Exception("One or more instance of notation being broked")

class FileTokenProccessor:

    def __init__(self, list, f_):
        self.tokenList = list
        self.setupProccess()
        self.fName = f_

        #do some proccessing on the token list such as removeing
        #all instance of namespace::
        index = 1
        for i  in reversed(range(0,len(self.tokenList))):
            if self.tokenList[i].type == "colon_colon":
                a  = 4
                self.tokenList.pop(i)
                if i != 1:
                    #print("removed")
                    if self.tokenList[i - 1].type =="identifier":
                        #print(self.tokenList[i-1].value)
                        self.tokenList.pop(i - 1)
            index += 1

        #Go through each token and check for instance where hungarain is broken 
        self.proccessFileTokens()
        

    def proccessFileTokens(self):
        self.setupProccess()
        while self.hasTokens():
            self.getNextVariableDecl()
            

    #Search until we find a thing that look like a variable declaration defined in the form of varType [modifers] varName [ = stuff];
    def getNextVariableDecl(self):
        typeNameTokens = self.getNextVariableTypeDecl()
        varName = self.getNextTokenNotOfTypes(["newline","space"])
        lineNum = varName.line
        if varName.type ==  "identifier":
            endSignal = self.getNextTokenNotOfTypes(["newline","space"])
            if endSignal.type  in ["semicolon", "assign", "rightparen", "comma", "leftbracket"] :
                if not self.validateHungarianNotation(typeNameTokens, varName.value):
                    setErrors(True)
                    print(dbcolors.FAIL + typeNameTokens + " " + varName.value + dbcolors.ENDC +  " breaks notation: " + self.fName + " line " + str(lineNum))


    #Given a type name and varname check if the duo follows hungarian notation 
    def validateHungarianNotation(self, typeName, varName):
        #remove prefixes


        #some types have names associated with them from convention
        #So check this corrner case
        #Map of types to a list of names that are convetionally used
        conventionNames = {"int" : [ "i" , "argc" ] , "int32_t" : ["i"] , "int64_t" : ["i"] , "uint32_t" : ["i"] , "int64_t" : ["i"]  ,
                           "char*" : ["argv"] }

        if typeName in conventionNames:
            conventionList = conventionNames[typeName]
            for con in conventionList:
                if varName == con:
                    return True
        
        pre_prefixes = ["m_", "_" , "g_", "s_"]
        for p in pre_prefixes:
            if len(varName) > len(p):
                pre = varName[:len(p)]
                if pre == p:
                    varName = varName[len(p):]
                    break

        if typeName[-1] == "&":
            typeName = typeName[:-1]

        #check if it is a pointer
        if typeName[-1] == "*" and typeName != "char*":
            if varName[:1] != "p":
                return False
            else:
                #strip the pointer
                #print("t: " + varName)
                typeName = typeName[:-1]
                varName = varName[1:]
                #print(varName)

        if varName == varName.upper():
            return True

        #python map
        #init it somewhere
        #check if typeName in map
        #if not use custom class stuff
        #otherwise get prefix from map
        #verify the prefix is the beigng of the string
        #handle memebr prefix though
        typeToPrefix = { "int" : ["n"], "string" : ["str" , "sz"],
                         "bool" : ["b" , "is"],
                         "int16_t" : ["n"] , "int32_t" : ["n"], "int64_t" : ["n"],
                         "uint16_t" : ["n" , "u"] , "uint32_t" :["n" , "u"], "uint64_t" : ["n" , "u"],
                         "char*" : ["sz", "str" , "func"], "char" : ["c" , "str", "sz"],
                         "double" : "f", "float" : "f"}
        if typeName in typeToPrefix:
            prefixList = typeToPrefix[typeName]
            for prefix in prefixList:
                prefixSize = len(prefix)
                if varName[:prefixSize] ==  prefix:
                    return True
            
            return False
        else:
            #print(typeName + " " + varName)
            return True
            #do custom class
        return True

    def setupProccess(self):
        self.currentTokenIndex = 0

    def getCurrentToken(self):
        try:
            return self.tokenList[self.currentTokenIndex]
        except:
            errorMsg = "index out of range"
            return self.tokenList[len(self.tokenList) - 1]

    def getNextToken(self):
        self.currentTokenIndex += 1
        return self.tokenList[self.currentTokenIndex]

    def getNextTokenNotOfTypes(self, typeList):
        startToken = self.getCurrentToken()
        try:
            while True:
                cToken = self.getNextToken()
                if cToken.type not in typeList:
                    return cToken
        except:
            errorMsg = "out of range"
            self.currentTokenIndex = len(self.tokenList) + 1
            return startToken

    def getPreviousToken(self):
        return self.tokenList[self.currentTokenIndex - 1]

    def getPreviousTokenNotOfTypes(self, tList):
        try:
            prev = 1
            while True:
                prevToke = self.tokenList[self.currentTokenIndex - prev]
                if prevToke.type not in tList:
                    return prevToke
                prev += 1
        except:
            errorMsg = "out of range"

    def hasTokens(self):
        return len(self.tokenList) > self.currentTokenIndex

    def getNextTokenOfTypes(self, typeList):
        startToken = self.getCurrentToken()
        try:
            while True:
                cToken = self.getNextToken()
                if cToken.type in typeList:
                    return cToken
        except:
            errorMsg = "out of range"
            self.currentTokenIndex = len(self.tokenList) + 1
            return startToken
            
    def getNextTokenOfType(self, type):
        return self.getNextTokenOfTypes([type])

    def getNextTokenOfTypesPrecededByTypes(self,typeList, precededTypeList):
        while True:
            typeToken = getNextTokenOfTypes(typeList)
            if(self.getPreviousToken().type in precededTypeList):
                return typeToken
            
    def getNextTokenOfTypesPrecededByType(self,typeList, precededType):
        return getNextTokenOfTypesPrecededByTypes(typeList,[precededType])

    def getNextTokenOfTypesPrecededByTypes(self,typeN, precededTypeList):
        return getNextTokenOfTypesPrecededByTypes([typeN],precededTypeList)

    def getNextTokenOfTypesPrecededByTypes(self,typeN, precededType):
        return getNextTokenOfTypesPrecededByTypes([typeN],[precededType])

    def moveToNextTokenIfOfTypes(self, typeList):
        return self.moveToNextTokenIfOfTypesSkippingTypes(typeList,[])

    def moveToNextTokenIfOfTypesSkippingTypes(self, typeList, skipList):
        indexPlus = 1
        while True:
            if self.tokenList[self.currentTokenIndex + indexPlus].type in skipList:
                indexPlus+=1
                continue
            if self.tokenList[self.currentTokenIndex + indexPlus].type in typeList:
                 self.currentTokenIndex += indexPlus
                 return True
            return False
    
    def getNextVariableTypeDecl(self):

        while True:
            varTypeTokenList = ["identifier", "int", "char", "bool", "string"]

            typeNamePartToken = self.getNextTokenOfTypes(varTypeTokenList) 
            typeDec = typeNamePartToken.value

            pastToke = self.getPreviousTokenNotOfTypes(["newline","space", "colon_colon"])
            if pastToke is None:
                return

            if pastToke.type == "typedef":
                continue

            if self.moveToNextTokenIfOfTypesSkippingTypes(["star", "and"], ["newline","space", "colon_colon"]):
                typeDec += self.getCurrentToken().value

            return typeDec

    
    
main()
