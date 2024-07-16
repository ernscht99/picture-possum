#!/bin/bash
topdir=picture-possum_$1-$2_amd64
rm -r $topdir
mkdir -p $topdir
mkdir -p $topdir/usr/bin
mkdir -p $topdir/DEBIAN
cp build/src/picture_possum $topdir/usr/bin/picture-possum
echo "Package: picture-possum
Version: $1
Maintainer: Felix Christ <felix.christ@posteo.de>
Depends: openssl, libqt6widgets6t64
Architecture: amd64
Homepage: https://github.com/ernscht99/picture_possum
Description: A program for manually, but efficiently, tagging and sorting large numbers of images" \
	> $topdir/DEBIAN/control
dpkg --build $topdir

