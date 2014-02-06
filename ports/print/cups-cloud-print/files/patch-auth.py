--- auth.py.orig	2013-06-08 11:07:37.000000000 -0500
+++ auth.py	2013-07-14 09:30:40.365665222 -0500
@@ -23,7 +23,7 @@
   
   clientid = "843805314553.apps.googleusercontent.com"
   clientsecret = 'MzTBsY4xlrD_lxkmwFbBrvBv'
-  config = '/etc/cloudprint.conf'
+  config = '/usr/local/etc/cloudprint.conf'
   
   def GetLPID():
     """Delete an account from the configuration file
