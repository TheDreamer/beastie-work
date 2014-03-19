PATCHES
=======
archivers/quazip

	This port got updated, but it wouldn't build.  After some investigation, it was obviously a problem of conflict
	with installed port.  Specifically for libquazip.so. Instead of resorting to "make clean deinstall reinstall", I
	made it hack the Makefile so to force it use the built one instead of the installed one.
	
	PR: ports/187735

databases/memcached

	It is becoming a peeve of ports that work fine, but PORTREVISION is bumped so that it will disrupted "portmaster -a"
	or disrupt my system.  Especially when it causes portmaster to remove the working build and fail during the install
	of the bumped port.  Especially, when the the bump is of questionable importance, though it might only be
	'especially' since it broke something that wasn't really broken.  Had it not broken things but was an
	enhancement....well, hard to say in this case.

	Here's the patch I make the new port option work.  Both for my 'servers' where I have "OPTIONS_UNSET = DOCS" set
	in /etc/make.conf, and on my desktop where I left it with its defaults.

	PR: ports/187337

	Discovered that I couldn't get into my email when checking to see what number had been assigned.  Roundcube logs
	have "DB Error: Failed to connect to memcached."  Also noticed that memcached is consuming 100% of CPU.

	I backed out parts of the "Enforce libevent2" patch, to get things working again.

databases/pecl-mysqlnd_qc

	Keep seeing the advise to use memcache instead of query cache in mysql, but it stumped me on how would I do this
	without requiring that I change the applications (the big one being cacti.)

	When I finally came across this extension, which sounded like it would fit.
	Except it didn't, because I use spine. :(

	Anyways, while trying it out, I couldn't figure out how to get it to use memcached.  Well, these are enabled at
	compile time, and the port didn't ask if I wanted to enable MEMCACHE or sqlite storage handlers.  So fiddled with
	the Makefile to have it ask.

	Wonder if I'll submit it?

	Decided to first remove reference to APC.

databases/pecl-mysqlnd_qc/patch-php_mysqlnd_qc.c

	Going with mysqlnd_qc.cache_by_default, there needed to be a way (to me) to have it use MEMCACHE by default.
	I started trying to add a new INI option to do this, but doing development on a 'production' server is bad,
	so I went with the QAD way....

deskutils/gcalcli

	Been trying to make use of the more advanced options of this, but haven't had much luck.  Went to see if there
	was a newer version, and found that the project had moved to github and the latest tag is 2.4.2.  Seems ports
	have of way of not getting updated if the site moves, and new releases aren't posted to the old site, etc.

	I started with a quick update the port so that I can have the latest, which wasn't too hard since its a NO_BUILD
	port.  Then I thought about sending it up, which resulted in more changes to things.  Like option knobs for the
	3 optional dependencies.  The default matching the one that had been listed as required dependency for previous
	package.

	And, then added copying/installing of docs, which led to staging.

	Haven't tried the options yet, but it did make one calendar that I could see in the previous version visible.
	Under other calendars on Google, I have various shared public calendars and one privately shared (exchange) calendar.
	Which was the main reason I was fiddling with gcalcli today.

	PR: ports/187619

mail/dovecot2

	ports/175813: [patch] mail/dovecot2 doesn't detect libstemmer or exttextcat

	Patches to get dovecot2 with fts_lucene to work, because textproc/libexttextcat has incremented its library name so
	configure, has trouble finding it.

	libstemmer is incorporated into lib-clucene-contribs-lib.so in textproc/clucene

	libextextcat has gone from:
	    libexttextcat.so(.*) ->
		libexttextcat-1.0.so(.*)->
			libexttextcat-2.0.so(.*)

mail/davmail

	Update to 4.4.0

net/nxserver

	PORTREVISION got bumped to 9, due to newer libaudiofile.  But, it wouldn't build.  audiofile-config has been
	removed, for pkgconf but nxserver (specifically nxesd's) configure doesn't know how to deal with this.  Started
	trying to patch configure, when I went with setting AUDIOFILE_CONFIG="/usr/local/bin/pkgconf audiofile" and
	passing '--disable-audiofiletest' to configure.  It can still get the necessary lib flags and cflags using the first
	part, but the process of checking that 0.3.6 is greater than minimum version of 0.1.5 was easier solved by disabling
	the test.

	Could have port system check version, but can't do that on a LIB_DEPENDS and went for simple.

	PR: ports/183825

sysutils/cfengine35

	Kind of odd making a patch for FreeBSD when the problem is upstream.
	
	So, opted to report it upstream, but in the meantime I rebuild cfengine using this patch.

	-------------------------------------------

	Issue #3207 - configure fails to find libxml2

	configure doesn't find -lxml2, because it doesn't call
		"xml2-config --libs" to get the LDFLAGS needed to find it.

	-------------------------------------------

	But, then also reported for FreeBSD.

	PR: ports/180896

	sysutils/cfengine35: can't use edit_xml because libxml2 not found

	========================================================================

	patch-3.5.1-3

	Noticed that the new CFEngine Standard Library for 3.5.1 is missing, so that's what 3.5.1-3 is.

	PR: ports/181509

sysutils/panicmail

	Recently, I had yet another panic.  Couldn't remember how I had examined the previous vmcore, while searching online
	on how again I came across this port.

	Almost immediately, I made some tweaks to it.  Like change the From: address for autosubmit to use a proper email
	address rather than my internal root account.

	I forget if I ever got around to creating a genericstable config for my FreeBSD sendmail servers, which have
	replaced my Ubuntu postfix servers.  Though if I had, the address I would use in place of root on my primary desktop
	would differ from the email address I use for FreeBSD related correspondence.
	
	Initially, I made the tweaks by hand and then coded up a CFE3 promise to recreate it on my other FreeBSD servers.

	Then, I started to set things up for possible PR submission to send up my 'enhancements'.  But, as I was doing that,
	I thought up other enhancements and it got a little out of hand.  Now, I'm not sure I want to send it up....

	new rc.conf(.local) entries (with defaults):

		panicmail_autonotify="YES"
		panicmail_sendfrom="root"
		panicmail_usecrashinfo="NO"
		panicmail_usekernel=""

sysutils/pefs-kmod

	Decided to give pefs a try.

	After loading the module, I noticed in dmesg:

		cryptosoft0: <software crypto> on motherboard

	Later I wondered if I work machine had an i7 with AESNI. So, I tried loading that module.

	This message appeared in dmesg:

		aesni0: <AES-CBC,AES-XTS> on motherboard

	But, then I noticed that AESNI support is disabled in the build....its turned on by a DEFINE.  Probably to have it
	build on a wider range of FreeBSD versions it was disabled.  Though would be nice if it could be turned on for
	people that want it and know they can, better if it could also tell if you can or can't have it.

	So, I quickly added a knob to turn it on.

	Now when I load pefs on my work machine, I get:

		pefs: AESNI hardware acceleration enabled

	in dmesg.

	Now trying this at home (where I had only built it, but never got around to trying it), I first try loading aesni.
	These two lines appeared in dmsesg:

		cryptosoft0: <software crypto> on motherboard
		aesni0: No AESNI support.

	I then unloaded aesni, resulting in this line in dmesg:

		cryptosoft0: detached

	Rebuilt the port with AESNI enabled, and loaded int resulted in the same two messages with loading aesni.  Guess
	that is expected, but wasn't known because I did things out of order at work.

	No message from pefs.  What I ideally want is something that'll work for between home and work, and multiple
	operating systems.  So, not sure this fulfills that need, though there is a need for it on work computer.

	PR: ports/187620

ports-mgmt/pkg_rmleaves

	Everytime that I've tried this program, it always fails with:

	Dialog Error, try to resize your terminal to at least 80x24.

	But, my terminal window is 80x24, and changing the size doesn't make
	any difference.  Eventually, I looked deeper into the problem, and
	its having trouble with package comments that contain quotation marks.

	So wiped up an ugly kluge to deal with them, and now its working... ;)

	PR: ports/186904

ports-mgmt/portrac

	For the last few days its been saying that there are no ports needing
	update.  Seems odd that nothing has been changing in for this long,
	but there were lots of ports I know I hadn't updated at home that I
	had to deal with on work computer.

	Finally, took a peek inside and its not PKGNG aware :(

	Patched it in a similar manner to how ports-mgmt/pkg_cutleaves handles
	PKGNG.

	PR: ports/186963

pr178818

	docs/178818 
		gmirror(8) says to use rc.eary which is no longer available

	proposed fix is to automate this in the dumpon script, which had one small flaw, which I corrected.  And, then tried
	to recall how to include a patch that won't get munged by mail client....

