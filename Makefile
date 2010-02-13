PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man/man1

export CXX = g++
export CFLAGS = -Wall -O3
export CXXFLAGS = $(CFLAGS)

spinify:
	cd src && $(MAKE) all

all: spinify tests

tests:
	cd test && $(MAKE) all

.PHONY: clean cleanall install uninstall

cleanall:
	cd src && $(MAKE) cleanall
	cd test && $(MAKE) cleanall

clean:
	cd src && $(MAKE) clean
	cd test && $(MAKE) clean

install:
	@install -v src/spinify $(BINDIR)/spinify
	@install -v doc/spinify.1 $(MANDIR)/spinify.1

uninstall:
	rm $(BINDIR)/spinify
	rm $(MANDIR)/spinify.1

