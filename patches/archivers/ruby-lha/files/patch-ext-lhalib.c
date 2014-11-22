--- ext/lhalib.c.orig	2006-09-18 04:11:35.000000000 -0500
+++ ext/lhalib.c	2014-11-01 09:46:39.370316994 -0500
@@ -18,7 +18,7 @@
 #define LHALIB_VERSION "0.8.1"
 
 #include "lha.h"
-#include "st.h"
+#include "ruby/st.h"
 
 static VALUE lhalib;
 
@@ -75,7 +75,7 @@
   VALUE file;
   proc = Qnil;
   rb_scan_args(argc, argv, "1&", &file, &proc);
-  rb_check_safe_str(file);
+  SafeStringValue(file);
   rb_secure(4);
 
   file_count = 0;
