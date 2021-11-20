#
# Build a Whitesmiths C Cross Compiler Edition 2.2
#

# BDIR is the base directory from which the top level make is invoked
export BDIR != pwd
# CCPROTO is the prototype file to use for compiling
export CPROTO = ccpm80.pro

#
# Build the Whitesmiths C compiler for Intel 8080 running under CP/M
#
# TARGET defines the name of the package to be built
TARGET = ccpm80
# DTARGET is the distribution target
export DTARGET = distr

# Which code to use for building this compiler
CCODE = libsrc/libcobj libsrc/libuobj 80/asm pp p1 80/p2 link util

# Where the distribution files ends up
DISTR = bin hdrs lib man test

.PHONY: $(TARGET) mkdistr clean distclean

$(TARGET): proto/$(CPROTO)
	@echo Building $@
	@for dir in $(CCODE); do \
	mkdir -p $@/build/$$dir; \
	cp $$dir/Makefile $@/build/$$dir; \
	$(MAKE) -C $@/build/$$dir $(TARGET) \
	|| exit 1; done;

$(DTARGET): $(TARGET)
	@echo Making distribution
	@for dir in $(DISTR); do \
	mkdir -p $(TARGET)/distr/$$dir \
	|| exit 1; done;
	@for dir in $(CCODE); do \
	$(MAKE) -C $(TARGET)/build/$$dir $(DTARGET) \
	|| exit 1; done;
	@cp $(BDIR)/hdrs/*.h $(TARGET)/distr/hdrs/

clean:
	rm -fr $(TARGET)/build

distclean:
	rm -fr $(TARGET)
