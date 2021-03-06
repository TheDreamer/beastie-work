# Created by: Gustau Perez i Querol <gustau.perez@gmail.com>
# $FreeBSD: head/x11-fm/sushi/Makefile 372768 2014-11-19 11:49:04Z kwm $
#    $MCom: ports/trunk/x11-fm/sushi/Makefile 19578 2014-04-25 21:50:30Z kwm $

PORTNAME=	sushi
PORTVERSION=	3.12.0
CATEGORIES=	x11-fm gnome
MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Tool similar to Gloobus Preview that provides quick file previews

LIB_DEPENDS=	libgcr-3.so:${PORTSDIR}/security/gcr \
		libgjs.so:${PORTSDIR}/lang/gjs \
		libclutter-gtk-1.0.so:${PORTSDIR}/graphics/clutter-gtk3 \
		libclutter-gst-2.0.so:${PORTSDIR}/multimedia/clutter-gst \
		libwebkitgtk-3.0.so:${PORTSDIR}/www/webkit-gtk3 \
		libmusicbrainz5.so:${PORTSDIR}/audio/libmusicbrainz5 \
		libevdocument3.so:${PORTSDIR}/graphics/evince
RUN_DEPENDS=	nautilus:${PORTSDIR}/x11-fm/nautilus

USES=		gettext gmake libtool pathfix pkgconfig tar:xz
INSTALLS_ICONS=	yes
USE_GNOME=	gnomeprefix gtksourceview3 intlhack introspection:build
USE_LDCONFIG=	yes
GNU_CONFIGURE=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib
INSTALL_TARGET=	install-strip

.include <bsd.port.pre.mk>

# because webkit-gtk3 needs it
.if ${OSVERSION} < 1000000
USES+= compiler:c++11-lib
MAKE_ENV+=	LD_LIBRARY_PATH=${_GCC_RUNTIME}
.endif

.include <bsd.port.mk>
