SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

all: spinify

spinify: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

Graph: Graph.o Point3D.o tezuka.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

Maths: Maths.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

Point3D: Point3D.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

Simul: Simul.o Maths.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

Surface: Surface.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

tezuka: tezuka.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

main.o: Graph.h Maths.h Simul.h tezuka.h

Graph.o: Graph.h

Point3D.o: Point3D.h

Maths.o: Maths.h

Simul.o: Simul.h

tezuka.o: tezuka.h

Surface.o: Surface.h

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: cleanall clean

cleanall: clean
	@rm -rf spinify

clean:
	@rm -rf *.o
