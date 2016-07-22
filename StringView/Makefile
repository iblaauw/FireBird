CPPFLAGS=-Wall -Werror -std=c++11
DEST_DIR=../lib

all: StringView.a

StringView.a : StringView.o
	mkdir -p $(DEST_DIR)
	ar crf $@ $^
	cp $@ $(DEST_DIR)

%.o : %.cpp
	g++ $(CPPFLAGS) $^ -c -o $@

clean:
	rm -f *.o -r
	rm -f *.a -r
