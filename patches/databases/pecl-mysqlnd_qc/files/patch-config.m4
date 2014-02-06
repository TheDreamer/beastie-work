--- config.m4.orig	2013-03-11 10:22:09.000000000 -0500
+++ config.m4	2013-12-09 11:06:53.339934112 -0600
@@ -89,7 +89,7 @@
         ],[
           AC_MSG_ERROR([wrong SQLite3 lib not found])
         ],[
-          -L$MYSQLND_QC_SQLITE_LIB_DIR/$PHP_LIBDIR -lm
+          -L$MYSQLND_QC_SQLITE_LIB_DIR -lm
         ])
       else
         AC_MSG_ERROR([SQLite3 header not found in $PHP_SQLITE_DIR.])
