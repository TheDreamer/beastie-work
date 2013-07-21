--- src/plugins/fts-lucene/SnowballFilter.h.orig	2012-09-24 15:21:29.000000000 -0500
+++ src/plugins/fts-lucene/SnowballFilter.h	2013-02-03 11:24:06.647137581 -0600
@@ -8,7 +8,7 @@
 #define _lucene_analysis_snowball_filter_
 
 #include "CLucene/analysis/AnalysisHeader.h"
-#include "libstemmer.h"
+#include "CLucene/snowball/libstemmer.h"
 
 CL_NS_DEF2(analysis,snowball)
 
