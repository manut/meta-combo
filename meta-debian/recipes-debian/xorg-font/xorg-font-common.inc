HOMEPAGE = "http://www.x.org"
BUGTRACKER = "https://bugs.freedesktop.org/enter_bug.cgi?product=xorg"

INC_PR = "r0"

inherit debian-package

LICENSE = "MIT-X"

DEPENDS = " encodings font-alias font-util-native mkfontdir-native mkfontscale-native"
RDEPENDS_${PN} = "encodings font-util font-alias"

inherit autotools pkgconfig

EXTRA_OEMAKE += "FCCACHE=/bin/true"

do_configure_prepend() {
        if [ -f "${S}"/configure.ac ] ; then
                sed -i "s#^MAPFILES_PATH=.*#MAPFILES_PATH=\"${STAGING_DIR_TARGET}/\$(pkg-config --variable=mapdir fontutil)\"#g" "${S}"/configure.ac
        fi
}

do_install_append() {
	find ${D}${libdir}/X11/fonts -type f -name fonts.dir | xargs rm -f
	find ${D}${libdir}/X11/fonts -type f -name fonts.scale | xargs rm -f
	find ${D}${datadir}/fonts/X11 -type f -name fonts.dir | xargs rm -f
	find ${D}${datadir}/fonts/X11 -type f -name fonts.scale | xargs rm -f	
}

FILES_${PN} += " ${libdir}/X11/fonts ${datadir}"

pkg_postinst_${PN} () {
        set -x
        for fontdir in `find $D/usr/lib/X11/fonts -type d`; do
                mkfontdir $fontdir
                mkfontscale $fontdir
        done
        for fontdir in `find $D/usr/share/fonts/X11 -type d`; do
                mkfontdir $fontdir
                mkfontscale $fontdir
        done
}
