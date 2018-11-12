# Copyright 1999-2003 Gentoo Technologies, Inc.
# Distributed under the terms of the GNU General Public License v2
# $Header: /sources/libcdio/libcdio/misc/libcdio.ebuild,v 1.1 2003/04/29 08:17:59 rocky Exp $

IUSE=""

DESCRIPTION="A library to encapsulate CD-ROM reading and control."
HOMEPAGE="http://savannah.nongnu.org/projects/libcdio/"
SRC_URI="http://savannah.nongnu.org/download/${PN}/${PN}.pkg/${PV}/${P}.tar.gz"
LICENSE="GPL"

SLOT="0"
KEYWORDS="x86"


src_compile() {
        econf || die
        emake || die
}

src_install() {
        make DESTDIR=${D} install
        dodoc AUTHORS COPYING Changelog INSTALL NEWS README THANKS
}
