# Created by: Andrey Chernov <ache@FreeBSD.org>
# $FreeBSD: head/www/lynx/Makefile 363710 2014-08-01 14:03:19Z culot $

PORTNAME=	lynx
PORTVERSION=	2.8.8.2
PORTEPOCH=	1
PORTREVISION=	1
CATEGORIES=	www ipv6
MASTER_SITES=	http://lynx.isc.org/current/ \
		http://bitrote.org/distfiles/
DISTNAME=	${PORTNAME}${PORTVERSION:R}rel.${PORTVERSION:E}

MAINTAINER=	jharris@widomaker.com
COMMENT=	Non-graphical, text-based World-Wide Web client

LICENSE=	GPLv2

CONFLICTS=	lynx-2.8.[8-9]d*

USES=		tar:bzip2
USE_OPENSSL=	yes
WRKSRC=		${WRKDIR}/${PORTNAME}2-8-8
GNU_CONFIGURE=	yes
CONFIGURE_ARGS+=--with-zlib --libdir="${L_LIB}" \
		--enable-nsl-fork --enable-persistent-cookies \
		--enable-nested-tables --enable-gzip-help \
		--with-screen=ncursesw

MAKE_JOBS_UNSAFE=	yes

OPTIONS_DEFINE=	DEFAULT_COLORS IPV6 NLS SSL DOCS
OPTIONS_DEFAULT=	SSL
DEFAULT_COLORS_DESC=	Colors support
OPTIONS_SUB=	yes

NLS_USES=	gettext iconv
NLS_CONFIGURE_ENABLE=	nls
NLS_CONFIGURE_ON=	${ICONV_CONFIGURE_ARG}

IPV6_CONFIGURE_ENABLE=	ipv6

DEFAULT_COLORS_CONFIGURE_ENABLE=	default-colors

DOCS_CONFIGURE_ENABLE=	local-docs
DOCS_INSTALL_TARGET=	install-full
DOCS_MAKE_ARGS=		helpdir=${L_HELP} docdir=${DOCSDIR}
L_HELP=			${PREFIX}/share/lynx_help

.include <bsd.port.options.mk>

.if ${PORT_OPTIONS:MSSL}
CONFIGURE_ARGS+=	--with-ssl=${OPENSSLBASE}
.endif

MAKEFILE=	makefile

L_LIB=		${PREFIX}/etc

SUB_FILES=	pkg-message

post-install:
# Fix some stage symlinks
	${LN} -sf ${DOCSDIR}/COPYHEADER ${STAGEDIR}${L_HELP}
	${LN} -sf ${DOCSDIR}/COPYING ${STAGEDIR}${L_HELP}

.include <bsd.port.mk>
