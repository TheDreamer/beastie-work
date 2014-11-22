# Created by: Akinori MUSHA aka knu <knu@idaemons.org>
# $FreeBSD: head/devel/ruby-mmap/Makefile 328414 2013-09-26 21:13:59Z tabthorpe $

PORTNAME=	mmap
PORTVERSION=	0.2.7
DISTVERSION=	0.2.6
CATEGORIES=	devel ruby
MASTER_SITES=	ftp://moulon.inra.fr/pub/ruby/
PKGNAMEPREFIX=	${RUBY_PKGNAMEPREFIX}
DIST_SUBDIR=	ruby

MAINTAINER=	clsung@FreeBSD.org
COMMENT=	Ruby interface to manage memory-mapped file objects

#DEPRECATED=	Does not work with Ruby 1.9
#EXPIRATION_DATE=	2013-10-10

USE_RUBY=	yes
USE_RUBY_EXTCONF=	yes
USE_RUBY_RDOC=		yes

RUBY_RD_FILES=	mmap.rd

INSTALL_TARGET=	site-install

DOCS=		Changes \
		README.en \
		${RUBY_RD_FILES} \
		${RUBY_RD_HTML_FILES} \
		doc/doc

.include <bsd.port.pre.mk>

#.if ${RUBY_VER} == 1.9
#BROKEN=	does not build with ruby 1.9
#.endif

post-patch:
.if exists(${RUBY_ARCHLIBDIR}/oniguruma.h)
EXTRA_PATCHES+=	${FILESDIR}/extra-patch-oniguruma
.endif

post-build:
.if !defined(NOPORTDOCS)
	cd ${BUILD_WRKSRC}; ${SETENV} ${MAKE_ENV} ${MAKE} ${MAKE_FLAGS} ${MAKEFILE} ${MAKE_ARGS} rdoc
.endif

post-install:
	${MKDIR} ${STAGEDIR}${RUBY_MODDOCDIR}
.for f in ${DOCS}
	${CP} -R ${WRKSRC}/${f} ${STAGEDIR}${RUBY_MODDOCDIR}/
.endfor

.include <bsd.port.post.mk>
