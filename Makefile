SOURCE_DIRS=core

SOURCES=$(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)/*.cpp))

OBJECTS=$(SOURCES:%.cpp=$(OUTDIR)/%.o)

OUTDIR=./build

CPPFLAGS=--std=c++11

.PHONY: clean

default: all

all: firebird

firebird: $(OBJECTS)
	g++ $^ -o $@


$(OUTDIR)/%.o : %.cpp
	mkdir -p $(@D)
	g++ $(CPPFLAGS) -c $^ -o $@


clean:
	rm -fr build/*
	rm -f firebird.exe
