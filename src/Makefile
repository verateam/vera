include Make.common


all : vera++

STRUCTURES_OBJ = structures/SourceFiles.o structures/SourceLines.o structures/Tokens.o
PLUGINS_OBJ = plugins/Interpreter.o plugins/Profiles.o plugins/Rules.o plugins/Exclusions.o \
	plugins/Transformations.o plugins/RootDirectory.o plugins/Parameters.o \
	plugins/Reports.o plugins/cpptcl-1.1.4/cpptcl.o

LIBS_DIRS = -L./structures/lexer -L${TCLLIB_DIR}
LIBS = -l${TCL_LIB} -llexer

ALL_OBJ = ${STRUCTURES_OBJ} ${PLUGINS_OBJ}

vera++ : main.o structures_objects plugins_objects liblexer
	${CXX} -o $@ main.o ${ALL_OBJ} ${LIBS_DIRS} ${LIBS}


liblexer :
	make -C structures/lexer

main.o : main.cpp
	${CXX} -c -o $@ ${CXXFLAGS} $?

structures_objects :
	make -C structures

plugins_objects :
	make -C plugins


clean :
	rm -f vera++ main.o
	make -C structures clean
	make -C structures/lexer clean
	make -C plugins clean
