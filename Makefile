export CXX = g++
export CFLAGS = -Wall -O3
export CXXFLAGS = $(CFLAGS)

all: spinify tests

spinify:
	cd src && $(MAKE) all
	cd contrib/qhull-2010.1/src && $(MAKE)

tests:
	cd test && $(MAKE) all


.PHONY: clean cleanall

cleanall:
	cd src && $(MAKE) cleanall
	cd test && $(MAKE) cleanall

clean:
	cd src && $(MAKE) clean
	cd test && $(MAKE) clean
