SRC_DIR = ../src

all: graphtest simultest swendsentest recttest randtest

graphtest: graphtest.o $(SRC_DIR)/Graph.o $(SRC_DIR)/tezuka.o
	$(CXX) -o $@ $^

simultest: simultest.o $(SRC_DIR)/Simul.o $(SRC_DIR)/Graph.o $(SRC_DIR)/tezuka.o
	$(CXX) -o $@ $^

swendsentest: swendsentest.o $(SRC_DIR)/Simul.o $(SRC_DIR)/Graph.o $(SRC_DIR)/tezuka.o
	$(CXX) -o $@ $^

recttest: recttest.o $(SRC_DIR)/Graph.o $(SRC_DIR)/tezuka.o
	$(CXX) -o $@ $^

randtest: randtest.o $(SRC_DIR)/tezuka.o
	$(CXX) -o $@ $^

%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(SRC_DIR)/Graph.o: $(SRC_DIR)/Graph.h

$(SRC_DIR)/Simul.o: $(SRC_DIR)/Simul.h

$(SRC_DIR)/tezuka.o: $(SRC_DIR)/tezuka.h

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	cd $(SRC_DIR)/ && $(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean

clean:
	@rm -rf *.o
