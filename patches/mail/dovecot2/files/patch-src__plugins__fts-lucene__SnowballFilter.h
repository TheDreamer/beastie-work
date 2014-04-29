--- src/plugins/fts-lucene/SnowballFilter.h.orig	2013-03-14 08:28:31.000000000 -0500
+++ src/plugins/fts-lucene/SnowballFilter.h	2014-04-29 09:30:51.625055519 -0500
@@ -8,7 +8,7 @@
 #define _lucene_analysis_snowball_filter_
 
 #include "CLucene/analysis/AnalysisHeader.h"
-#include "libstemmer.h"
+#include "CLucene/snowball/libstemmer.h"
 
 CL_NS_DEF2(analysis,snowball)
 
