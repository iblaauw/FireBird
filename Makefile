OUTDIR=./build

SOURCE_DIRS=core

SOURCES=$(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)/*.cpp))
OBJECTS=$(SOURCES:%.cpp=$(OUTDIR)/%.o)

EGG_SOURCES=$(wildcard egg/*.cpp)
EGG_OBJECTS=$(EGG_SOURCES:%.cpp=$(OUTDIR)/%.o)

CPPFLAGS=--std=c++11

.PHONY: clean

default: all

all: firebird fireegg

firebird: $(OBJECTS)
	g++ $^ -o $@

fireegg: $(EGG_OBJECTS)
	g++ $^ -o $@

$(OUTDIR)/%.o : %.cpp
	mkdir -p $(@D)
	g++ $(CPPFLAGS) -c $^ -o $@


clean:
	rm -fr build/*
	rm -f firebird.exe
