CXX=c++
CXXFLAGS= -std=gnu++17 -fPIC -g
LIB=libClangTidy2310.so
OBJS=GotoCheck.o StaticLocalCheck.o FileExtensionsUtils.o HeaderGuard.o \
     LLVMHeaderGuardCheck.o DoubleAssignmentCheck.o FileCommentCheck.o \
     FileCommentCompleteCheck.o InappropriateCommaOperatorCheck.o \
     NonConstVariableDeclarationCheck.o \
     CSSE2310Checks.o
COURSEDIR=/local/courses/csse2310

$(LIB): $(OBJS)
	$(CXX) -fPIC -shared -o $(LIB) $(OBJS)

install: $(LIB)
	install -m775 $(LIB) $(COURSEDIR)/lib

clean:
	rm -f *.o *.so

CSSE2310Checks.o: *.h
