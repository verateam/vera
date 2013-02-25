export VERA_VERSION=1.1.1

find . -name '*~' | xargs rm
make clean

export DISTRO=vera++-$VERA_VERSION

rm -f $DISTRO.tar.gz

mkdir $DISTRO
cp -R doc/ $DISTRO/doc
cp -R profiles/ $DISTRO/profiles
cp -R scripts/ $DISTRO/scripts
cp -R src/ $DISTRO/src
cp CHANGES LICENSE_1_0.txt Makefile $DISTRO

tar czf $DISTRO.tar.gz $DISTRO
rm -rf $DISTRO
