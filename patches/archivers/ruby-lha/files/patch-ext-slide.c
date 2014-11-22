--- ext/slide.c.orig	2006-09-17 12:45:51.000000000 -0500
+++ ext/slide.c	2014-11-01 08:40:14.022188105 -0500
@@ -87,7 +87,7 @@
 
 static unsigned int txtsiz;
 static unsigned long dicsiz;
-static unsigned int remainder;
+static unsigned int lha_remainder;
 
 struct matchdata {
     int len;
@@ -162,7 +162,7 @@
 
     n = fread_crc(crc, &text[txtsiz - dicsiz], dicsiz, infile);
 
-    remainder += n;
+    lha_remainder += n;
 
     *pos -= dicsiz;
     for (i = 0; i < HSHSIZ; i++) {
@@ -270,7 +270,7 @@
         /* re-search */
         search_dict_1(token, pos, 0, off+2, m);
 
-    if (m->len > remainder) m->len = remainder;
+    if (m->len > lha_remainder) m->len = lha_remainder;
 }
 
 /* slide dictionary */
@@ -280,7 +280,7 @@
     unsigned int *pos;
     unsigned int *crc;
 {
-    remainder--;
+    lha_remainder--;
     if (++*pos >= txtsiz - maxmatch) {
         update_dict(pos, crc);
 #ifdef DEBUG
@@ -316,17 +316,17 @@
     encode_set.encode_start();
     memset(text, ' ', TXTSIZ);
 
-    remainder = fread_crc(&crc, &text[dicsiz], txtsiz-dicsiz, infile);
+    lha_remainder = fread_crc(&crc, &text[dicsiz], txtsiz-dicsiz, infile);
 
     match.len = THRESHOLD - 1;
     match.off = 0;
-    if (match.len > remainder) match.len = remainder;
+    if (match.len > lha_remainder) match.len = lha_remainder;
 
     pos = dicsiz;
     token = INIT_HASH(pos);
     insert_hash(token, pos);     /* associate token and pos */
 
-    while (remainder > 0 && ! unpackable) {
+    while (lha_remainder > 0 && ! unpackable) {
         last = match;
 
         next_token(&token, &pos, &crc);
