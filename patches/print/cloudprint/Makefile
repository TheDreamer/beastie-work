# Created by: Bernhard Froehlich <decke@FreeBSD.org>
# $FreeBSD: head/print/cloudprint/Makefile 385643 2015-05-07 17:32:52Z mat $

PORTNAME=	cloudprint
PORTVERSION=	0.12.20150607
CATEGORIES=	print

MAINTAINER=	decke@FreeBSD.org
COMMENT=	Google Cloud Print proxy for local CUPS printers

LICENSE=	GPLv3

RUN_DEPENDS=	${PYTHON_SITELIBDIR}/cups.so:${PORTSDIR}/print/py-cups \
		${PYTHON_PKGNAMEPREFIX}daemon>0:${PORTSDIR}/devel/py-daemon

USE_GITHUB=	yes
GH_ACCOUNT=	armooo
GH_TAGNAME=	a2fdab3

USES=		python:2
USE_PYTHON=	distutils
USE_RC_SUBR=	${PORTNAME}

USERS=		cups
GROUPS=		cups

SUB_LIST+=	PYTHON_CMD="${PYTHON_CMD}" \
		CPUSER="${USERS}" \
		CPGROUP="${GROUPS}"

.include <bsd.port.mk>
