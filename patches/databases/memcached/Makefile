# Created by: Sean Chittenden <seanc@FreeBSD.org>
# $FreeBSD: head/databases/memcached/Makefile 347120 2014-03-05 16:28:10Z bapt $

PORTNAME=	memcached
PORTVERSION=	1.4.17
PORTREVISION=	1
CATEGORIES=	databases
MASTER_SITES=	${MASTER_SITE_GOOGLE_CODE} \
		${MASTER_SITE_GENTOO}
MASTER_SITE_SUBDIR=	distfiles

MAINTAINER=	swills@FreeBSD.org
COMMENT=	High-performance distributed memory object cache system

LIB_DEPENDS=	libevent-2.0.so:${PORTSDIR}/devel/libevent2

TEST_DEPENDS=	prove:${PORTSDIR}/lang/${PERL_PORT} \
		git:${PORTSDIR}/devel/git

CONFLICTS=	memcached-1.2*

# Dtrace option disabled until it is fixed upstream
OPTIONS_DEFINE=	SASL DOCS
DTRACE_DESC=	Enable dtrace probes
SASL_DESC=	Enable SASL Authentication

USE_RC_SUBR=	memcached
SUB_LIST+=	RC_SUBR_SUFFIX=${RC_SUBR_SUFFIX}

USE_CSTD=	c99
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--with-libevent=${LOCALBASE}/lib/event2 --program-prefix= \
		--disable-coverage

MAN1=		memcached.1

PLIST_FILES=	bin/memcached \
		bin/memcached-tool \
		include/memcached/protocol_binary.h
PLIST_DIRS=	include/memcached

PORTDOCS=	protocol.txt readme.txt

CFLAGS+=	-fstack-protector \
		-I${LOCALBASE}/include \
		-I${LOCALBASE}/include/event2/compat

DTRACE_COFNIGURE_ENABLE=	dtrace

SASL_LIB_DEPENDS=	libsasl2.so:${PORTSDIR}/security/cyrus-sasl2
SASL_CONFIGURE_ENABLE=	sasl
SASL_CFLAGS=		-I${LOCALBASE}/include
SASL_CPPFLAGS+=		${CFLAGS}
SASL_LDFLAGS=		-L${LOCALBASE}/lib

.include <bsd.port.pre.mk>

.if ${ARCH} == amd64
CONFIGURE_ARGS+=	--enable-64bit
.endif

.if ${PORT_OPTIONS:MDOCS}
PLIST_FILES+=	man/man1/memcached.1.gz
.endif

.if ${PORT_OPTIONS:MDTRACE}
BROKEN=			DTRACE broken with gcc/clang - reported upstream
.endif

post-patch:
	@${REINPLACE_CMD} -e "s,-levent,-levent-2.0,g" ${WRKSRC}/configure

post-configure:
	@${REINPLACE_CMD} -e 's#doc/memcached.1##' ${WRKSRC}/Makefile

post-install:
	${INSTALL_SCRIPT} ${WRKSRC}/scripts/memcached-tool ${STAGEDIR}${PREFIX}/bin
.if ${PORT_OPTIONS:MDOCS}
	${INSTALL_MAN} ${WRKSRC}/doc/${MAN1} ${STAGEDIR}${MAN1PREFIX}/man/man1
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
.for i in ${PORTDOCS}
	${INSTALL_DATA} ${WRKSRC}/doc/${i} ${STAGEDIR}${DOCSDIR}
.endfor
.endif

regression-test: test
test: build
	${MAKE} -C ${WRKSRC} test

.include <bsd.port.post.mk>
