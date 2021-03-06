# Created by: Jon Wilson <jon@phuq.co.uk>
# $FreeBSD: head/sysutils/am-utils/Makefile 382777 2015-03-30 22:39:26Z bapt $

PORTNAME=	am-utils
PORTVERSION=	6.2
PORTEPOCH=	1
CATEGORIES=	sysutils net
MASTER_SITES=	ftp://ftp.am-utils.org/pub/am-utils/ \
		ftp://ftp.cs.umn.edu/pub/am-utils/ \
		ftp://ftp.u-aizu.ac.jp/pub/net/amd/am-utils/

MAINTAINER=	ports@FreeBSD.org
COMMENT=	The Berkeley Automounter Suite of Utilities

LICENSE=	BSD3CLAUSE
LICENSE_FILE=	${WRKSRC}/COPYING

OPTIONS_DEFINE=	OPENLDAP DOCS
OPTIONS_DEFAULT=OPENLDAP
OPENLDAP_DESC=	OpenLDAP support

USES=		bison libtool makeinfo perl5
USE_AUTOTOOLS=	autoconf
USE_LDCONFIG=	yes
MAKE_JOBS_UNSAFE=yes
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--enable-shared
# Enable some filesystems which are not auto-detected with GENERIC kernel.
CONFIGURE_ARGS+=	ac_cv_fs_nullfs=yes ac_cv_mnttab_type_nullfs='"nullfs"' ac_cv_mount_type_nullfs='"nullfs"' \
			ac_cv_fs_udf=yes ac_cv_mnttab_type_udf='"udf"' ac_cv_mount_type_udf='"udf"' \
			ac_cv_fs_tmpfs=yes ac_cv_mnttab_type_tmpfs='"tmpfs"' ac_cv_mount_type_tmpfs='"tmpfs"'
MAKE_ENV=	MAKEINFOFLAGS=--no-split
INSTALL_TARGET=	install-strip

INFO=		am-utils
DOCS=		AUTHORS BUGS FAQ NEWS README.attrcache

OPENLDAP_USE=			OPENLDAP=yes
OPENLDAP_CONFIGURE_WITH=	ldap
OPENLDAP_CONFIGURE_ENV=		LDFLAGS="-L${LOCALBASE}/lib" \
				CPPFLAGS="-I${LOCALBASE}/include"
OPENLDAP_VARS=			DOCS+="README.ldap ldap-id.txt ldap.schema"

OPTIONS_SUB=	yes

.include <bsd.port.options.mk>

post-patch:
	@${RM} ${WRKSRC}/doc/am-utils.info*

post-install:
.if ${PORT_OPTIONS:MDOCS}
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
	cd ${WRKSRC} && ${INSTALL_DATA} ${DOCS} ${STAGEDIR}${DOCSDIR}
.endif

.include <bsd.port.mk>
