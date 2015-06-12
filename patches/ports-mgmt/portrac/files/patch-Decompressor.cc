--- Decompressor.cc.orig	2014-09-29 13:19:21.000000000 -0500
+++ Decompressor.cc	2015-06-11 19:53:42.065668238 -0500
@@ -24,6 +24,18 @@
  */
 
 #include <string>
+#include <sstream>
+
+namespace patch
+{
+    template < typename T > std::string to_string( const T& n )
+    {
+        std::ostringstream stm ;
+        stm << n ;
+        return stm.str() ;
+    }
+}
+
 #include <bzlib.h>
 
 #include "Decompressor.hh"
@@ -41,7 +53,7 @@ void Decompressor::decompress(std::strin
     BZFILE *bzfile = BZ2_bzReadOpen(&bzerror, sourceFile, 0, 0, NULL, 0);
     if(BZ_OK != bzerror)
     {
-        std::string message = Logger::libraryCallError(std::string("bz2"), std::string("BZ2_bzReadOpen"), source, std::to_string(bzerror));
+        std::string message = Logger::libraryCallError(std::string("bz2"), std::string("BZ2_bzReadOpen"), source, patch::to_string(bzerror));
         throw PortracException(message);
     }
     signed char buf[BUFF_LEN] = {0};
