--- deleteaccount.py.orig	2013-06-08 11:07:37.000000000 -0500
+++ deleteaccount.py	2013-07-14 09:30:55.092665366 -0500
@@ -38,8 +38,8 @@
         if ( Auth.DeleteAccount(accounts[int(answer)-1]) == None ):
             print(accounts[int(answer)-1] + " deleted. Associated printers were not deleted.")
         else:
-            print("Error deleting stored credentials, perhaps /etc/cloudprint.conf is not writable?")
+            print("Error deleting stored credentials, perhaps /usr/local/etc/cloudprint.conf is not writable?")
     elif ( answer == "0" ):
         break
     else:
-        print("Invalid response, use '0' to exit")
\ No newline at end of file
+        print("Invalid response, use '0' to exit")
