include ../../Make.common


OBJECTS = aq.o cpp_re.o token_ids.o instantiate_re2c_lexer_str.o \
	error_code.o

all : liblexer.a

liblexer.a : ${OBJECTS}
	ar rs $@ ${OBJECTS}
#	rm ${OBJECTS}

aq.o : ${BOOST_DIR}/libs/wave/src/cpplexer/re2clex/aq.cpp
	${CXX} ${CXXFLAGS_BOOST} -I${BOOST_DIR} -c $? -o $@

cpp_re.o : ${BOOST_DIR}/libs/wave/src/cpplexer/re2clex/cpp_re.cpp
	${CXX} ${CXXFLAGS_BOOST} -I${BOOST_DIR} -c $? -o $@

token_ids.o : ${BOOST_DIR}/libs/wave/src/token_ids.cpp
	${CXX} ${CXXFLAGS_BOOST} -I${BOOST_DIR} -c $? -o $@

instantiate_re2c_lexer_str.o : ${BOOST_DIR}/libs/wave/src/instantiate_re2c_lexer_str.cpp
	${CXX} ${CXXFLAGS_BOOST} -I${BOOST_DIR} -c $? -o $@

error_code.o : ${BOOST_DIR}/libs/system/src/error_code.cpp
	${CXX} ${CXXFLAGS_BOOST} -I${BOOST_DIR} -c $? -o $@

clean :
	rm -f liblexer.a ${OBJECTS}
