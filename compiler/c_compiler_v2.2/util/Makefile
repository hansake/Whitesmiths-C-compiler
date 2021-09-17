#
#	Makefile for Whitesmiths utilities
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

VPATH = $(BDIR)/util

# Commands
CMDS = hex.com lib.com lord.com rel.com

# Manual pages
MANS = rel.man

# Internal documentation
DOCS =

%.o: %.c
	$(CC) +o $<

%.man: doc/%.mp
	roff $< > $@

%.doc: doc/%.ic
	roff $< > $@

.PHONY: $(TARGET) clean

$(TARGET): $(CMDS) $(MANS) $(DOCS)

HEXOBJ = hex.o

hex.com: $(HEXOBJ)
	$(CC) -o $@ $(HEXOBJ)

LIBOBJ = lib.o

lib.com: $(LIBOBJ)
	$(CC) -o $@ $(LIBOBJ)

LORDOBJ = lord.o

lord.com: $(LORDOBJ)
	$(CC) -o $@ $(LORDOBJ)

RELOBJ = rel.o

rel.com: $(RELOBJ)
	$(CC) -o $@ $(RELOBJ)

clean:
	rm -f $(CMDS) $(MANS) $(DOCS) $(HEXOBJ) $(LIBOBJ) $(LORDOBJ) $(RELOBJ)

$(DTARGET): $(CMDS) $(MANS)
	@cp $(CMDS) ../../distr/bin
ifdef MANS
	@cp $(MANS) ../../distr/man
endif
