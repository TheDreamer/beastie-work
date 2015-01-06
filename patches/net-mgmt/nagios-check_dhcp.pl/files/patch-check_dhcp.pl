--- check_dhcp.pl.orig	2015-01-05 20:13:21.750727469 -0600
+++ check_dhcp.pl	2015-01-05 20:21:16.975726358 -0600
@@ -25,8 +25,7 @@
 # 
 #
 use strict;
-use IO::Socket;
-use IO::Interface;
+use IO::Interface::Simple;
 use Net::DHCP::Watch;
 use Getopt::Long qw(:config no_ignore_case);
 
@@ -101,12 +100,12 @@
 } 
 
 # Get the mac and IP address for the given interface
-my $s = IO::Socket::INET->new(Proto => 'udp');
-if ( !($ip  = $s->if_addr($interface)) ){
+my $if = IO::Interface::Simple->new($interface);
+if ( !($ip  = $if->address) ){
     print "Error: $!\n";
     exit($STATUSCODE{'CRITICAL'});
 }
-if ( !($mac  = $s->if_hwaddr($interface)) ){
+if ( !($mac  = $if->hwaddr) ){
     print "Error: $!\n";
     exit($STATUSCODE{'CRITICAL'});
 }

