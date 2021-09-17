#
#       Makefile for Whitesmiths libc.80 library
#

# If this Makefile is not invoked as a sub-make
ifndef TARGET
    TARGET = ccpm80
endif

ifndef BDIR
    BDIR = ../..
endif

ifndef CPROTO
    CPROTO = ccpm80.pro
endif

LIBTARGET = libc.80 crtscpm.80

# Cross compiler
CC = cx80 -prefix "" -proto $(BDIR)/proto/$(CPROTO) -I$(BDIR)/hdrs

VPATH = $(BDIR)/libsrc/cio:$(BDIR)/libsrc/cfloat:$(BDIR)/libsrc/clib:$(BDIR)/libsrc/cpm:$(BDIR)/80/run


COBJ = abs.o arctan.o btoi.o btou.o cfcan.o count.o crypt.o decode.o\
       encode.o enter.o entx.o errfmt.o exp.o fclose.o fcreat.o fill.o\
       fopen.o fread.o frelst.o ftd.o gbfile.o getch.o getf.o getfi.o\
       getfl.o getfmt.o getin.o getlin.o in.o inbuf.o itob.o land.o\
       lcom.o ln.o lower.o lstoi.o lstol.o lstou.o lsub.o ltols.o\
       lxor.o main.o mapchr.o match.o mkord.o notbuf.o notstr.o open.o\
       ordbuf.o out.o pat.o pathnm.o prefix.o putch.o putf.o putfmt.o\
       putlin.o remark.o remove.o round.o scnbuf.o sin.o sort.o sqr.o\
       sqrt.o sqze.o stdin.o stdout.o substr.o trunc.o uname.o usage.o\
       utob.o buybuf.o cffun.o cgetf.o cmpbuf.o cmpstr.o cputf.o cpybuf.o\
       cpystr.o create.o doesc.o dtf.o dtoe.o dtof.o finit.o getc.o\
       getl.o instr.o itols.o lor.o lseek.o ltob.o putc.o putl.o\
       putstr.o read.o readerr.o scnstr.o stob.o subbuf.o btod.o btol.o\
       btos.o cnorm.o copen.o cround.o dcmp.o dneg.o dsub.o dtento.o\
       dtl.o fioerr.o fwrite.o idiv.o ilsh.o imul.o ldiv.o lenstr.o\
       lmul.o lneg.o ltd.o writerr.o cfcon.o close.o dadd.o ddiv.o\
       dmul.o dtd.o repk.o unpk.o alloc.o memerr.o raise.o sbreak.o\
       stop.o error.o pname.o write.o ckfd.o onintr.o wblk.o wcbs.o\
       exit.o ladd.o lclt.o lcmp.o lcpy.o llsh.o switch.o zret.o\
       ent.o ents.o

$(TARGET): $(LIBTARGET)

%.o: %.c
	$(CC) +o $<

%.o: %.s
	$(CC) +o $<

crtscpm.80: $(BDIR)/libsrc/cpm/crts.s
	$(CC) +o $<
	mv crts.o $@
	cp $@ $(BDIR)/lib

libc.80: $(COBJ)
	rel80 $(COBJ) | lord80 -c"lby $@ -c" | sh
	cp $@ $(BDIR)/lib

clean:
	rm -f *.o *.80

$(DTARGET): $(LIBTARGET)
	@cp $(LIBTARGET) $(BDIR)/$(TARGET)/distr/lib
