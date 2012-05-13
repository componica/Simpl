# Eventually this should be replaced with an AutoConf.
CC = gcc
CFLAGS = -O3 -Wall -ansi

INCLUDE_DIR = include
MAIN_SRC_DIR = src
THIRD_PARTY = third_party/jpeg-8b third_party/lpng-1.2.49 third_party/zlib-1.2.7
THIRD_PARTY_DIR = $(addprefix src/,$(THIRD_PARTY))

SRC_DIR = $(MAIN_SRC_DIR) $(THIRD_PARTY_DIR)

SRC = $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
OBJ = $(patsubst src/%.c,src/%.o,$(SRC))

LIB = libsimpl.a

.c.o:
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I./src/third_party -o $*.o -c $*.c

all: $(LIB)

$(LIB): $(OBJ)
	rm -f $@
	ar rcs $@ $(OBJ)

clean:
	rm -f $(LIB) $(OBJ) *.o *.exe *.stackdump core

install:
	cp -f $(LIBS) /usr/local/lib
	rm -rf /usr/local/include/libSimpl
	cp -rf $(INCLUDE_DIR) /usr/local/include/libSimpl