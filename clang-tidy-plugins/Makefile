CXX=c++
CXXFLAGS= -std=gnu++17 -fPIC
LIB=libClangTidy2310.so
OBJS=StaticLocalCheck.o FileExtensionsUtils.o HeaderGuard.o CSSE2310Checks.o \
     LLVMHeaderGuardCheck.o DoubleAssignmentCheck.o
COURSEDIR=/local/courses/csse2310

$(LIB): $(OBJS)
	$(CXX) -fPIC -shared -o $(LIB) $(OBJS)

install: $(LIB)
	install -m775 $(LIB) $(COURSEDIR)/lib

clean:
	rm -f *.o *.so
