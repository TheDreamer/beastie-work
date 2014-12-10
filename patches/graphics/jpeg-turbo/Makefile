# Created by: Denis Podolskiy <bytestore@yandex.ru>
# $FreeBSD: head/graphics/jpeg-turbo/Makefile 371283 2014-10-20 17:04:20Z bapt $

PORTNAME=	jpeg-turbo
PORTVERSION=	1.3.1
PORTREVISION=	1
CATEGORIES=	graphics
MASTER_SITES=	SF/lib${PORTNAME}/${PORTVERSION}
DISTNAME=	lib${PORTNAME}-${PORTVERSION}

MAINTAINER=	bytestore@yandex.ru
COMMENT?=	SIMD-accelerated JPEG codec which replaces libjpeg

BUILD_DEPENDS=	nasm:${PORTSDIR}/devel/nasm

ONLY_FOR_ARCHS=	amd64 i386
USES=		libtool
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--with-jpeg8
USE_LDCONFIG=	yes
ALL_TARGET=	test
INSTALL_TARGET=	install-strip

PORTDOCS=	*
PORTEXAMPLES=	*

OPTIONS_DEFINE=	DOCS EXAMPLES PIC REPLACE
OPTIONS_DEFAULT=	REPLACE

PIC_DESC=	Static library with PIC (required for VirtualGL)
REPLACE_DESC=	Drop-in replacement for the graphics/jpeg library

DOCS_MAKE_ARGS=		docdir="${DOCSDIR}"
DOCS_MAKE_ARGS_OFF=	dist_doc_DATA=""

EXAMPLES_MAKE_ARGS=	exampledir="${EXAMPLESDIR}"
EXAMPLES_MAKE_ARGS_OFF=	dist_example_DATA=""

PIC_CONFIGURE_ON=	--with-pic

.include <bsd.port.options.mk>

.if defined(SLAVE_PORT)
PLIST_SUB+=	JPEG="@comment " LIBTURBOJPEG=""
PLIST_SUB+=	REPLACE="" NOREPLACE="" SUFFIX=""
.else
PLIST_SUB+=	JPEG="" LIBTURBOJPEG="@comment "
CONFIGURE_ARGS+=--without-libturbojpeg
.if ${PORT_OPTIONS:MREPLACE}
CONFIGURE_ENV=	SO_MAJOR_VERSION=11
CONFLICTS_INSTALL=	jpeg-[0-9]*
PLIST_SUB+=	REPLACE="" NOREPLACE="@comment " SUFFIX=""
.else
CONFIGURE_ENV=	SO_MAJOR_VERSION=62
CONFIGURE_ARGS+=	--program-suffix=-turbo
PLIST_SUB+=	REPLACE="@comment " NOREPLACE="" SUFFIX="-turbo"

post-install:
	@${ECHO_CMD} "libjpeg.so.11 libjpeg.so.62" >> ${STAGEDIR}${PREFIX}/etc/libmap.d/${PORTNAME}.conf
.endif

.include <bsd.port.mk>
