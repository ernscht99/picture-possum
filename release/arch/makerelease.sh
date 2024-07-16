#!/bin/bash
url=https://github.com/ernscht99/picture_possum/archive/refs/tags/v$1.tar.gz
wget $url
shasum=$(sha256sum v$1.tar.gz| awk "{print \$1}")
rm v$1.tar.gz
echo "# Maintainer: Felix Christ <felix dot christ (at) posteo dot de>

pkgname=picture-possum
pkgver=$1
pkgrel=1
pkgdesc='Manually, but efficiently, tag and sort large numbers of images'
arch=('x86_64')
url='https://github.com/ernscht99/picture_possum'
license=('GPL2')
depends=('openssl' 'qt6-base')
makedepends=('cmake')
source=(https://github.com/ernscht99/$"{pkgname}"/archive/refs/tags/v$"{pkgver}".tar.gz)
sha256sums=('$shasum')

build() {
  cd $"{srcdir}"/$"{pkgname}"-$"{pkgver}"
  cmake .
  make
}

package() {
  cd $"{srcdir}"/$"{pkgname}"-$"{pkgver}"
  make DESTDIR=$"{pkgdir}" install
}" > PKGBUILD 
