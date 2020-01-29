CC=g++
CPPFLAGS= -Wall -g
OBJECT=test_prog.o
SRC=test_prog.cc

all: $(OBJECT)
	$(CC) $(CFLAGS) -o test_prog $(OBJECT) -std=c++11

$(OBJECT):%.o:%.cc
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm  test_prog $(OBJECT)