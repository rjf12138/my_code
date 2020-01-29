CC=g++
CPPFLAGS= -Wall -g
OBJECT=main.o bit_operation.o build_tree.o rwfile.o \
		decode.o encode.o compress.o extract.o compress_extract.o
SRC=main.cc bit_operation.cc build_tree.cc rwfile.cc \
		decode.cc encode.cc compress.cc extract.cc compress_extract.cc

all: $(OBJECT)
	$(CC) $(CFLAGS) -o main $(OBJECT) -std=c++11

$(OBJECT):%.o:%.cc
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm  main $(OBJECT)
