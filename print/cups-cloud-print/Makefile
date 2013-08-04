# Created by: Lawrence Chen <beastie@tardisi.com>
# $FreeBSD$

PORTNAME=	cups-cloud-print
PORTVERSION=	20130608
CATEGORIES=	print


MAINTAINER=	beastie@tardisi.com
COMMENT=	Google Cloud Print driver for CUPS

LICENSE=	GPLv3 LGPL3
LICENSE_COMB=	multi

#USE_GITHUB=	yes
GH_ACCOUNT=     simoncadman
GH_PROJECT=     CUPS-Cloud-Print
GH_TAGNAME=     ${PORTVERSION}
GH_COMMIT=	41833b4

MASTER_SITES=	https://github.com/${GH_ACCOUNT}/${GH_PROJECT}/archive/${GH_TAGNAME}.tar.gz?dummy=/
FETCH_ARGS=	-Fpr
DISTNAME=	${GH_PROJECT}-${GH_TAGNAME}

BUILD_DEPENDS=	ppdc:${PORTSDIR}/print/cups-base
RUN_DEPENDS=	cups:${PORTSDIR}/print/cups-client \
		${PYTHON_SITELIBDIR}/cups.so:${PORTSDIR}/print/py-cups \
		${PKGNAMEPREFIX}httplib2>=0:${PORTSDIR}/www/py-httplib2

USE_PYTHON=	2.6-2.7
USE_GHOSTSCRIPT_RUN=	9

GNU_CONFIGURE=	yes
USE_PKGCONFIG=  build
USE_GMAKE=	yes

post-install:
		chgrp cups /etc/cloudprint.conf
		/usr/local/lib/cloudprint-cups/upgrade.py	

.include <bsd.port.mk>
