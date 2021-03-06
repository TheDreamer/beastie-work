--- src/ip/Intercept.cc.orig	2015-09-01 19:52:00 UTC
+++ src/ip/Intercept.cc
@@ -203,9 +203,9 @@ Ip::Intercept::IpfInterception(const Com
     if (newConn->remote.isIPv6()) {
 #if IPFILTER_VERSION < 5000003
         // warn once every 10 at critical level, then push down a level each repeated event
-        static int warningLevel = DBG_CRITICAL;
-        debugs(89, warningLevel, "IPF (IPFilter v4) NAT does not support IPv6. Please upgrade to IPFilter v5.1");
-        warningLevel = ++warningLevel % 10;
+        // static int warningLevel = DBG_CRITICAL;
+        // debugs(89, warningLevel, "IPF (IPFilter v4) NAT does not support IPv6. Please upgrade to IPFilter v5.1");
+        // warningLevel = ++warningLevel % 10;
         return false;
 #else
         natLookup.nl_v = 6;
