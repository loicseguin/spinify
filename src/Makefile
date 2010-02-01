SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

all: spinify

spinify: $(OBJ)
	$(CXX) -o $@ $^

Graph: Graph.o Point3D.o
	$(CXX) -o $@ $^

Point3D: Point3D.o
	$(CXX) -o $@ $^

Output: Output.o
	$(CXX) -o $@ $^

Simul: Simul.o
	$(CXX) -o $@ $^

Surface: Surface.o
	$(CXX) -o $@ $^

tezuka: tezuka.o
	$(CXX) -o $@ $^

main.o: Graph.h Output.h Simul.h tezuka.h

Graph.o: Graph.h

Output.o: Output.h

Point3D.o: Point3D.h

Simul.o: Simul.h

tezuka.o: tezuka.h

Surface.o: Surface.h

%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: cleanall clean

cleanall: clean
	@rm -rf spinify

clean:
	@rm -rf *.o
