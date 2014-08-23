# Created by: Gea-Suan Lin <gslin@gslin.org>
# $FreeBSD: head/databases/sqlitemanager/Makefile 359802 2014-06-30 05:51:02Z eadler $

PORTNAME=	sqlitemanager
PORTVERSION=	1.2.4
CATEGORIES=	databases www
MASTER_SITES=	SF
DISTNAME=	SQliteManager-${PORTVERSION}

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Multilingual web based tool to manage SQLite database

WRKSRC=		${WRKDIR}/SQLiteManager-${PORTVERSION}

NO_BUILD=	yes
PLIST_SUB+=	WWWDIR_REL="${WWWDIR_REL}"
USE_PHP=	sqlite3

do-install:
	${MKDIR} ${STAGEDIR}${WWWDIR}
	${CP} -R ${WRKSRC}/ ${STAGEDIR}${WWWDIR}

.include <bsd.port.mk>
