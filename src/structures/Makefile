include ../Make.common


OBJECTS = SourceFiles.o SourceLines.o Tokens.o

all : ${OBJECTS}

Tokens.o : Tokens.cpp
	${CXX} -c ${CXXFLAGS_BOOST} -I${BOOST_DIR} $?

%.o : %.cpp
	${CXX} ${CXXFLAGS} -c $<


clean :
	rm -f ${OBJECTS}
