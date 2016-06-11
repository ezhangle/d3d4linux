
BINARIES = compile-hlsl d3d4linux.exe

all: $(BINARIES)

check: all
	./compile-hlsl sample_ps.hlsl ps_main ps_4_0

INCLUDE = include/d3d4linux.h \
          include/d3d4linux_impl.h

CXXFLAGS += -Wall -I./include -std=c++11

d3d4linux.exe: d3d4linux.cpp $(INCLUDE) Makefile
	x86_64-w64-mingw32-c++ $(CXXFLAGS) $(filter %.cpp, $^) -static -o $@ -ldxguid

compile-hlsl: compile-hlsl.cpp $(INCLUDE) Makefile
	$(CXX) -g $(CXXFLAGS) $(filter %.cpp, $^) -o $@

clean:
	rm -f $(BINARIES)

