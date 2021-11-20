#
# Makefile for Whitesmiths C compiler phase 1
# p1.com: handles processing between pp and p2
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

VPATH = $(BDIR)/p1

# Commands
CMDS = p1.com

# Manual pages
MANS = p1.man

# Internal documentation
DOCS = p1data.doc p1expr.doc p1func.doc p1gets.doc p1.doc p1main.doc p1puts.doc p1red.doc p1subs.doc p1type.doc

%.o: %.c
	$(CC) +o $<

%.man: doc/%.mp
	roff $< > $@

%.doc: doc/%.ic
	roff $< > $@

.PHONY: $(TARGET) clean

$(TARGET): $(CMDS) $(MANS) $(DOCS)

P1OBJ = p1data.o p1expr.o p1func.o p1gets.o p1main.o p1puts.o p1red.o p1subs.o p1type.o

p1.com:	$(P1OBJ)
	$(CC) -o $@ $(P1OBJ)

clean:
	rm -f $(CMDS) $(MANS) $(DOCS) $(P1OBJ)

$(DTARGET): $(CMDS) $(MANS)
	@cp $(CMDS) $(BDIR)/$(TARGET)/distr/bin
ifdef MANS
	@cp $(MANS) $(BDIR)/$(TARGET)/distr/man
endif
