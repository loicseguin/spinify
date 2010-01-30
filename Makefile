export CXX = g++
export CFLAGS = -Wall -O3
export CXXFLAGS = $(CFLAGS)

spinify:
	cd src && $(MAKE) all
	cd contrib/qhull-2010.1/src && $(MAKE)

all: spinify tests

tests:
	cd test && $(MAKE) all


.PHONY: clean cleanall

cleanall:
	rm spinify
	cd src && $(MAKE) cleanall
	cd test && $(MAKE) cleanall

clean:
	cd src && $(MAKE) clean
	cd test && $(MAKE) clean
