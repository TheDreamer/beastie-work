--- src/plugins/fts-lucene/SnowballFilter.h.orig	2014-06-02 06:50:10.000000000 -0500
+++ src/plugins/fts-lucene/SnowballFilter.h	2015-05-19 20:46:58.799052362 -0500
@@ -8,7 +8,7 @@
 #define _lucene_analysis_snowball_filter_
 
 #include "CLucene/analysis/AnalysisHeader.h"
-#include "libstemmer.h"
+#include "CLucene/snowball/libstemmer.h"
 
 CL_NS_DEF2(analysis,snowball)
 
