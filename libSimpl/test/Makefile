CC = gcc

CFLAGS = -I../include/ -ggdb -Wall -W -ansi

test = unit_tests.exe
test_src = main.c

lib = ../libsimpl.a -lm
test_objects = $(patsubst %.c, %.o, $(test_src))
test_dependencies = $(subst .o,.d,$(test_objects))

define make-depend
  $(CC) $(CFLAGS) -M $1 | \
  $(SED) -e 's,\($(notdir $2)\) *:,$(dir $2)\1: ,' > $3.tmp
  $(SED) -e 's/#.*//' \
      -e 's/^[^:]*: *//' \
      -e 's/ *\\$$//' \
      -e '/^$$/ d' \
      -e 's/$$/ :/' $3.tmp >> $3.tmp
  $(MV) $3.tmp $3
endef

all: $(test)

$(test): $(test_objects)
	@echo Linking $(test)...
	@$(CC) -o $(test) $(test_objects) $(lib)
	@echo Running unit tests...
	@./$(test)

clean:
	rm -f *.exe $(test_objects) $(test_dependencies) 2> /dev/null
