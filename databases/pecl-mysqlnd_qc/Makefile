# Created by: johnpupu <johnpupu@gmail.com>
# $FreeBSD: databases/pecl-mysqlnd_qc/Makefile 327717 2013-09-20 16:13:47Z bapt $

PORTNAME=	mysqlnd_qc
PORTVERSION=	1.2.0
PORTREVISION=	1
CATEGORIES=	databases
MASTER_SITES=	http://pecl.php.net/get/
PKGNAMEPREFIX=	pecl-
EXTRACT_SUFX=	.tgz
DIST_SUBDIR=	PECL

MAINTAINER=	johnpupu@gmail.com
COMMENT=	A query cache plugin for mysqlnd

USE_PHP=	yes
USE_PHPEXT=	yes
USE_PHPIZE=	yes

OPTIONS_DEFINE=	APC MEMCACHE SQLITE

APC_DESC=	APC storage handler
MEMCACHE_DESC=	MEMCACHE storage handler
SQLITE_DESC=	SQLITE storage handler

APC_USE=	PHP=apc
MEMCACHE_RUN_DEPEND=	${PHPBASE}/lib/php/$PHP_EXT_DIR}/memcached.so:${PORTSDIR}/databases/pecl-memcached
MEMCACHE_BUILD_DEPEND=	${PHPBASE}/lib/php/$PHP_EXT_DIR}/memcached.so:${PORTSDIR}/databases/pecl-memcached
SQLITE_USE=	PHP=sqlite3
APC_CONFIGURE_ENABLE=		mysqlnd-qc-apc
MEMCACHE_CONFIGURE_ENABLE=	mysqlnd-qc-memcache
SQLITE_CONFIGURE_ENABLE=	mysqlnd-qc-sqlite
SQLITE_CONFIGURE_WITH=		sqlite-dir=${LOCALBASE}

.include <bsd.port.options.mk>

.if ${PORT_OPTIONS:MAPC}
BROKEN=	APC support requires both APC and MySQL Query Cache to be compiled statically
.endif

NO_STAGE=	yes
.include <bsd.port.mk>
