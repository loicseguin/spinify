SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

all: spinify

spinify: $(OBJ)
	$(CXX) -o $@ $^

main.o: Graph.h Simul.h tezuka.h

Graph.o: Graph.h

Simul.o: Simul.h

tezuka.o: tezuka.h

%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean

clean:
	@rm -rf *.o
