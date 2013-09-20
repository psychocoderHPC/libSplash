#!/bin/bash
#
# update the doxygen docu with branch in $1

branch=${1:-"master"}
myDir=`pwd`

# tmp dir with auto-delete if something goes wrong
TMPDIR=`mktemp -d`
trap "{ cd - ; rm -rf $TMPDIR; exit 255; }" SIGINT

# create new dokumentation
cd $TMPDIR
git clone https://github.com/ComputationalRadiationPhysics/libSplash.git
cd libSplash
git checkout $branch
cd doc
doxygen

# update old documentation
cd $myDir
rsync -r --delete --filter='P update.sh' --filter='P .git' $TMPDIR/libSplash/doc/doxygenDoku/html/ .

rm -rf $TDIR
exit 0
