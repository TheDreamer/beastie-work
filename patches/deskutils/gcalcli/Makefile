# Created by: ijliao
# $FreeBSD: head/deskutils/gcalcli/Makefile 371173 2014-10-19 08:50:17Z mva $

PORTNAME=	gcalcli
PORTVERSION=	3.1
CATEGORIES=	deskutils

MAINTAINER=	lwhsu@FreeBSD.org
COMMENT=	Google Calendar Command Line Interface

LICENSE=	MIT

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}dateutil>=0:${PORTSDIR}/devel/py-dateutil \
		${PYTHON_PKGNAMEPREFIX}gflags>=0:${PORTSDIR}/devel/py-gflags \
		${PYTHON_PKGNAMEPREFIX}httplib2>=0:${PORTSDIR}/www/py-httplib2 \
		${PYTHON_PKGNAMEPREFIX}google-api-python-client>=0:${PORTSDIR}/www/py-google-api-python-client

USE_GITHUB=	yes
GH_ACCOUNT=	insanum
GH_PROJECT=	gcalcli
GH_TAGNAME=	v${PORTVERSION}
GH_COMMIT=	3a13fe1

USES=		python

NO_BUILD=	yes
NO_ARCH=	yes

PORTDOCS=	ChangeLog README.md gcalcli_?.png

PLIST_FILES=	bin/gcalcli

OPTIONS_DEFINE=		DOCS FUZZYDATES ICS_VCAL
OPTIONS_DEFAULT=	ICS_VCAL

ICS_VCAL_DESC=		enable importing of ics/vcal files
FUZZYDATES_DESC=	use parsedatetime for fuzzy date parsing

ICS_VCAL_RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}vobject>=0.4.8:${PORTSDIR}/deskutils/py-vobject
FUZZYDATES_RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}parsedatetime>=0:${PORTSDIR}/devel/py-parsedatetime

.include <bsd.port.options.mk>

do-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/${PORTNAME} ${STAGEDIR}${PREFIX}/bin
.if ${PORT_OPTIONS:MDOCS}
	${MKDIR} ${STAGEDIR}/${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/ChangeLog ${STAGEDIR}${DOCSDIR}
	(cd ${WRKSRC}/docs && ${COPYTREE_SHARE} . ${STAGEDIR}${DOCSDIR})
.endif

.include <bsd.port.mk>
