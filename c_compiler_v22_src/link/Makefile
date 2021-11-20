#
#	Makefile for Whitesmiths linker
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

VPATH = $(BDIR)/link

# Commands
CMDS = link.com

# Manual pages
MANS =

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

LINKOBJ = link1.o link2.o

link.com: $(LINKOBJ)
	$(CC) -o $@ $(LINKOBJ)

clean:
	rm -f $(CMDS) $(MANS) $(DOCS) $(LINKOBJ)

$(DTARGET): $(CMDS) $(MANS)
	@cp $(CMDS) ../../distr/bin
ifdef MANS
	@cp $(MANS) ../../distr/man
endif
