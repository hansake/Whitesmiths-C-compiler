#
#	Makefile for Whitesmiths C preprocessor
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
CC = cx80 -v -prefix "" -proto $(BDIR)/proto/$(CPROTO) -I$(BDIR)/hdrs -L$(BDIR)/lib/

VPATH = $(BDIR)/pp

# Commands
CMDS = pp.com

# Manual pages
MANS = pp.man

# Internal documentation
DOCS = p0eval.doc p0.doc p0lex.doc p0main.doc p0subs.doc

%.o: %.c
	$(CC) +o $<

%.man: doc/%.mp
	roff $< > $@

%.doc: doc/%.ic
	roff $< > $@

.PHONY: $(TARGET) clean

$(TARGET): $(CMDS) $(MANS) $(DOCS)

PPOBJ = p0eval.o p0lex.o p0main.o p0subs.o

pp.com:	$(PPOBJ)
	$(CC) -o $@ $(PPOBJ)

clean:
	rm -f $(PPOBJ) $(CMDS) $(MANS) $(DOCS)

$(DTARGET): $(CMDS) $(MANS)
	@cp $(CMDS) ../../distr/bin
ifdef MANS
	@cp $(MANS) ../../distr/man
endif
