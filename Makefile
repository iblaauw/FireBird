OUTDIR=./build

SOURCE_DIRS=core

SOURCES=$(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)/*.cpp))
OBJECTS=$(SOURCES:%.cpp=$(OUTDIR)/%.o)

EGG_SOURCES=$(wildcard egg/*.cpp)
EGG_OBJECTS=$(EGG_SOURCES:%.cpp=$(OUTDIR)/%.o)

COMMON_SOURCES=$(wildcard common/*.cpp)
COMMON_OBJECTS=$(COMMON_SOURCES:%.cpp=$(OUTDIR)/%.o)

LIB_SEARCH_FLAGS=-Lparse_args/lib
LIB_FLAGS=-lparse_args

INCLUDES=-Icommon -Iparse_args/inc

CPPFLAGS=--std=c++11 -Wall

.PHONY: clean

default: all

all: firebird fireegg

firebird: $(OBJECTS) $(COMMON_OBJECTS)
	g++ $(LIB_SEARCH_FLAGS) $^ $(LIB_FLAGS) -o $@

fireegg: $(EGG_OBJECTS) $(COMMON_OBJECTS)
	g++ $(LIB_SEARCH_FLAGS) $^ $(LIB_FLAGS) -o $@

test: fireegg
	rm -f test1.fb
	./fireegg.exe test1.fire -o test1.fb

test%: test/test%.fb firebird test/test%.ans
	./firebird $< -t test/test$*.ans

%.fb: %.fire
	./fireegg.exe $^ -o $@

$(OUTDIR)/%.o : %.cpp
	mkdir -p $(@D)
	g++ $(CPPFLAGS) $(INCLUDES) -c $^ -o $@


clean:
	rm -fr build/*
	rm -f firebird.exe
	rm -f fireegg.exe
	rm -f *.stackdump
