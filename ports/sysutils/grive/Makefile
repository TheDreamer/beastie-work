# Created by: Lawrence Chen <beastie@tardisi.com>
# $FreeBSD$

PORTNAME=	grive
PORTVERSION=	0.2.0
CATEGORIES=	sysutils www
MASTER_SITES=	http://www.lbreda.com/grive/_media/packages/0.2.0/

MAINTAINER=	beastie@tardisi.com
COMMENT=	Independent open source Google Drive client implementation

BUILD_DEPENDS=	boost-libs>=1.52:${PORTSDIR}/devel/boost-libs \
		curl:${PORTSDIR}/ftp/curl \
		json-c>=0.9:${PORTSDIR}/devel/json-c \
		libgcrypt>=1.5.2:${PORTSDIR}/security/libgcrypt \
		expat>=2.1.0:${PORTSDIR}/textproc/expat2
RUN_DEPENDS:=	${BUILD_DEPENDS}

MAN1=		grive.1

USE_BINUTILS=	yes
USES=		cmake:outsource

SUB_FILES=	README

post-patch:
	@${REINPLACE_CMD -e 's|log::|gr::log::|g' ${WRKSRC}/grive/src/main.cc

post-install:
	@${MKDIR} ${DATATDIR}
	@${INSTALL_PROGRAM} ${WRKSRC}/grive/grive ${DATADIR}
	@${INSTALL_DATA} ${WRKDIR}/README ${DATADIR}
	@${INSTALL_MAN} ${WRKSRC}/grive/doc/grive.1 ${MANPREFIX}/man/man1/

.include <bsd.port.mk>
