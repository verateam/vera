include ../Make.common

CPPTCL = cpptcl-1.1.4

OBJECTS = Interpreter.o Profiles.o Rules.o Exclusions.o Transformations.o \
	RootDirectory.o Parameters.o Reports.o ${CPPTCL}/cpptcl.o

all : ${OBJECTS}

Interpreter.o : Interpreter.cpp
	${CXX} -c ${CXXFLAGS_TCL} -I${TCLINCLUDE_DIR} -I${BOOST_DIR} $?

Profiles.o : Profiles.cpp
	${CXX} -c ${CXXFLAGS_TCL} -I${TCLINCLUDE_DIR} -I${BOOST_DIR} $?

Exclusions.o : Exclusions.cpp
	${CXX} -c ${CXXFLAGS_TCL} -I${TCLINCLUDE_DIR} -I${BOOST_DIR} $?

${CPPTCL}/cpptcl.o : ${CPPTCL}/cpptcl.cpp
	${CXX} -o $@ -c ${CXXFLAGS_TCL} -I${TCLINCLUDE_DIR} -I${BOOST_DIR} $?


%.o : %.cpp
	${CXX} ${CXXFLAGS} -c $<


clean :
	rm -f ${OBJECTS}
