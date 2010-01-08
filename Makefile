export CXX = g++
export CFLAGS = -Wall -g -O2
export CXXFLAGS = $(CFLAGS)

all: spinify tests

spinify:
	cd src && $(MAKE) all

tests:
	cd test && $(MAKE) all


.PHONY: clean

clean:
	cd src && $(MAKE) clean
	cd test && $(MAKE) clean

