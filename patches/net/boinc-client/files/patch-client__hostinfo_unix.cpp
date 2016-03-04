--- client/hostinfo_unix.cpp.orig	2013-12-20 14:28:08.000000000 -0600
+++ client/hostinfo_unix.cpp	2015-04-02 10:56:06.735994493 -0500
@@ -1213,6 +1213,8 @@ int HOST_INFO::get_virtualbox_version() 
 
 #if LINUX_LIKE_SYSTEM
     safe_strcpy(path, "/usr/lib/virtualbox/VBoxManage");
+#elif defined( __FreeBSD__)
+    safe_strcpy(path, "@PREFIX@/lib/virtualbox/VBoxManage");
 #elif defined( __APPLE__)
     FSRef theFSRef;
     OSStatus status = noErr;
@@ -1231,7 +1233,7 @@ int HOST_INFO::get_virtualbox_version() 
 #endif
 
     if (boinc_file_exists(path)) {
-#if LINUX_LIKE_SYSTEM
+#if LINUX_LIKE_SYSTEM || defined( __FreeBSD__)
         if (access(path, X_OK)) {
             return 0;
         }
@@ -1857,6 +1859,7 @@ bool interrupts_idle(time_t t) {
     }
     return last_irq < t;
 }
+#endif // LINUX_LIKE_SYSTEM
 
 #if HAVE_XSS
 // Ask the X server for user idle time (using XScreenSaver API)
@@ -1929,7 +1932,6 @@ bool xss_idle(long idle_treshold) {
     return idle_treshold < idle_time;
 }
 #endif // HAVE_XSS
-#endif // LINUX_LIKE_SYSTEM
 
 bool HOST_INFO::users_idle(bool check_all_logins, double idle_time_to_run) {
     time_t idle_time = time(0) - (long) (60 * idle_time_to_run);
@@ -1958,6 +1960,7 @@ bool HOST_INFO::users_idle(bool check_al
     // USB mice.  The tty check will catch keyboards if they are entering
     // data into a tty.
     if (!device_idle(idle_time, "/dev/input/mice")) return false;
+#else
 
 #if HAVE_XSS
     if (!xss_idle((long)(idle_time_to_run * 60))) {
@@ -1965,15 +1968,19 @@ bool HOST_INFO::users_idle(bool check_al
     }
 #endif
 
-#else
     // We should find out which of the following are actually relevant
     // on which systems (if any)
     //
+
     if (!device_idle(idle_time, "/dev/mouse")) return false;
         // solaris, linux
     if (!device_idle(idle_time, "/dev/input/mice")) return false;
     if (!device_idle(idle_time, "/dev/kbd")) return false;
         // solaris
+
+    // FreeBSD
+    if (!device_idle(idle_time, "/dev/sysmouse")) return false;
+    if (!device_idle(idle_time, "/dev/kbdmux")) return false;
 #endif
     return true;
 }
