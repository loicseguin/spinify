# Spinify

Version: 2.4dev

http://bitbucket.org/loicseguin/spinify/
Copyright (C) 2009-2015 Loïc Séguin-C. <lsc@loicseguin.com>

Spinify is a Markov chain Monte Carlo simulation for the 2D Ising model
of ferromagnetism on curved surfaces. It is written in C++ and
distributed under a BSD license (see LICENSE for more information).

See INSTALL for detailed installation instructions.


## Description

The Spinify project aims at producing an efficient simulation algorithm
for the Ising model on surfaces with non trivial metric tensor.

The idea behind the project is that conformal field theories make
interesting predictions on the behaviour of the Ising model on
different surfaces in the limit where the number of atoms on the
lattice tends to infinity. However, there are no proofs that the
discrete model tends to the continuous model considered by CFT.

The problem with surfaces with non trivial metrics is that they often
do not admit regular lattices. To circumvent this difficulty, it has
been proposed to make simulations on random lattices.

More details available in the document
http://www.math.mcgill.ca/lseguin/documents/Ising2006.pdf.
Please note that this document refers to an older version of Spinify.
I will try to write a complete and more up to date explanation of the
background as well as the program as soon as possible.


## Mailing list

If you have questions, comments or just want to discuss about the
development of Spinify, you can do so on the mailing list:

spinify-discuss@googlegroups.com

You can subscribe to the mailing list here:

http://groups.google.ca:80/group/spinify-discuss?pli=1


## Bug reporting

Please report any bug to the issue tracker at
http://bitbucket.org/loicseguin/spinify/issues/


## Feedback

If you use this software and find it useful, I would like to hear from
you. Please send me an email at <lsc@loicseguin.com> and tell me how
you use the software.
