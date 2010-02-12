# Unix Makefile for qhull and rbox
#
#       see README.txt
#       the Unix distribution contains a configure Makefile
#       
#       make           to produce qhull qconvex qdelaunay qhalf qvoronoi rbox
#       make qdelaunay to produce qvoronoi (etc.)
#       make qhullx    to produce qhull qconvex etc.  w/o using libqhull.a
#       make install   to copy qhull, rbox, qhull.1, rbox.1 to BINDIR, MANDIR
#       make new       to rebuild qhull and rbox from source
#
#       make clean     to remove object files and core
#       make cleanall  to remove all generated files
#
#       BINDIR --      directory where to copy executables
#       MANDIR --      directory where to copy manual pages
#       CC --          ANSI C or C++ compiler
#       CCOPTS1 -      options used to compile .c files
#       CCOPTS2 --     options used to link .o files
#
#       CFILES --      .c files for printing
#       HFILES --      .h files for printing
#       DFILES --       documentation files
#       MFILES --      man pages and html files
#       TFILES --      .txt versions of html html files
#       FILES --       all other files
#       OBJS --        specifies the object files of libqhull.a
#
# Do not replace tabs with spaces.  Needed by 'make' for build rules

BINDIR	= /usr/local/bin
MANDIR	= /usr/local/share/man/man1

#for Gnu's gcc compiler -O2 for optimization, -g for debugging, -Wall for check
#   -fno-strict-aliasing not needed for gcc 4.4+ (segfault in qset.c)
CC  = gcc
CCOPTS1 = -O2 -ansi -fno-strict-aliasing 

# for loader, ld
CCOPTS2 = $(CCOPTS1)

# OBJS ordered by frequency of execution with small files at end.  Better locality.
OBJS = user.o global.o stat.o io.o geom2.o poly2.o merge.o \
       libqhull.o geom.o poly.o qset.o mem.o usermem.o userprintf.o random.o

# CFILES ordered alphabetically after libqhull.c 
HFILES= user.h libqhull.h qhull_a.h geom.h io.h mem.h merge.h poly.h random.h \
		qset.h stat.h

all: qdelaunay

qdelaun.o:  libqhull.h user.h mem.h
libqhull.o: $(HFILES)
geom.o:	    $(HFILES)
geom2.o:    $(HFILES)
global.o:   $(HFILES)
io.o:	    $(HFILES)
mem.o:	    mem.h 
merge.o:    $(HFILES)
poly.o:	    $(HFILES)
poly2.o:    $(HFILES)
random.o:   libqhull.h random.h
qset.o:	    qset.h mem.h 
stat.o:	    $(HFILES)
user.o:	    $(HFILES)

.c.o:
	$(CC) -c $(CCOPTS1) $<

clean:
	rm -f *.o qdelaunay

cleanall: clean
	rm -f *~ qdelaunay ../*.exe >/dev/null

install:  
	cp qdelaunay $(BINDIR)/qdelaunay
	cp ../qhull.man $(MANDIR)/qhull.1

qdelaunay: qdelaun.o $(OBJS)
	$(CC) -o qdelaunay $(CCOPTS2) qdelaun.o $(OBJS) -lm 

