SRC_DIR = ../src

all: graphtest simultest swendsentest recttest randtest spheretest point3dtest planedelaunaytest

graphtest: graphtest.o $(SRC_DIR)/Graph.o $(SRC_DIR)/Point3D.o $(SRC_DIR)/tezuka.o
	$(CXX) -o $@ $^

simultest: simultest.o $(SRC_DIR)/Simul.o $(SRC_DIR)/Graph.o $(SRC_DIR)/Point3D.o $(SRC_DIR)/tezuka.o
	$(CXX) -o $@ $^

swendsentest: swendsentest.o $(SRC_DIR)/Simul.o $(SRC_DIR)/Graph.o $(SRC_DIR)/Point3D.o $(SRC_DIR)/tezuka.o
	$(CXX) -o $@ $^

recttest: recttest.o $(SRC_DIR)/Graph.o $(SRC_DIR)/Point3D.o $(SRC_DIR)/tezuka.o
	$(CXX) -o $@ $^

randtest: randtest.o $(SRC_DIR)/tezuka.o
	$(CXX) -o $@ $^

spheretest: spheretest.o $(SRC_DIR)/Graph.o $(SRC_DIR)/Point3D.o $(SRC_DIR)/Surface.o $(SRC_DIR)/tezuka.o
	$(CXX) -o $@ $^

point3dtest: point3dtest.o $(SRC_DIR)/Point3D.o
	$(CXX) -o $@ $^

planedelaunaytest: planedelaunaytest.o $(SRC_DIR)/Graph.o $(SRC_DIR)/Point3D.o $(SRC_DIR)/Surface.o $(SRC_DIR)/tezuka.o
	$(CXX) -o $@ $^

%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(SRC_DIR)/Graph.o: $(SRC_DIR)/Graph.h

$(SRC_DIR)/Point3D.o: $(SRC_DIR)/Point3D.h

$(SRC_DIR)/Simul.o: $(SRC_DIR)/Simul.h

$(SRC_DIR)/tezuka.o: $(SRC_DIR)/tezuka.h

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	cd $(SRC_DIR)/ && $(MAKE) $@

.PHONY: cleanall clean

cleanall: clean
	@rm -rf graphtest simultest swendsentest recttest randtest spheretest point3dtest

clean:
	@rm -rf *.o
