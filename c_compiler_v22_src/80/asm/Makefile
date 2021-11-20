#
# Makefile for Whitesmiths C compiler 8080 assembler and tools
# as80.com: the assembler handles 8080 assembler code with anat syntax
# dis80.com: disassembles Whitesmiths object code
# anat.com: translates anat syntax to strandard 8080 assembler code
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

VPATH = $(BDIR)/80/asm

# Commands
CMDS = anat.com as80.com dis80.com

# Manual pages
MANS = anat.man as80.man dis80.man

# Internal documentation
DOCS = as80.doc

%.o: %.c
	$(CC) +o $<

%.man: doc/%.mp
	roff $< > $@

%.doc: doc/%.ip
	roff $< > $@

.PHONY: $(TARGET) clean

$(TARGET): $(CMDS) $(MANS) $(DOCS)

ANATOBJ = tab.o tmain.o tmake.o trn.o

anat.com : $(ANATOBJ)
	$(CC) -o $@ $(ANATOBJ)

AS80OBJ = amain.o amake.o asm.o auxx.o

as80.com: $(AS80OBJ)
	$(CC) -o $@ $(AS80OBJ)

DIS80OBJ = dis80.o

dis80.com: $(DIS80OBJ)
	$(CC) -o $@ $(DIS80OBJ)

clean:
	rm -f $(CMDS) $(MANS) $(DOCS) $(ANATOBJ) $(AS80OBJ) $(DIS80OBJ)

$(DTARGET): $(CMDS) $(MANS)
	@cp $(CMDS) $(BDIR)/$(TARGET)/distr/bin
ifdef MANS
	@cp $(MANS) $(BDIR)/$(TARGET)/distr/man
endif

