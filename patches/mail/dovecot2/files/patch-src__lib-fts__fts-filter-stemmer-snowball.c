--- src/lib-fts/fts-filter-stemmer-snowball.c.orig	2015-05-09 07:00:34.000000000 -0500
+++ src/lib-fts/fts-filter-stemmer-snowball.c	2015-05-20 10:13:53.000000000 -0500
@@ -7,7 +7,7 @@
 
 #ifdef HAVE_FTS_STEMMER
 
-#include <libstemmer.h>
+#include <CLucene/snowball/libstemmer.h>
 
 struct fts_filter_stemmer_snowball {
 	struct fts_filter filter;
