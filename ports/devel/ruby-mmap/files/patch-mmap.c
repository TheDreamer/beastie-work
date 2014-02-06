--- mmap.c.orig	2005-09-15 08:18:38.000000000 -0500
+++ mmap.c	2013-11-19 08:31:37.320290664 -0600
@@ -12,23 +12,8 @@
 #include <sys/sem.h>
 #endif
 
-#include <rubyio.h>
-#include <intern.h>
-#include <re.h>
-
-#ifndef StringValue
-#define StringValue(x) do { 				\
-    if (TYPE(x) != T_STRING) x = rb_str_to_str(x); 	\
-} while (0)
-#endif
-
-#ifndef StringValuePtr
-#define StringValuePtr(x) STR2CSTR(x)
-#endif
-
-#ifndef SafeStringValue
-#define SafeStringValue(x) Check_SafeStr(x)
-#endif
+#include <ruby/io.h>
+#include <ruby/re.h>
 
 #ifndef MADV_NORMAL
 #ifdef POSIX_MADV_NORMAL
@@ -41,8 +26,8 @@
 #endif
 #endif
 
-#define BEG(no) regs->beg[no]
-#define END(no) regs->end[no]
+#define BEG(no) rmatch->regs.beg[no]
+#define END(no) rmatch->regs.end[no]
 
 #ifndef MMAP_RETTYPE
 #ifndef _POSIX_C_SOURCE
@@ -119,8 +104,7 @@
 #endif
 
 static void
-mm_free(i_mm)
-    mm_ipc *i_mm;
+mm_free(mm_ipc *i_mm)
 {
 #if HAVE_SEMCTL && HAVE_SHMCTL
     if (i_mm->t->flag & MM_IPC) {
@@ -157,9 +141,7 @@
 }
 
 static void
-mm_lock(i_mm, wait_lock)
-    mm_ipc *i_mm;
-    int wait_lock;
+mm_lock(mm_ipc *i_mm, int wait_lock)
 {
 #if HAVE_SEMCTL && HAVE_SHMCTL
     struct sembuf sem_op;
@@ -187,8 +169,7 @@
 }
 
 static void
-mm_unlock(i_mm)
-    mm_ipc *i_mm;
+mm_unlock(mm_ipc *i_mm)
 {
 #if HAVE_SEMCTL && HAVE_SHMCTL
     struct sembuf sem_op;
@@ -222,8 +203,7 @@
     }
 
 static VALUE
-mm_vunlock(obj)
-    VALUE obj;
+mm_vunlock(VALUE obj)
 {
     mm_ipc *i_mm;
 
@@ -232,10 +212,13 @@
     return Qnil;
 }
 
+/*
+ * call-seq: semlock
+ *
+ * Create a lock
+ */
 static VALUE
-mm_semlock(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_semlock(int argc, VALUE *argv, VALUE obj)
 {
     mm_ipc *i_mm;
 
@@ -259,9 +242,13 @@
     return Qnil;
 }
 
+/*
+ * call-seq: ipc_key
+ *
+ * Get the ipc key
+ */
 static VALUE
-mm_ipc_key(obj)
-    VALUE obj;
+mm_ipc_key(VALUE obj)
 {
     mm_ipc *i_mm;
 
@@ -272,9 +259,16 @@
     return INT2NUM(-1);
 }
 
+/*
+ * Document-method: munmap
+ * Document-method: unmap
+ *
+ * call-seq: munmap
+ *
+ * terminate the association
+ */
 static VALUE
-mm_unmap(obj)
-    VALUE obj;
+mm_unmap(VALUE obj)
 {
     mm_ipc *i_mm;
 
@@ -295,9 +289,13 @@
     return Qnil;
 }
 
+/*
+ * call-seq: freeze
+ *
+ * freeze the current file 
+ */
 static VALUE
-mm_freeze(obj)
-    VALUE obj;
+mm_freeze(VALUE obj)
 {
     mm_ipc *i_mm;
     rb_obj_freeze(obj);
@@ -307,9 +305,7 @@
 }
 
 static VALUE
-mm_str(obj, modify)
-    VALUE obj;
-    int modify;
+mm_str(VALUE obj, int modify)
 {
     mm_ipc *i_mm;
     VALUE ret = Qnil;
@@ -320,29 +316,17 @@
 	if (!OBJ_TAINTED(ret) && rb_safe_level() >= 4)
 	    rb_raise(rb_eSecurityError, "Insecure: can't modify mmap");
     }
-#if HAVE_RB_DEFINE_ALLOC_FUNC
     ret = rb_obj_alloc(rb_cString);
     if (rb_obj_tainted(obj)) {
 	OBJ_TAINT(ret);
     }
-#else
-    if (rb_obj_tainted(obj)) {
-	ret = rb_tainted_str_new2("");
-    }
-    else {
-	ret = rb_str_new2("");
-    }
-    free(RSTRING(ret)->ptr);
-#endif
-    RSTRING(ret)->ptr = i_mm->t->addr;
-    RSTRING(ret)->len = i_mm->t->real;
+    FL_SET(ret, RSTRING_NOEMBED);
+    RSTRING(ret)->as.heap.ptr = i_mm->t->addr;
+    RSTRING(ret)->as.heap.len = i_mm->t->real;
     if (modify & MM_ORIGIN) {
-#if HAVE_RB_DEFINE_ALLOC_FUNC
-	RSTRING(ret)->aux.shared = ret;
+	RSTRING(ret)->as.heap.aux.shared = ret;
 	FL_SET(ret, ELTS_SHARED);
-#else
-	RSTRING(ret)->orig = ret;
-#endif
+        OBJ_FREEZE(ret);
     }
     if (i_mm->t->flag & MM_FROZEN) {
 	ret = rb_obj_freeze(ret);
@@ -350,9 +334,13 @@
     return ret;
 }
 
+/*
+ * call-seq: to_str
+ *
+ * Convert object to a string
+ */
 static VALUE
-mm_to_str(obj)
-    VALUE obj;
+mm_to_str(VALUE obj)
 {
     return mm_str(obj, MM_ORIGIN);
 }
@@ -365,8 +353,7 @@
 } mm_st;
 
 static VALUE
-mm_i_expand(st_mm)
-    mm_st *st_mm;
+mm_i_expand(mm_st *st_mm)
 {
     int fd;
     mm_ipc *i_mm = st_mm->i_mm;
@@ -407,9 +394,7 @@
 }
 
 static void
-mm_expandf(i_mm, len)
-    mm_ipc *i_mm;
-    size_t len;
+mm_expandf(mm_ipc *i_mm, size_t len)
 {
     int status;
     mm_st st_mm;
@@ -427,7 +412,7 @@
     st_mm.len = len;
     if (i_mm->t->flag & MM_IPC) {
 	mm_lock(i_mm, Qtrue);
-	rb_protect(mm_i_expand, (VALUE)&st_mm, &status);
+	rb_protect((VALUE (*)(VALUE))mm_i_expand, (VALUE)&st_mm, &status);
 	mm_unlock(i_mm);
 	if (status) {
 	    rb_jump_tag(status);
@@ -439,9 +424,7 @@
 }
 
 static void
-mm_realloc(i_mm, len)
-    mm_ipc *i_mm;
-    size_t len;
+mm_realloc(mm_ipc *i_mm, size_t len)
 {
     if (i_mm->t->flag & MM_FROZEN) rb_error_frozen("mmap");
     if (len > i_mm->t->len) {
@@ -452,9 +435,14 @@
     }
 }
 
+/*
+ * call-seq:
+ *   extend(count)
+ *
+ * add <em>count</em> bytes to the file (i.e. pre-extend the file) 
+ */
 static VALUE
-mm_extend(obj, a)
-    VALUE obj, a;
+mm_extend(VALUE obj, VALUE a)
 {
     mm_ipc *i_mm;
     long len;
@@ -468,8 +456,7 @@
 }
 
 static VALUE
-mm_i_options(arg, obj)
-    VALUE arg, obj;
+mm_i_options(VALUE arg, VALUE obj)
 {
     mm_ipc *i_mm;
     char *options;
@@ -524,8 +511,7 @@
 #if HAVE_SEMCTL && HAVE_SHMCTL
 
 static VALUE
-mm_i_ipc(arg, obj)
-    VALUE arg, obj;
+mm_i_ipc(VALUE arg, VALUE obj)
 {
     mm_ipc *i_mm;
     char *options;
@@ -555,10 +541,50 @@
 
 #endif
 
+/*
+ * call-seq:
+ *  new(file, mode = "r", protection = Mmap::MAP_SHARED, options = {})
+ *
+ * create a new Mmap object
+ * 
+ * * <em>file</em>
+ * 
+ *   Pathname of the file, if <em>nil</em> is given an anonymous map
+ *   is created <em>Mmanp::MAP_ANON</em>
+ * 
+ * * <em>mode</em>
+ * 
+ *   Mode to open the file, it can be "r", "w", "rw", "a"
+ * 
+ * * <em>protection</em>
+ * 
+ *   specify the nature of the mapping
+ * 
+ *   * <em>Mmap::MAP_SHARED</em>
+ * 
+ *     Creates a mapping that's shared with all other processes 
+ *     mapping the same areas of the file. 
+ *     The default value is <em>Mmap::MAP_SHARED</em>
+ * 
+ *   * <em>Mmap::MAP_PRIVATE</em>
+ * 
+ *     Creates a private copy-on-write mapping, so changes to the
+ *     contents of the mmap object will be private to this process
+ * 
+ * * <em>options</em>
+ * 
+ *   Hash. If one of the options <em>length</em> or <em>offset</em>
+ *   is specified it will not possible to modify the size of
+ *   the mapped file.
+ * 
+ *   length:: maps <em>length</em> bytes from the file
+ * 
+ *   offset:: the mapping begin at <em>offset</em>
+ * 
+ *   advice:: the type of the access (see #madvise)
+ */
 static VALUE
-mm_s_new(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_s_new(int argc, VALUE *argv, VALUE obj)
 {
     VALUE res = rb_funcall2(obj, rb_intern("allocate"), 0, 0);
     rb_obj_call_init(res, argc, argv);
@@ -566,8 +592,7 @@
 }
 
 static VALUE
-mm_s_alloc(obj)
-    VALUE obj;
+mm_s_alloc(VALUE obj)
 {
     VALUE res;
     mm_ipc *i_mm;
@@ -579,10 +604,13 @@
     return res;
 }
 
+/*
+ * call-seq: initialize
+ *
+ * Create a new Mmap object
+ */
 static VALUE
-mm_init(argc, argv, obj)
-    VALUE obj, *argv;
-    int argc;
+mm_init(int argc, VALUE *argv, VALUE obj)
 {
     struct stat st;
     int fd, smode = 0, pmode = 0, vscope, perm, init;
@@ -651,13 +679,13 @@
             VALUE tmp;
 
             vmode = rb_convert_type(vmode, T_ARRAY, "Array", "to_ary");
-            if (RARRAY(vmode)->len != 2) {
+            if (RARRAY(vmode)->as.heap.len != 2) {
                 rb_raise(rb_eArgError, "Invalid length %d (expected 2)",
-                         RARRAY(vmode)->len);
+                         RARRAY(vmode)->as.heap.len);
             }
-	    tmp = RARRAY(vmode)->ptr[0];
+	    tmp = RARRAY(vmode)->as.heap.ptr[0];
 	    mode = StringValuePtr(tmp);
-	    perm = NUM2INT(RARRAY(vmode)->ptr[1]);
+	    perm = NUM2INT(RARRAY(vmode)->as.heap.ptr[1]);
 	}
 	else {
 	    mode = StringValuePtr(vmode);
@@ -853,10 +881,28 @@
     return obj;
 }
 
+/*
+ * call-seq: initialize
+ *
+ * Create a new Mmap object
+ */
+static VALUE
+mm_init_copy(VALUE obj, VALUE other)
+{
+    rb_raise(rb_eTypeError, "can't copy %s", rb_obj_classname(other));
+}
+
+/*
+ * Document-method: msync
+ * Document-method: sync
+ * Document-method: flush
+ *
+ * call-seq: msync
+ *
+ * flush the file
+ */
 static VALUE
-mm_msync(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_msync(int argc, VALUE *argv, VALUE obj)
 {
     mm_ipc *i_mm;
     VALUE oflag;
@@ -876,9 +922,16 @@
     return obj;
 }
 
+/*
+ * Document-method: mprotect
+ * Document-method: protect
+ *
+ * call-seq: mprotect(mode)
+ *
+ * change the mode, value must be "r", "w" or "rw"
+ */
 static VALUE
-mm_mprotect(obj, a)
-    VALUE obj, a;
+mm_mprotect(VALUE obj, VALUE a)
 {
     mm_ipc *i_mm;
     int ret, pmode;
@@ -920,9 +973,19 @@
 }
 
 #ifdef MADV_NORMAL
+/*
+ * Document-method: madvise
+ * Document-method: advise
+ *
+ * call-seq: madvise(advice)
+ *
+ * <em>advice</em> can have the value <em>Mmap::MADV_NORMAL</em>,
+ * <em>Mmap::MADV_RANDOM</em>, <em>Mmap::MADV_SEQUENTIAL</em>,
+ * <em>Mmap::MADV_WILLNEED</em>, <em>Mmap::MADV_DONTNEED</em>
+ *
+ */
 static VALUE
-mm_madvise(obj, a)
-    VALUE obj, a;
+mm_madvise(VALUE obj, VALUE a)
 {
     mm_ipc *i_mm;
     
@@ -945,23 +1008,19 @@
     }									   \
     else {								   \
 	bp = StringValuePtr(b);						   \
-	bl = RSTRING(b)->len;						   \
+	bl = RSTRING_LEN(b);   				       	           \
     }									   \
 } while (0);
 
 static void
-mm_update(str, beg, len, val)
-    mm_ipc *str;
-    VALUE val;
-    long beg;
-    long len;
+mm_update(mm_ipc *str, long beg, long len, VALUE val)
 {
     char *valp;
     long vall;
 
     if (str->t->flag & MM_FROZEN) rb_error_frozen("mmap");
     if (len < 0) rb_raise(rb_eIndexError, "negative length %d", len);
-    mm_lock(str);
+    mm_lock(str, Qfalse);
     if (beg < 0) {
 	beg += str->t->real;
     }
@@ -978,7 +1037,7 @@
 
     mm_unlock(str);
     StringMmap(val, valp, vall);
-    mm_lock(str);
+    mm_lock(str, Qfalse);
 
     if ((str->t->flag & MM_FIXED) && vall != len) {
 	mm_unlock(str);
@@ -1003,9 +1062,13 @@
     mm_unlock(str);
 }
 
+/*
+ * call-seq: =~(other)
+ *
+ * return an index of the match 
+ */
 static VALUE
-mm_match(x, y)
-    VALUE x, y;
+mm_match(VALUE x, VALUE y)
 {
     VALUE reg, res;
     long start;
@@ -1034,8 +1097,7 @@
 }
 
 static VALUE
-get_pat(pat)
-    VALUE pat;
+get_pat(VALUE pat)
 {
     switch (TYPE(pat)) {
       case T_REGEXP:
@@ -1053,7 +1115,7 @@
 }
 
 static int
-mm_correct_backref()
+mm_correct_backref(void)
 {
     VALUE match;
     int i, start;
@@ -1065,7 +1127,7 @@
     RMATCH(match)->str = rb_str_new(StringValuePtr(RMATCH(match)->str) + start,
 				    RMATCH(match)->END(0) - start);
     if (OBJ_TAINTED(match)) OBJ_TAINT(RMATCH(match)->str);
-    for (i = 0; i < RMATCH(match)->regs->num_regs && RMATCH(match)->BEG(i) != -1; i++) {
+    for (i = 0; i < RMATCH(match)->rmatch->regs.num_regs && RMATCH(match)->BEG(i) != -1; i++) {
 	RMATCH(match)->BEG(i) -= start;
 	RMATCH(match)->END(i) -= start;
     }
@@ -1074,8 +1136,7 @@
 }
 
 static VALUE
-mm_sub_bang_int(bang_st)
-    mm_bang *bang_st;
+mm_sub_bang_int(mm_bang *bang_st)
 {
     int argc = bang_st->argc;
     VALUE *argv = bang_st->argv;
@@ -1086,6 +1147,7 @@
     int tainted = 0;
     long plen;
     mm_ipc *i_mm;
+    char *ptr;
 
     if (argc == 1 && rb_block_given_p()) {
 	iter = 1;
@@ -1105,34 +1167,34 @@
     if (rb_reg_search(pat, str, 0, 0) >= 0) {
 	start = mm_correct_backref();
 	match = rb_backref_get();
-	regs = RMATCH(match)->regs;
+	regs = &RMATCH(match)->rmatch->regs;
 	if (iter) {
 	    rb_match_busy(match);
 	    repl = rb_obj_as_string(rb_yield(rb_reg_nth_match(0, match)));
 	    rb_backref_set(match);
 	}
 	else {
-	    RSTRING(str)->ptr += start;
-	    repl = rb_reg_regsub(repl, str, regs);
-	    RSTRING(str)->ptr -= start;
+	    RSTRING(str)->as.heap.ptr += start;
+	    repl = rb_reg_regsub(repl, str, regs, RMATCH(match)->regexp);
+	    RSTRING(str)->as.heap.ptr -= start;
 	}
 	if (OBJ_TAINTED(repl)) tainted = 1;
-	plen = END(0) - BEG(0);
-	if (RSTRING(repl)->len > plen) {
-	    mm_realloc(i_mm, RSTRING(str)->len + RSTRING(repl)->len - plen);
-	    RSTRING(str)->ptr = i_mm->t->addr;
+	plen = regs->end[0] - regs->beg[0];
+	if (RSTRING_LEN(repl) > plen) {
+	    mm_realloc(i_mm, RSTRING(str)->as.heap.len + RSTRING_LEN(repl) - plen);
+	    RSTRING(str)->as.heap.ptr = i_mm->t->addr;
 	}
-	if (RSTRING(repl)->len != plen) {
+	ptr = RSTRING(str)->as.heap.ptr + start + regs->beg[0];
+	if (RSTRING_LEN(repl) != plen) {
 	    if (i_mm->t->flag & MM_FIXED) {
 		rb_raise(rb_eTypeError, "try to change the size of a fixed map");
 	    }
-	    memmove(RSTRING(str)->ptr + start + BEG(0) + RSTRING(repl)->len,
-		    RSTRING(str)->ptr + start + BEG(0) + plen,
-		    RSTRING(str)->len - start - BEG(0) - plen);
-	}
-	memcpy(RSTRING(str)->ptr + start + BEG(0),
-	       RSTRING(repl)->ptr, RSTRING(repl)->len);
-	i_mm->t->real += RSTRING(repl)->len - plen;
+	    memmove(ptr + RSTRING_LEN(repl),
+		    ptr + plen,
+		    RSTRING(str)->as.heap.len - start - regs->beg[0] - plen);
+	}
+	memcpy(ptr, RSTRING_PTR(repl), RSTRING_LEN(repl));
+	i_mm->t->real += RSTRING_LEN(repl) - plen;
 	if (tainted) OBJ_TAINT(obj);
 
 	res = obj;
@@ -1141,11 +1203,15 @@
     return res;
 }
 
+/*
+ * call-seq:
+ *    str.sub!(pattern, replacement)      => str or nil
+ *    str.sub!(pattern) {|match| block }  => str or nil
+ *
+ * substitution 
+ */
 static VALUE
-mm_sub_bang(argc, argv, obj)
-    int argc;
-    VALUE *argv;
-    VALUE obj;
+mm_sub_bang(int argc, VALUE *argv, VALUE obj)
 {
     VALUE res;
     mm_bang bang_st;
@@ -1166,8 +1232,7 @@
 }
 
 static VALUE
-mm_gsub_bang_int(bang_st)
-    mm_bang *bang_st;
+mm_gsub_bang_int(mm_bang *bang_st)
 {
     int argc = bang_st->argc;
     VALUE *argv = bang_st->argv;
@@ -1179,6 +1244,7 @@
     int tainted = 0;
     long plen;
     mm_ipc *i_mm;
+    char *ptr;
 
     if (argc == 1 && rb_block_given_p()) {
 	iter = 1;
@@ -1203,42 +1269,42 @@
     while (beg >= 0) {
 	start = mm_correct_backref();
 	match = rb_backref_get();
-	regs = RMATCH(match)->regs;
+	regs = &RMATCH(match)->rmatch->regs;
 	if (iter) {
 	    rb_match_busy(match);
 	    val = rb_obj_as_string(rb_yield(rb_reg_nth_match(0, match)));
 	    rb_backref_set(match);
 	}
 	else {
-	    RSTRING(str)->ptr += start;
-	    val = rb_reg_regsub(repl, str, regs);
-	    RSTRING(str)->ptr -= start;
+	    RSTRING(str)->as.heap.ptr += start;
+	    val = rb_reg_regsub(repl, str, regs, RMATCH(match)->regexp);
+	    RSTRING(str)->as.heap.ptr -= start;
 	}
 	if (OBJ_TAINTED(repl)) tainted = 1;
-	plen = END(0) - BEG(0);
-	if ((i_mm->t->real + RSTRING(val)->len - plen) > i_mm->t->len) {
-	    mm_realloc(i_mm, RSTRING(str)->len + RSTRING(val)->len - plen);
+	plen = regs->end[0] - regs->beg[0];
+	if ((i_mm->t->real + RSTRING_LEN(val) - plen) > i_mm->t->len) {
+	    mm_realloc(i_mm, RSTRING(str)->as.heap.len + RSTRING_LEN(val) - plen);
 	}
-	if (RSTRING(val)->len != plen) {
+	ptr = RSTRING_PTR(str) + start + regs->beg[0];
+	if (RSTRING_LEN(val) != plen) {
 	    if (i_mm->t->flag & MM_FIXED) {
 		rb_raise(rb_eTypeError, "try to change the size of a fixed map");
 	    }
-	    memmove(RSTRING(str)->ptr + start + BEG(0) + RSTRING(val)->len,
-		    RSTRING(str)->ptr + start + BEG(0) + plen,
-		    RSTRING(str)->len - start - BEG(0) - plen);
-	}
-	memcpy(RSTRING(str)->ptr + start + BEG(0),
-	       RSTRING(val)->ptr, RSTRING(val)->len);
-	RSTRING(str)->len += RSTRING(val)->len - plen;
-	i_mm->t->real = RSTRING(str)->len;
-	if (BEG(0) == END(0)) {
-	    offset = start + END(0) + mbclen2(RSTRING(str)->ptr[END(0)], pat);
-	    offset += RSTRING(val)->len - plen;
+	    memmove(ptr + RSTRING_LEN(val),
+		    ptr + plen,
+		    RSTRING_LEN(str) - start - regs->beg[0] - plen);
+	}
+	memcpy(ptr, RSTRING_PTR(val), RSTRING_LEN(val));
+	RSTRING(str)->as.heap.len += RSTRING_LEN(val) - plen;
+	i_mm->t->real = RSTRING(str)->as.heap.len;
+	if (regs->beg[0] == regs->end[0]) {
+	    offset = start + regs->end[0] + mbclen2(RSTRING(str)->as.heap.ptr[regs->end[0]], pat);
+	    offset += RSTRING_LEN(val) - plen;
 	}
 	else {
-	    offset = start + END(0) + RSTRING(val)->len - plen;
+	    offset = start + regs->end[0] + RSTRING_LEN(val) - plen;
 	}
-	if (offset > RSTRING(str)->len) break;
+	if (offset > RSTRING(str)->as.heap.len) break;
 	beg = rb_reg_search(pat, str, offset, 0);
     }
     rb_backref_set(match);
@@ -1247,11 +1313,15 @@
     return obj;
 }
 
+/*
+ * call-seq:
+ *    str.gsub!(pattern, replacement)        => str or nil
+ *    str.gsub!(pattern) {|match| block }    => str or nil
+ *
+ * global substitution
+ */
 static VALUE
-mm_gsub_bang(argc, argv, obj)
-    int argc;
-    VALUE *argv;
-    VALUE obj;
+mm_gsub_bang(int argc, VALUE *argv, VALUE obj)
 {
     VALUE res;
     mm_bang bang_st;
@@ -1273,13 +1343,8 @@
 
 static VALUE mm_index __((int, VALUE *, VALUE));
 
-#if HAVE_RB_DEFINE_ALLOC_FUNC
-
 static void
-mm_subpat_set(obj, re, offset, val)
-    VALUE obj, re;
-    int offset;
-    VALUE val;
+mm_subpat_set(VALUE obj, VALUE re, int offset, VALUE val)
 {
     VALUE str, match;
     int start, end, len;
@@ -1290,7 +1355,7 @@
 	rb_raise(rb_eIndexError, "regexp not matched");
     }
     match = rb_backref_get();
-    if (offset >= RMATCH(match)->regs->num_regs) {
+    if (offset >= RMATCH(match)->rmatch->regs.num_regs) {
 	rb_raise(rb_eIndexError, "index %d out of regexp", offset);
     }
 
@@ -1304,12 +1369,8 @@
     mm_update(i_mm, start, len, val);
 }
 
-#endif
-
 static VALUE
-mm_aset(str, indx, val)
-    VALUE str;
-    VALUE indx, val;
+mm_aset(VALUE str, VALUE indx, VALUE val)
 {
     long idx;
     mm_ipc *i_mm;
@@ -1338,16 +1399,7 @@
 	return val;
 
       case T_REGEXP:
-#if HAVE_RB_DEFINE_ALLOC_FUNC
 	  mm_subpat_set(str, indx, 0, val);
-#else 
-        {
-	    VALUE args[2];
-	    args[0] = indx;
-	    args[1] = val;
-	    mm_sub_bang(2, args, str);
-	}
-#endif
 	return val;
 
       case T_STRING:
@@ -1356,7 +1408,7 @@
 
 	  res = mm_index(1, &indx, str);
 	  if (!NIL_P(res)) {
-	      mm_update(i_mm, NUM2LONG(res), RSTRING(indx)->len, val);
+	      mm_update(i_mm, NUM2LONG(res), RSTRING_LEN(indx), val);
 	  }
 	  return val;
       }
@@ -1375,11 +1427,26 @@
     }
 }
 
+/*
+ * call-seq: []=(args)
+ *
+ * Element assignement - with the following syntax
+ *
+ *   self[nth] = val
+ *
+ * change the <em>nth</em> character with <em>val</em>
+ *
+ *   self[start..last] = val
+ *
+ * change substring from <em>start</em> to <em>last</em> with <em>val</em>
+ *
+ *   self[start, len] = val
+ * 
+ * replace <em>length</em> characters from <em>start</em> with <em>val</em>.
+ * 
+ */
 static VALUE
-mm_aset_m(argc, argv, str)
-    int argc;
-    VALUE *argv;
-    VALUE str;
+mm_aset_m(int argc, VALUE *argv, VALUE str)
 {
     mm_ipc *i_mm;
 
@@ -1387,13 +1454,10 @@
     if (argc == 3) {
 	long beg, len;
 
-#if HAVE_RB_DEFINE_ALLOC_FUNC
 	if (TYPE(argv[0]) == T_REGEXP) {
 	    mm_subpat_set(str, argv[0], NUM2INT(argv[1]), argv[2]);
 	}
-	else
-#endif
-	{
+	else {
 	    beg = NUM2INT(argv[0]);
 	    len = NUM2INT(argv[1]);
 	    mm_update(i_mm, beg, len, argv[2]);
@@ -1406,18 +1470,20 @@
     return mm_aset(str, argv[0], argv[1]);
 }
 
-#if HAVE_RB_STR_INSERT
-
+/*
+ * call-seq: insert(index, str)
+ *
+ * insert <em>str</em> at <em>index</em>
+ */
 static VALUE
-mm_insert(str, idx, str2)
-    VALUE str, idx, str2;
+mm_insert(VALUE str, VALUE idx, VALUE str2)
 {
     mm_ipc *i_mm;
     long pos = NUM2LONG(idx);
 
     GetMmap(str, i_mm, MM_MODIFY);
     if (pos == -1) {
-	pos = RSTRING(str)->len;
+	pos = RSTRING_LEN(str);
     }
     else if (pos < 0) {
 	pos++;
@@ -1426,15 +1492,15 @@
     return str;
 }
 
-#endif
-
-static VALUE mm_aref_m _((int, VALUE *, VALUE));
+static VALUE mm_aref_m(int, VALUE *, VALUE);
 
+/*
+ * call-seq: slice!(str)
+ *
+ * delete the specified portion of the file
+ */
 static VALUE
-mm_slice_bang(argc, argv, str)
-    int argc;
-    VALUE *argv;
-    VALUE str;
+mm_slice_bang(int argc, VALUE *argv, VALUE str)
 {
     VALUE result;
     VALUE buf[3];
@@ -1455,10 +1521,7 @@
 }
 
 static VALUE
-mm_cat(str, ptr, len)
-    VALUE str;
-    const char *ptr;
-    long len;
+mm_cat(VALUE str, const char *ptr, long len)
 {
     mm_ipc *i_mm;
     char *sptr;
@@ -1486,17 +1549,23 @@
 }
 
 static VALUE
-mm_append(str1, str2)
-    VALUE str1, str2;
+mm_append(VALUE str1, VALUE str2)
 {
     str2 = rb_str_to_str(str2);
-    str1 = mm_cat(str1, StringValuePtr(str2), RSTRING(str2)->len);
+    str1 = mm_cat(str1, StringValuePtr(str2), RSTRING_LEN(str2));
     return str1;
 }
 
+/*
+ * Document-method: concat
+ * Document-method: <<
+ *
+ * call-seq: concat(other)
+ *
+ * append the contents of <em>other</em>
+ */
 static VALUE
-mm_concat(str1, str2)
-    VALUE str1, str2;
+mm_concat(VALUE str1, VALUE str2)
 {
     if (FIXNUM_P(str2)) {
 	int i = FIX2INT(str2);
@@ -1509,48 +1578,13 @@
     return str1;
 }
 
-#ifndef HAVE_RB_STR_LSTRIP
-
+/*
+ * call-seq: lstrip!
+ *
+ * removes leading whitespace
+ */
 static VALUE
-mm_strip_bang(str)
-    VALUE str;
-{
-    char *s, *t, *e;
-    mm_ipc *i_mm;
-
-    GetMmap(str, i_mm, MM_MODIFY);
-    mm_lock(i_mm, Qtrue);
-    s = (char *)i_mm->t->addr;
-    e = t = s + i_mm->t->real;
-    while (s < t && ISSPACE(*s)) s++;
-    t--;
-    while (s <= t && ISSPACE(*t)) t--;
-    t++;
-
-    if (i_mm->t->real != (t - s) && (i_mm->t->flag & MM_FIXED)) {
-	mm_unlock(i_mm);
-        rb_raise(rb_eTypeError, "try to change the size of a fixed map");
-    }
-    i_mm->t->real = t-s;
-    if (s > (char *)i_mm->t->addr) { 
-        memmove(i_mm->t->addr, s, i_mm->t->real);
-        ((char *)i_mm->t->addr)[i_mm->t->real] = '\0';
-    }
-    else if (t < e) {
-        ((char *)i_mm->t->addr)[i_mm->t->real] = '\0';
-    }
-    else {
-        str = Qnil;
-    }
-    mm_unlock(i_mm);
-    return str;
-}
-
-#else
-
-static VALUE
-mm_lstrip_bang(str)
-    VALUE str;
+mm_lstrip_bang(VALUE str)
 {
     char *s, *t, *e;
     mm_ipc *i_mm;
@@ -1576,9 +1610,13 @@
     return Qnil;
 }
 
+/*
+ * call-seq: rstrip!
+ *
+ * removes trailing whitespace
+ */
 static VALUE
-mm_rstrip_bang(str)
-    VALUE str;
+mm_rstrip_bang(VALUE str)
 {
     char *s, *t, *e;
     mm_ipc *i_mm;
@@ -1605,8 +1643,7 @@
 }
 
 static VALUE
-mm_strip_bang(str)
-    VALUE str;
+mm_strip_bang(VALUE str)
 {
     VALUE l = mm_lstrip_bang(str);
     VALUE r = mm_rstrip_bang(str);
@@ -1615,8 +1652,6 @@
     return str;
 }
 
-#endif
-
 #define MmapStr(b, recycle)						    \
 do {									    \
     recycle = 0;							    \
@@ -1630,9 +1665,13 @@
 } while (0);
  
  
+/*
+ * call-seq: <=>(other)
+ *
+ * comparison : return -1, 0, 1
+ */
 static VALUE
-mm_cmp(a, b)
-    VALUE a, b;
+mm_cmp(VALUE a, VALUE b)
 {
     int result;
     int recycle = 0;
@@ -1645,11 +1684,13 @@
     return INT2FIX(result);
 }
 
-#if HAVE_RB_STR_CASECMP
-
+/*
+ * call-seq: casecmp(other)
+ *
+ * only with ruby >= 1.7.1
+ */
 static VALUE
-mm_casecmp(a, b)
-    VALUE a, b;
+mm_casecmp(VALUE a, VALUE b)
 {
     VALUE result;
     int recycle = 0;
@@ -1662,11 +1703,16 @@
     return result;
 }
 
-#endif
-
+/*
+ * Document-method: ==
+ * Document-method: ===
+ *
+ * call-seq: ==
+ *
+ * comparison
+ */
 static VALUE
-mm_equal(a, b)
-    VALUE a, b;
+mm_equal(VALUE a, VALUE b)
 {
     VALUE result;
     mm_ipc *i_mm, *u_mm;
@@ -1687,9 +1733,13 @@
     return result;
 }
 
+/*
+ * call-seq: eql?(other)
+ *
+ * Is this eql? to +other+ ?
+ */
 static VALUE
-mm_eql(a, b)
-    VALUE a, b;
+mm_eql(VALUE a, VALUE b)
 {
     VALUE result;
     mm_ipc *i_mm, *u_mm;
@@ -1710,9 +1760,13 @@
     return result;
 }
 
+/*
+ * call-seq: hash
+ *
+ * Get the hash value
+ */
 static VALUE
-mm_hash(a)
-    VALUE a;
+mm_hash(VALUE a)
 {
     VALUE b;
     int res;
@@ -1723,9 +1777,14 @@
     return INT2FIX(res);
 }
 
+/*
+ * Document-method: length
+ * Document-method: size
+ *
+ * return the size of the file
+ */
 static VALUE
-mm_size(a)
-    VALUE a;
+mm_size(VALUE a)
 {
     mm_ipc *i_mm;
 
@@ -1733,9 +1792,13 @@
     return UINT2NUM(i_mm->t->real);
 }
 
+/*
+ * call-seq: empty?
+ *
+ * return <em>true</em> if the file is empty
+ */
 static VALUE
-mm_empty(a)
-    VALUE a;
+mm_empty(VALUE a)
 {
     mm_ipc *i_mm;
 
@@ -1745,23 +1808,20 @@
 }
 
 static VALUE
-mm_protect_bang(t)
-    VALUE *t;
+mm_protect_bang(VALUE *t)
 {
     return rb_funcall2(t[0], (ID)t[1], (int)t[2], (VALUE *)t[3]);
 }
 
 static VALUE
-mm_recycle(str)
-    VALUE str;
+mm_recycle(VALUE str)
 {
     rb_gc_force_recycle(str);
     return str;
 }
 
 static VALUE
-mm_i_bang(bang_st)
-    mm_bang *bang_st;
+mm_i_bang(mm_bang *bang_st)
 {
     VALUE str, res;
     mm_ipc *i_mm;
@@ -1781,16 +1841,14 @@
     }
     if (res != Qnil) {
 	GetMmap(bang_st->obj, i_mm, 0);
-	i_mm->t->real = RSTRING(str)->len;
+	i_mm->t->real = RSTRING(str)->as.heap.len;
     }
     return res;
 }
 
 
 static VALUE
-mm_bang_i(obj, flag, id, argc, argv)
-    VALUE obj, *argv;
-    int flag, id, argc;
+mm_bang_i(VALUE obj, int flag, int id, int argc, VALUE *argv)
 {
     VALUE res;
     mm_ipc *i_mm;
@@ -1817,93 +1875,130 @@
 
 }
 
-#if HAVE_RB_STR_MATCH
-
+/*
+ * call-seq: match(pattern)
+ *
+ * convert <em>pattern</em> to a <em>Regexp</em> and then call
+ * <em>match</em> on <em>self</em>
+ */
 static VALUE
-mm_match_m(a, b)
-    VALUE a, b;
+mm_match_m(VALUE a, VALUE b)
 {
     return mm_bang_i(a, MM_ORIGIN, rb_intern("match"), 1, &b);
 }
 
-#endif
-
+/*
+ * call-seq: upcase!
+ *
+ * replaces all lowercase characters to downcase characters
+ */
 static VALUE
-mm_upcase_bang(a)
-    VALUE a;
+mm_upcase_bang(VALUE a)
 {
     return mm_bang_i(a, MM_MODIFY, rb_intern("upcase!"), 0, 0);
 }
 
+/*
+ * call-seq: downcase!
+ *
+ * change all uppercase character to lowercase character
+ */
 static VALUE
-mm_downcase_bang(a)
-    VALUE a;
+mm_downcase_bang(VALUE a)
 {
     return mm_bang_i(a, MM_MODIFY, rb_intern("downcase!"), 0, 0);
 }
 
+/*
+ * call-seq: capitalize!
+ *
+ * change the first character to uppercase letter
+ */
 static VALUE
-mm_capitalize_bang(a)
-    VALUE a;
+mm_capitalize_bang(VALUE a)
 {
     return mm_bang_i(a, MM_MODIFY, rb_intern("capitalize!"), 0, 0);
 }
 
+/*
+ * call-seq: swapcase!
+ *
+ * replaces all lowercase characters to uppercase characters, and vice-versa
+ */
 static VALUE
-mm_swapcase_bang(a)
-    VALUE a;
+mm_swapcase_bang(VALUE a)
 {
     return mm_bang_i(a, MM_MODIFY, rb_intern("swapcase!"), 0, 0);
 }
  
+/*
+ * call-seq: reverse!
+ *
+ * reverse the content of the file 
+ */
 static VALUE
-mm_reverse_bang(a)
-    VALUE a;
+mm_reverse_bang(VALUE a)
 {
     return mm_bang_i(a, MM_MODIFY, rb_intern("reverse!"), 0, 0);
 }
 
+/*
+ * call-seq: chop!
+ *
+ * chop off the last character
+ */
 static VALUE
-mm_chop_bang(a)
-    VALUE a;
+mm_chop_bang(VALUE a)
 {
     return mm_bang_i(a, MM_CHANGE, rb_intern("chop!"), 0, 0);
 }
 
 static VALUE
-mm_inspect(a)
-    VALUE a;
+mm_inspect(VALUE a)
 {
     return rb_any_to_s(a);
 }
 
+/*
+ * call-seq: chomp!(rs = $/)
+ *
+ * chop off the  line ending character, specified by <em>rs</em>
+ */
 static VALUE
-mm_chomp_bang(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_chomp_bang(int argc, VALUE *argv, VALUE obj)
 {
     return mm_bang_i(obj, MM_CHANGE | MM_PROTECT, rb_intern("chomp!"), argc, argv);
 }
 
+/*
+ * call-seq: delete!(str)
+ *
+ * delete every characters included in <em>str</em>
+ */
 static VALUE
-mm_delete_bang(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_delete_bang(int argc, VALUE *argv, VALUE obj)
 {
     return mm_bang_i(obj, MM_CHANGE | MM_PROTECT, rb_intern("delete!"), argc, argv);
 }
 
+/*
+ * squeeze!(str)
+ *
+ * squeezes sequences of the same characters which is included in <em>str</em>
+ */
 static VALUE
-mm_squeeze_bang(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_squeeze_bang(int argc, VALUE *argv, VALUE obj)
 {
     return mm_bang_i(obj, MM_CHANGE | MM_PROTECT, rb_intern("squeeze!"), argc, argv);
 }
 
+/*
+ * call-seq: tr!(search, replace)
+ *
+ * translate the character from <em>search</em> to <em>replace</em> 
+ */
 static VALUE
-mm_tr_bang(obj, a, b)
-    VALUE obj, a, b;
+mm_tr_bang(VALUE obj, VALUE a, VALUE b)
 {
     VALUE tmp[2];
     tmp[0] = a;
@@ -1911,9 +2006,14 @@
     return mm_bang_i(obj, MM_MODIFY | MM_PROTECT, rb_intern("tr!"), 2, tmp);
 }
 
+/*
+ * call-seq: tr_s!(search, replace)
+ *
+ * translate the character from <em>search</em> to <em>replace</em>, then
+ * squeeze sequence of the same characters 
+ */
 static VALUE
-mm_tr_s_bang(obj, a, b)
-    VALUE obj, a, b;
+mm_tr_s_bang(VALUE obj, VALUE a, VALUE b)
 {
     VALUE tmp[2];
     tmp[0] = a;
@@ -1921,78 +2021,123 @@
     return mm_bang_i(obj, MM_CHANGE | MM_PROTECT, rb_intern("tr_s!"), 2, tmp);
 }
 
+/*
+ * call-seq: crypt
+ *
+ * crypt with <em>salt</em> 
+ */
 static VALUE
-mm_crypt(a, b)
-    VALUE a, b;
+mm_crypt(VALUE a, VALUE b)
 {
     return mm_bang_i(a, MM_ORIGIN, rb_intern("crypt"), 1, &b);
 }
 
+/*
+ * call-seq: include?(other)
+ *
+ * return <em>true</em> if <em>other</em> is found
+ */
 static VALUE
-mm_include(a, b)
-    VALUE a, b;
+mm_include(VALUE a, VALUE b)
 {
     return mm_bang_i(a, MM_ORIGIN, rb_intern("include?"), 1, &b);
 }
 
+/*
+ * call-seq: index
+ *
+ * return the index of <em>substr</em> 
+ */
 static VALUE
-mm_index(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_index(int argc, VALUE *argv, VALUE obj)
 {
     return mm_bang_i(obj, MM_ORIGIN, rb_intern("index"), argc, argv);
 }
 
+/*
+ * call-seq: rindex(sibstr, pos = nil)
+ *
+ * return the index of the last occurrence of <em>substr</em>
+ */
 static VALUE
-mm_rindex(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_rindex(int argc, VALUE *argv, VALUE obj)
 {
     return mm_bang_i(obj, MM_ORIGIN, rb_intern("rindex"), argc, argv);
 }
 
+/*
+ * Document-method: []
+ * Document-method: slice
+ *
+ * call-seq: [](args)
+ *
+ * Element reference - with the following syntax:
+ * 
+ *   self[nth] 
+ * 
+ * retrieve the <em>nth</em> character
+ * 
+ *   self[start..last]
+ * 
+ * return a substring from <em>start</em> to <em>last</em>
+ * 
+ *   self[start, length]
+ * 
+ * return a substring of <em>lenght</em> characters from <em>start</em> 
+ */
 static VALUE
-mm_aref_m(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_aref_m(int argc, VALUE *argv, VALUE obj)
 {
     return mm_bang_i(obj, MM_ORIGIN, rb_intern("[]"), argc, argv);
 }
 
+/*
+ * call-seq: sum(bits = 16)
+ *
+ * return a checksum
+ */
 static VALUE
-mm_sum(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_sum(int argc, VALUE *argv, VALUE obj)
 {
     return mm_bang_i(obj, MM_ORIGIN, rb_intern("sum"), argc, argv);
 }
 
+/*
+ * call-seq: split(sep, limit = 0)
+ *
+ * splits into a list of strings and return this array
+ */
 static VALUE
-mm_split(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_split(int argc, VALUE *argv, VALUE obj)
 {
     return mm_bang_i(obj, MM_ORIGIN, rb_intern("split"), argc, argv);
 }
 
+/*
+ * call-seq: count(o1, *args)
+ *
+ * each parameter defines a set of character to count
+ */
 static VALUE
-mm_count(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_count(int argc, VALUE *argv, VALUE obj)
 {
     return mm_bang_i(obj, MM_ORIGIN, rb_intern("count"), argc, argv);
 }
 
 static VALUE
-mm_internal_each(tmp)
-    VALUE *tmp;
+mm_internal_each(VALUE arg)
 {
+    VALUE *tmp = (VALUE *)arg;
     return rb_funcall2(tmp[0], (ID)tmp[1], (int)tmp[2], (VALUE *)tmp[3]);
 }
 
+/*
+ * call-seq: scan(pattern, &block)
+ *
+ * return an array of all occurence matched by <em>pattern</em> 
+ */
 static VALUE
-mm_scan(obj, a)
-    VALUE obj, a;
+mm_scan(VALUE obj, VALUE a)
 {
     VALUE tmp[4];
 
@@ -2007,10 +2152,17 @@
     return obj;
 }
 
+/*
+ * Document-method: each
+ * Document-method: each_line
+ *
+ * call-seq:
+ *    each(rs = $/, &block)
+ *
+ * iterate on each line
+ */
 static VALUE
-mm_each_line(argc, argv, obj)
-    int argc;
-    VALUE obj, *argv;
+mm_each_line(int argc, VALUE *argv, VALUE obj)
 {
     VALUE tmp[4];
 
@@ -2022,10 +2174,13 @@
     return obj;
 }
 
+/*
+ * call-seq: each_byte(&block)
+ *
+ * iterate on each byte
+ */
 static VALUE
-mm_each_byte(argc, argv, obj)
-    int argc;
-    VALUE obj, *argv;
+mm_each_byte(int argc, VALUE *argv, VALUE obj)
 {
     VALUE tmp[4];
 
@@ -2038,16 +2193,23 @@
 }
 
 static VALUE
-mm_undefined(argc, argv, obj)
-    int argc;
-    VALUE *argv, obj;
+mm_undefined(int argc, VALUE *argv, VALUE obj)
 {
     rb_raise(rb_eNameError, "not yet implemented");
 }
 
+/*
+ * Document-method: lockall
+ * Document-method: mlockall
+ *
+ * call-seq:
+ *  lockall(flag)
+ *
+ * disable paging of all pages mapped. <em>flag</em> can be 
+ * <em>Mmap::MCL_CURRENT</em> or <em>Mmap::MCL_FUTURE</em>
+ */
 static VALUE
-mm_mlockall(obj, flag)
-    VALUE obj, flag;
+mm_mlockall(VALUE obj, VALUE flag)
 {
     if (mlockall(NUM2INT(flag)) == -1) {
 	rb_raise(rb_eArgError, "mlockall(%d)", errno);
@@ -2055,9 +2217,16 @@
     return Qnil;
 }
 
+/*
+ * Document-method: unlockall
+ * Document-method: munlockall
+ *
+ * call-seq: unlockall
+ *
+ * reenable paging
+ */
 static VALUE
-mm_munlockall(obj)
-    VALUE obj;
+mm_munlockall(VALUE obj)
 {
     if (munlockall() == -1) {
 	rb_raise(rb_eArgError, "munlockall(%d)", errno);
@@ -2065,9 +2234,16 @@
     return Qnil;
 }
 
+/*
+ * Document-method: lock
+ * Document-method: mlock
+ *
+ * call-seq: mlock
+ *
+ * disable paging
+ */
 static VALUE
-mm_mlock(obj)
-    VALUE obj;
+mm_mlock(VALUE obj)
 {
     mm_ipc *i_mm;
 
@@ -2085,9 +2261,16 @@
     return obj;
 }
 
+/*
+ * Document-method: munlock
+ * Document-method: unlock
+ *
+ * call-seq: unlock
+ *
+ * reenable paging
+ */
 static VALUE
-mm_munlock(obj)
-    VALUE obj;
+mm_munlock(VALUE obj)
 {
     mm_ipc *i_mm;
 
@@ -2103,7 +2286,7 @@
 }
 
 void
-Init_mmap()
+Init_mmap(void)
 {
     if (rb_const_defined_at(rb_cObject, rb_intern("Mmap"))) {
 	rb_raise(rb_eNameError, "class already defined");
@@ -2156,11 +2339,7 @@
     rb_include_module(mm_cMap, rb_mComparable);
     rb_include_module(mm_cMap, rb_mEnumerable);
 
-#if HAVE_RB_DEFINE_ALLOC_FUNC
     rb_define_alloc_func(mm_cMap, mm_s_alloc);
-#else
-    rb_define_singleton_method(mm_cMap, "allocate", mm_s_alloc, 0);
-#endif
     rb_define_singleton_method(mm_cMap, "new", mm_s_new, -1);
     rb_define_singleton_method(mm_cMap, "mlockall", mm_mlockall, 1);
     rb_define_singleton_method(mm_cMap, "lockall", mm_mlockall, 1);
@@ -2168,6 +2347,7 @@
     rb_define_singleton_method(mm_cMap, "unlockall", mm_munlockall, 0);
 
     rb_define_method(mm_cMap, "initialize", mm_init, -1);
+    rb_define_method(mm_cMap, "initialize_copy", mm_init_copy, 1);
 
     rb_define_method(mm_cMap, "unmap", mm_unmap, 0);
     rb_define_method(mm_cMap, "munmap", mm_unmap, 0);
@@ -2187,33 +2367,24 @@
 
     rb_define_method(mm_cMap, "extend", mm_extend, 1);
     rb_define_method(mm_cMap, "freeze", mm_freeze, 0);
-    rb_define_method(mm_cMap, "clone", mm_undefined, -1);
-    rb_define_method(mm_cMap, "initialize_copy", mm_undefined, -1);
-    rb_define_method(mm_cMap, "dup", mm_undefined, -1);
     rb_define_method(mm_cMap, "<=>", mm_cmp, 1);
     rb_define_method(mm_cMap, "==", mm_equal, 1);
     rb_define_method(mm_cMap, "===", mm_equal, 1);
     rb_define_method(mm_cMap, "eql?", mm_eql, 1);
     rb_define_method(mm_cMap, "hash", mm_hash, 0);
-#if HAVE_RB_STR_CASECMP
     rb_define_method(mm_cMap, "casecmp", mm_casecmp, 1);
-#endif
     rb_define_method(mm_cMap, "+", mm_undefined, -1);
     rb_define_method(mm_cMap, "*", mm_undefined, -1);
     rb_define_method(mm_cMap, "%", mm_undefined, -1);
     rb_define_method(mm_cMap, "[]", mm_aref_m, -1);
     rb_define_method(mm_cMap, "[]=", mm_aset_m, -1);
-#if HAVE_RB_STR_INSERT
     rb_define_method(mm_cMap, "insert", mm_insert, 2);
-#endif
     rb_define_method(mm_cMap, "length", mm_size, 0);
     rb_define_method(mm_cMap, "size", mm_size, 0);
     rb_define_method(mm_cMap, "empty?", mm_empty, 0);
     rb_define_method(mm_cMap, "=~", mm_match, 1);
     rb_define_method(mm_cMap, "~", mm_undefined, -1);
-#if HAVE_RB_STR_MATCH
     rb_define_method(mm_cMap, "match", mm_match_m, 1);
-#endif
     rb_define_method(mm_cMap, "succ", mm_undefined, -1);
     rb_define_method(mm_cMap, "succ!", mm_undefined, -1);
     rb_define_method(mm_cMap, "next", mm_undefined, -1);
@@ -2264,18 +2435,14 @@
     rb_define_method(mm_cMap, "chop", mm_undefined, -1);
     rb_define_method(mm_cMap, "chomp", mm_undefined, -1);
     rb_define_method(mm_cMap, "strip", mm_undefined, -1);
-#if HAVE_RB_STR_LSTRIP
     rb_define_method(mm_cMap, "lstrip", mm_undefined, -1);
     rb_define_method(mm_cMap, "rstrip", mm_undefined, -1);
-#endif
 
     rb_define_method(mm_cMap, "sub!", mm_sub_bang, -1);
     rb_define_method(mm_cMap, "gsub!", mm_gsub_bang, -1);
     rb_define_method(mm_cMap, "strip!", mm_strip_bang, 0);
-#if HAVE_RB_STR_LSTRIP
     rb_define_method(mm_cMap, "lstrip!", mm_lstrip_bang, 0);
     rb_define_method(mm_cMap, "rstrip!", mm_rstrip_bang, 0);
-#endif
     rb_define_method(mm_cMap, "chop!", mm_chop_bang, 0);
     rb_define_method(mm_cMap, "chomp!", mm_chomp_bang, -1);
 
@@ -2286,7 +2453,7 @@
     rb_define_method(mm_cMap, "count", mm_count, -1);
 
     rb_define_method(mm_cMap, "tr!", mm_tr_bang, 2);
-    rb_define_method(mm_cMap, "tr_s!", mm_tr_s_bang, 2);
+    rb_define_method(mm_cMap, "tr_s!", mm_undefined, -1);
     rb_define_method(mm_cMap, "delete!", mm_delete_bang, -1);
     rb_define_method(mm_cMap, "squeeze!", mm_squeeze_bang, -1);
 
