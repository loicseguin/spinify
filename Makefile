SHELL = /bin/sh
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
datarootdir = $(prefix)/share
mandir = $(datarootdir)/man
man1dir = $(mandir)/man1
INSTALL = install -v
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

export CXX = g++
export CFLAGS = -Wall -g -O3
export CXXFLAGS = $(CFLAGS)

.SUFFIXES:
.SUFFIXES: .cpp .o

all: spinify

spinify:
	cd src && $(MAKE) all

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
	@$(INSTALL_PROGRAM) src/spinify $(DESTDIR)$(bindir)/spinify
	@$(INSTALL_DATA) doc/spinify.1 $(DESTDIR)$(man1dir)/spinify.1

uninstall:
	rm $(DESTDIR)$(bindir)/spinify
	rm $(DESTDIR)$(mandir)/spinify.1

