--- lib/BackupPC/CGI/EditConfig.pm.orig	2015-01-30 20:35:47.461970304 -0600
+++ lib/BackupPC/CGI/EditConfig.pm	2015-01-30 20:40:28.517026850 -0600
@@ -492,7 +492,7 @@
     }
 
     my $groupText;
-    foreach my $m ( keys(%ConfigMenu) ) {
+    foreach my $m ( sort(keys(%ConfigMenu)) ) {
         next if ( $menuDisable{$m}{top} );
 	my $text = eval("qq($Lang->{$ConfigMenu{$m}{text}})");
         if ( $m eq $menu ) {
@@ -868,7 +868,7 @@
     #
     # Emit all the remaining editable config settings as hidden values
     #
-    foreach my $param ( keys(%ConfigMeta) ) {
+    foreach my $param ( sort(keys(%ConfigMeta)) ) {
         next if ( $doneParam->{$param} );
         next if ( $userHost
                       && (!defined($bpc->{Conf}{CgiUserConfigEdit}{$param})
@@ -924,7 +924,7 @@
 	# First time: emit all the original config settings
 	#
 	$doneParam = {};
-        foreach my $param ( keys(%ConfigMeta) ) {
+        foreach my $param ( sort(keys(%ConfigMeta)) ) {
             next if ( $doneParam->{$param} );
             next if ( $userHost
                           && (!defined($bpc->{Conf}{CgiUserConfigEdit}{$param})
@@ -1529,7 +1529,7 @@
         $conf = "main config";
     }
 
-    foreach my $p ( keys(%ConfigMeta) ) {
+    foreach my $p ( sort(keys(%ConfigMeta)) ) {
         if ( !exists($oldConf->{$p}) && !exists($newConf->{$p}) ) {
             next;
         } elsif ( exists($oldConf->{$p}) && !exists($newConf->{$p}) ) {
@@ -1605,7 +1605,7 @@
         }
     }
 
-    foreach my $host ( keys(%$hostsNew) ) {
+    foreach my $host ( sort(keys(%$hostsNew)) ) {
         next if ( defined($hostsOld->{$host}) );
         my $dump = Data::Dumper->new([$hostsNew->{$host}]);
         $dump->Indent(0);
