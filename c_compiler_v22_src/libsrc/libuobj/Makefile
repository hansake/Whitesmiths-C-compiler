#
#       Makefile for Whitesmiths libu.80 library
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


LIBTARGET = libu.80

# Cross compiler
CC = cx80 -prefix "" -proto $(BDIR)/proto/$(CPROTO) -I$(BDIR)/hdrs

VPATH = $(BDIR)/libsrc/v7:$(BDIR)/libsrc/cio:$(BDIR)/libsrc/cfloat:$(BDIR)/libsrc/clib:$(BDIR)/libsrc/cpm:$(BDIR)/80/run

UOBJ = atoi.o atol.o calloc.o fclos7.o fflush.o fgets.o fopen7.o fputs.o\
       fread7.o fseek.o fwrite.o gets.o log10.o lsek7.o pow.o printf.o\
       puts.o qsort.o rand.o remark.o rindex.o scanf.o sprint.o strcmp.o\
       strncat.o strncm.o strncpy.o tolower.o toupper.o ungetc.o atof.o ctype.o\
       error.o exit7.o fgetc.o fprint.o fputc.o index.o stdin.o feof.o\
       fmtcvt.o

$(TARGET): $(LIBTARGET)

%.o: %.c
	$(CC) +o $<

%.o: %.s
	$(CC) +o $<

libu.80: $(UOBJ)
	rel80 $(UOBJ) | lord80 -c"lby $@ -c" | sh
	cp $@ $(BDIR)/lib

clean:
	rm -f *.o *.80

$(DTARGET): $(LIBTARGET)
	@cp $(LIBTARGET) $(BDIR)/$(TARGET)/distr/lib
