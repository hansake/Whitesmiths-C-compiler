#
# Makefile for Whitesmiths C compiler phase 2, 8080 code generator
# p280.com: gets its input from p1 and generates assembler code
#

# If this Makefile is not invoked as a sub-make
ifndef TARGET
    TARGET = ccpm80
endif

ifndef BDIR
    BDIR = ..
endif

ifndef CPROTO
    CPROTO = ccpm80.pro
endif

# Cross compiler
CC = cx80 -prefix "" -proto $(BDIR)/proto/$(CPROTO) -I$(BDIR)/hdrs -L$(BDIR)/lib/

VPATH = $(BDIR)/80/p2

# Commands
CMDS = p280.com

# Manual pages
MANS = 

# Internal documentation
DOCS = int012.doc int12.doc int2.doc p2expr.doc p2fmov.doc \
       p2gen.doc p2.doc p2jump.doc p2main.doc p2subs.doc p2tab.doc

%.o: %.c
	$(CC) +o $<

%.man: doc/%.mp
	roff $< > $@

%.doc: doc/%.ic
	roff $< > $@

.PHONY: $(TARGET) clean

$(TARGET): $(CMDS) $(MANS) $(DOCS)

P2OBJ = p2expr.o p2fmov.o p2gen.o p2jump.o p2main.o p2subs.o p2tab.o

p280.com: $(P2OBJ)
	$(CC) -o $@ $(P2OBJ)

clean:
	rm -f $(CMDS) $(MANS) $(DOCS) $(P2OBJ)

$(DTARGET): $(CMDS) $(MANS)
	@cp $(CMDS) $(BDIR)/$(TARGET)/distr/bin
ifdef MANS
	@cp $(MANS) $(BDIR)/$(TARGET)/distr/man
endif
