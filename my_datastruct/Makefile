CC=g++
AR=ar
CPPFLAGS= -O2 -Wall -g
OBJECT=buffer.o
LIB_SRC=buffer.cc
DEMO_SRC=buffer_demo.cc
LIB=datastruct.a
BIN=

all:
	$(CC) $(CFLAGS) -o ./demo/$(DEMO_SRC) $(OBJECT) -std=c++11

$(LIB): $(OBJECT)
	$(AR) -r $(LIB) $(OBJECT)

$(OBJECT):%.o:%.cc
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm  main $(OBJECT)
#https://blog.csdn.net/aflyeaglenku/article/details/47779941