.DEFAULT_GOAL := brainfuck # make

CFLAGS = -Wall -Wextra -pedantic

SRCDIR = src
INCDIR = include
BUILDDIR = build
EXE = brainfuck

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	gcc $(CFLAGS) -I$(INCDIR) -c -o $@ $<

test: $(OBJS) tests/test.c
	gcc $(CFLAGS) -g -I$(INCDIR) -L. -o test $^

d: $(OBJS)
	gcc $(CFLAGS) -g -I$(INCDIR) brainfuck.c -o $(EXE) $(OBJS)

brainfuck: $(OBJS)
	gcc $(CFLAGS) -I$(INCDIR) brainfuck.c -o $(EXE) $(OBJS)

clean:
	rm -rf $(BUILDDIR)/* test.exe test brainfuck.exe brainfuck
