# Specific part of the Makefile
EXEC = stack_ex2
# Begin generic part of the Makefile
CC = gcc
CFLAGS = -std=c99 -Werror -Wextra -Wall -pedantic
LDFLAGS =
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
ECHO = @

ifeq ($(VERBOSE),1)
ECHO=

endif

ifeq ($(DEBUG),yes)
CFLAGS += -g
LDFLAGS =
else
CFLAGS += -O3 -DNDEBUG
LDFLAGS =
endif

all:
ifeq ($(DEBUG),yes)
	@echo "Generating in debug mode"
else
	@echo "Generating in release mode"
endif
	@$(MAKE) $(EXEC)

%.o: %.c
	$(ECHO)$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(ECHO)$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean mrproper

clean:
	$(ECHO)rm -rf *.o
mrproper: clean
	$(ECHO)rm -rf $(EXEC)

doc: stack.h main.c
	$(ECHO)doxygen documentation/TP1

# End generic part of the makefile
# List of target dependencies
stack_ex2.o: stack.h
main.o: stack.h