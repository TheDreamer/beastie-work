--- PortChecker.cc.orig	2014-09-29 13:19:21.000000000 -0500
+++ PortChecker.cc	2015-06-11 19:33:58.843669381 -0500
@@ -23,6 +23,8 @@
  * should not be interpreted as representing official policies, either expressed or implied, of e-soul.org.
  */
 
+#include <cstdio>
+#include <cerrno>
 #include <unistd.h>
 #include <stdlib.h>
 #include <sys/wait.h>
