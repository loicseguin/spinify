#!/bin/sh

VERSION=2.0
TMPDIR="spinify-$VERSION"
FOLDERS="src doc contrib"
FILES="LICENSE Makefile README"

mkdir -p $TMPDIR/test

for f in $FOLDERS; do
    cp -R $f $TMPDIR/
done

for f in $FILES; do
    cp $f $TMPDIR/
done

for f in test/*.cpp; do
    cp $f $TMPDIR/test/
done

cp test/Makefile $TMPDIR/test/

