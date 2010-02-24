confparser_sources = ../src/ConfParser.cpp ../src/ConfParser.h
graph_sources = ../src/Graph.cpp ../src/Graph.h
maths_sources = ../src/Maths.cpp ../src/Maths.h
point3d_sources = ../src/Point3D.cpp ../src/Point3D.h
simul_sources = ../src/Simul.cpp ../src/Simul.h
surface_sources = ../src/Surface.cpp ../src/Surface.h
tezuka_sources = ../src/tezuka.cpp ../src/tezuka.h


noinst_PROGRAMS = graphtest parsertest planedelaunaytest point3dtest \
			   randtest recttest spheretest swendsentest

graphtest_SOURCES = graphtest.cpp \
					$(graph_sources) \
					$(point3d_sources) \
					$(tezuka_sources)

parsertest_SOURCES = parsertest.cpp \
					 $(confparser_sources)

planedelaunaytest_SOURCES = planedelaunaytest.cpp \
					$(graph_sources) \
					$(point3d_sources) \
					$(surface_sources) \
					$(tezuka_sources)

point3dtest_SOURCES = point3dtest.cpp \
					$(point3d_sources)

randtest_SOURCES = randtest.cpp \
					$(tezuka_sources)

recttest_SOURCES = recttest.cpp \
					$(graph_sources) \
					$(point3d_sources) \
					$(tezuka_sources)

spheretest_SOURCES = spheretest.cpp \
					$(graph_sources) \
					$(point3d_sources) \
					$(surface_sources) \
					$(tezuka_sources)

swendsentest_SOURCES = swendsentest.cpp \
					$(graph_sources) \
					$(maths_sources) \
					$(point3d_sources) \
					$(simul_sources) \
					$(surface_sources) \
					$(tezuka_sources)

