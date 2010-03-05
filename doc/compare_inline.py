#!/usr/bin/env python

"""
This file compares the running time of two versions of the Spinify program.
The first version is version 2.2; it is referred to as without_inline. The
other version is a modified version 2.2 (revision ) in which many functions
have been replaced by inline functions to improve efficiency. Each version
was run 20 times with the default configuration file spinifyrc.sample. Thus,
the program had to generate an even spherical graph of 1000 nodes and make
measurements of energy, magnetization and susceptibility. The running times
are noted below.

It turns out that the version with inline functions is about 23% faster then
the version without inline functions. This is a significant improvement.

"""

with_inline = ['1m37.654', '2m14.077', '3m14.801', '1m56.694', '1m55.114',
               '2m11.498', '1m58.838', '2m10.646', '2m30.520', '2m1.782',
               '2m25.293', '1m59.586', '2m21.288', '2m4.458', '1m58.497',
               '2m28.985', '2m17.112', '2m2.197', '2m5.987', '2m5.487']
without_inline = ['3m8.617', '2m36.846', '2m37.688', '1m57.824', '2m38.811',
               '2m36.572', '3m15.122', '3m17.996', '2m35.566', '3m33.937',
               '2m36.923', '2m33.372', '3m17.096', '2m41.795', '3m13.178',
               '2m36.658', '2m37.677', '2m38.450', '2m34.791', '3m34.481']

import math

def to_seconds(minsecs):
    min, sec = minsecs.split('m')
    nb_secs = int(min)*60 + float(sec)
    return nb_secs

def avg(V):
    sum = 0.
    for i in V:
        sum += i
    return sum/len(V)

def stddev(V):
    avgV = avg(V)
    sum = 0.
    for i in V:
        sum += (i - avgV)**2
    return math.sqrt(sum / len(V))

nwith_inline = [to_seconds(i) for i in with_inline]
nwithout_inline = [to_seconds(i) for i in without_inline]

print "With inline functions: ", avg(nwith_inline), "\pm", stddev(nwith_inline)
print "Without inline functions: ", avg(nwithout_inline), "\pm", stddev(nwithout_inline)

