# Created by: Gleb Kurtsou <gleb@FreeBSD.org>
# $FreeBSD: head/sysutils/pefs-kmod/Makefile 341389 2014-01-27 12:30:10Z rene $

PORTNAME=	pefs
DISTVERSION=	2013-10-13
CATEGORIES=	sysutils
MASTER_SITES=	https://pefs.googlecode.com/files/
PKGNAMESUFFIX=	-kmod

MAINTAINER=	gleb@FreeBSD.org
COMMENT=	PEFS kernel level stacked cryptographic filesystem

LICENSE=	BSD2CLAUSE

MAKE_ENV=	BINDIR="${PREFIX}/sbin" MANDIR="${MANPREFIX}/man/man" \
		NO_MANCOMPRESS=

ONLY_FOR_ARCHS=	i386 amd64	# not tested on other archs

OPTIONS_DEFINE=	AESNI
AESNI_DESC=	Use AES accelerator on (some) Intel CPUs

AESNI_MAKE_ENV=	PEFS_AESNI=yes

USES=		kmod uidfix

.include <bsd.port.mk>
