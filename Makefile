BINDIR = /usr/local/bin
MANDIR = /usr/local/share/man/man1

export CXX = g++
export CFLAGS = -Wall -O3
export CXXFLAGS = $(CFLAGS)

spinify:
	cd src && $(MAKE) all
	cd contrib/qhull-2010.1/src && $(MAKE) qdelaunay

all: spinify tests

tests:
	cd test && $(MAKE) all

.PHONY: clean cleanall install uninstall

cleanall:
	cd src && $(MAKE) cleanall
	cd test && $(MAKE) cleanall
	cd contrib/qhull-2010.1/src && $(MAKE) cleanall

clean:
	cd src && $(MAKE) clean
	cd test && $(MAKE) clean

install:
	@install -v src/spinify $(BINDIR)/spinify
	@install -v doc/spinify.1 $(MANDIR)/spinify.1
	@install -v contrib/qhull-2010.1/src/qdelaunay $(BINDIR)/qdelaunay
	@install -v contrib/qhull-2010.1/qhull.man $(MANDIR)/qhull.1

uninstall:
	rm $(BINDIR)/spinify
	rm $(MANDIR)/spinify.1
	rm $(BINDIR)/qdelaunay
	rm $(MANDIR)/qhull.1


