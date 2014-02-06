--- php_mysqlnd_qc.c.orig	2013-03-11 10:22:09.000000000 -0500
+++ php_mysqlnd_qc.c	2013-12-09 16:01:41.719945167 -0600
@@ -58,7 +58,11 @@
 
 ZEND_DECLARE_MODULE_GLOBALS(mysqlnd_qc)
 
+#ifdef MYSQLND_QC_HAVE_MEMCACHE
+struct st_mysqlnd_qc_methods *mysqlnd_qc_methods = &MYSQLND_CLASS_METHOD_TABLE_NAME(mysqlnd_qc_memcache);
+#else
 struct st_mysqlnd_qc_methods *mysqlnd_qc_methods = &MYSQLND_CLASS_METHOD_TABLE_NAME(mysqlnd_qc_std);
+#endif
 
 struct st_mysqlnd_qc_qcache	norm_query_trace_log;
 
