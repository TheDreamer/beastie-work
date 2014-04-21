# Created by: jrhett@netconsonance.com
# $FreeBSD: head/sysutils/cfengine35/Makefile 349792 2014-03-31 19:13:04Z cy $

PORTNAME=	cfengine
PORTVERSION=	3.5.3
PORTREVISION=	4
CATEGORIES=	sysutils
MASTER_SITES=	http://cfengine.com/source-code/download?file=

MAINTAINER=	cy@FreeBSD.org
#		gjb@FreeBSD.org is also committer for this port
COMMENT=	A systems administration tool for networks

LICENSE=	GPLv3

.if !defined(MASTERDIR)
PKGNAMESUFFIX=	35
LATEST_LINK=	cfengine35
.endif

FETCH_BEFORE_ARGS=	-o ${DISTDIR}/${DISTFILES}

CONFLICTS=	cfengine-[234]*

USE_RC_SUBR=	cf-execd cf-serverd cf-monitord

USE_LDCONFIG=	yes
USE_OPENSSL=	yes
GNU_CONFIGURE=	yes
USES=		gmake
CFLAGS+=	-Wno-return-type
# EXAMPLESDIR=	${PREFIX}/share/examples/cfengine3
DOCSDIR=	${PREFIX}/share/doc/cfengine
CONFIGURE_ARGS=	--docdir=${DOCSDIR} \
		--htmldir=${DOCSDIR}/html \
		--libexecdir=${PREFIX}/libexec \
		--libdir=${PREFIX}/libexec \
		--mandir=${PREFIX}/man \
		--with-workdir=/var/cfengine \
		--with-pcre=${LOCALBASE} \
		--enable-fhs

LIB_DEPENDS+=	libpcre.so:${PORTSDIR}/devel/pcre

OPTIONS_DEFINE=	PGSQL MYSQL LIBVIRT LIBXML2
OPTIONS_SINGLE=	DB
OPTIONS_SINGLE_DB=	TOKYOCABINET QDBM
OPTIONS_RADIO=	STAT
OPTIONS_RADIO_STAT=	STATIC PARTIAL

TOKYOCABINET_DESC=	Enable TokyoCabinet database backend
QDBM_DESC=	Enable QDBM database backend
LIBXML2_DESC=	Enable edit_xml bundle

STAT_DESC=	Build static cf-agent/cf-promises?
PARTIAL_DESC=	Build with static libpromises(/libxml2)
 
OPTIONS_DEFAULT=TOKYOCABINET

LIBXML2_CONFIGURE_ON=	--with-libxml2=${LOCALBASE}
LIBXML2_CONFIGURE_OFF=	--without-libxml2
LIBXML2_LIB_DEPENDS=	libxml2.so:${PORTSDIR}/textproc/libxml2

STATIC_CONFIGURE_ON=	--enable-static
PARTIAL_CONFIGURE_ON=	--enable-static

.include <bsd.port.pre.mk>

.if ${PORT_OPTIONS:MTOKYOCABINET}
CONFIGURE_ARGS+=	--with-tokyocabinet=${LOCALBASE}
LIB_DEPENDS+=	libtokyocabinet.so:${PORTSDIR}/databases/tokyocabinet
.endif

.if ${PORT_OPTIONS:MQDBM}
CONFIGURE_ARGS+=	--with-qdbm=${LOCALBASE}
LIB_DEPENDS+=	libqdbm.so:${PORTSDIR}/databases/qdbm
.endif

.if ${PORT_OPTIONS:MPGSQL}
USE_PGSQL=		yes
CONFIGURE_ARGS+=	--with-postgresql=${LOCALBASE}
LIB_DEPENDS+=		libpq.so:${PORTSDIR}/databases/postgresql${PGSQL_VER}-client
.endif

.if ${PORT_OPTIONS:MMYSQL}
USE_MYSQL=		yes
CONFIGURE_ARGS+=	--with-mysql=${LOCALBASE}
LIB_DEPENDS+=		libmysqlclient.so:${PORTSDIR}/${_MYSQL_CLIENT}
LDFLAGS+=		-L${LOCALBASE}/lib/mysql
.endif

.if ${PORT_OPTIONS:MLIBVIRT}
CONFIGURE_ARGS+=	--with-libvirt=${LOCALBASE}
LIB_DEPENDS+=		libvirt.so:${PORTSDIR}/devel/libvirt
.endif

.if ${PORT_OPTIONS:MPARTIAL}
post-configure:
	@${REINPLACE_CMD} \
		-e 's|^AM_LDFLAGS =|AM_LDFLAGS = -static-libtool-libs|' \
		${WRKSRC}/cf-agent/Makefile
	@${REINPLACE_CMD} \
		-e 's|^AM_LDFLAGS =|AM_LDFLAGS = -static-libtool-libs|' \
		${WRKSRC}/cf-promises/Makefile
.if ${PORT_OPTIONS:MLIBXML2}
	@${REINPLACE_CMD} \
		-e "s|-lxml2|`xml2-config --libtool-libs`|" \
		${WRKSRC}/cf-agent/Makefile
	@${REINPLACE_CMD} \
		-e "s|-lxml2|`xml2-config --libtool-libs`|" \
		${WRKSRC}/cf-promises/Makefile
.endif
.endif

.if ${PORT_OPTIONS:MSTATIC}
post-configure:
	@${REINPLACE_CMD} \
		-e 's|^AM_LDFLAGS =|AM_LDFLAGS = -all-static|' \
		${WRKSRC}/cf-agent/Makefile
	@${REINPLACE_CMD} \
		-e 's|^AM_LDFLAGS =|AM_LDFLAGS = -all-static|' \
		${WRKSRC}/cf-promises/Makefile
.if ${PORT_OPTIONS:MTOKYOCABINET}
	@${REINPLACE_CMD} \
		-e "s|-ltokyocabinet|`pkg-config tokyocabinet --static --libs`|" \
		${WRKSRC}/cf-agent/Makefile
	@${REINPLACE_CMD} \
		-e "s|-ltokyocabinet|`pkg-config tokyocabinet --static --libs`|" \
		${WRKSRC}/cf-promises/Makefile
.endif
.endif

.include <bsd.port.post.mk>
