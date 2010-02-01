BINDIR = /usr/local/bin
MANDIR = /usr/local/share/man/man1

export CXX = g++
export CFLAGS = -Wall -O3
export CXXFLAGS = $(CFLAGS)

spinify:
	cd src && $(MAKE) all
	cd contrib/qhull-2010.1/src && $(MAKE)

all: spinify tests

tests:
	cd test && $(MAKE) all

install:
	cp src/spinify $(BINDIR)/spinify
	cp doc/spinify.1 $(MANDIR)/spinify.1
	cp contrib/qhull-2010.1/qdelaunay $(BINDIR)/qdelaunay

.PHONY: clean cleanall

cleanall:
	cd src && $(MAKE) cleanall
	cd test && $(MAKE) cleanall
	cd contrib/qhull-2010.1/src && $(MAKE) cleanall

clean:
	cd src && $(MAKE) clean
	cd test && $(MAKE) clean
