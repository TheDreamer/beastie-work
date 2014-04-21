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

editors/lazarus

	This build dependency got updated, as did the port that depended on it.  But, it wouldn't build.  When it was
	doing eidtors/lazarus-lcl-units, steps were in need of lcl to have been built...but it wasn't going to build these
	until last.

	So, I patched it to make it build sooner.

	PR: ports/187961

	Turns out the problem was stray files left behind by the old version causing problems.  The maintainer says he
	fixed the deinstall in this version, and a later rebuild/reinstall and then delete of this build depend worked
	without my hack.
	
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

mail/evolution

	Got updated to do staging, but now its missing files (shared libraries).  pkg-plist didn't include them.  So,
	updated pkg-plist.

	PR: ports/188525

net/boinc-client

	When I first started running boinc on FreeBSD, I discovered to my delight that VirtualBox was detected and usable
	for Test4Theory.  But, somewhere along the way, whenever I checked on things, it would report that it didn't detect
	VirtualBox.  This was usually resolved by restarting it, as this was usually contention between the respective rc
	scripts.

	Except that I was seeing it more and more lately, but having solved the rcorder problem I was having where
	/etc/rc.d/tmp and /etc/rc.d/cleartmp were being called after /usr/local/etc/rc.d/vboxheadless (partly my fault,
	because I run an NFS server in the headless VM which my host mounts.  Its how I get dropbox into my FreeBSD
	system.

	But, the problem continued to happen with boinc-client.  So, I decided to take a closer look.

	I found that hostinfo_unix.cpp thinks its a linux like system, it looks runs "/usr/lib/virtualbox/VBoxManage",
	which is located at "/usr/local/lib/virtualbox/VBoxManage" (of course.).  Else if its __APPLE__, it looks for
	the app, etc.

	So, I added a patch to handle __FreeBSD__.

	Not sure when or how this got broke.  At first I thought the patch had disappeared, but didn't find anything
	resembling my patch (though I only went back a couple of years...) or if the support disappeared in the 7.x
	version of the code.

	Strange how I had failed to notice for so long....

	PR: ports/188710

net/freeradius2

	Updating to 2.2.4 was failing for me, turns out configure has an auto-activation on execinfo.h, except it wasn't
	using it correctly for FreeBSD.  So, I made it an explicit library depend and made it link to the library.

	PR: ports/188089

net/nxserver

	PORTREVISION got bumped to 9, due to newer libaudiofile.  But, it wouldn't build.  audiofile-config has been
	removed, for pkgconf but nxserver (specifically nxesd's) configure doesn't know how to deal with this.  Started
	trying to patch configure, when I went with setting AUDIOFILE_CONFIG="/usr/local/bin/pkgconf audiofile" and
	passing '--disable-audiofiletest' to configure.  It can still get the necessary lib flags and cflags using the first
	part, but the process of checking that 0.3.6 is greater than minimum version of 0.1.5 was easier solved by disabling
	the test.

	Could have port system check version, but can't do that on a LIB_DEPENDS and went for simple.

	PR: ports/183825

security/gnutls3

	Recently this port updated, and it wants security/openssl to be installed as well.  But, I don't want that.
	So, I tracked down the port that had installed this port and removed it, and removed this and other leafs.

	But, word is that this port might be replacing gnutls, which is a much bigger mess to purge.

	So, I looked at why gnutls3 wanted openssl-1.x.  And, made it an option, preferring to build without it.

	Its being pulled in by dns/unbound, which its default options cause it to require openssl from ports on <10.
	And, this is to create libgnutls-dane, which adds DNSSEC verification support to DANE.  And, is needed for
	the '--check' option with danetool3.

	PR: ports/188184

sysutils/cfengine35

	Recently in /usr/ports/UPDATING:

	The library version for textproc/libxml2 was brought in line with what the upstream expects.  Causing a problem for
	cfengine.

	Using "portmaster -r libxml2" would've missed updating this port as it doesn't list it as a depend, but uses it.
	Though I updated it, because I ran into the args too long problem...so I used pkg_libchk method.

	This however broke all my cfengine installs, as the copy in /var/cfengine/bin is updated by cfengine.  cf-execd
	is running unaffected by this, but it is unable to call cf-agent as its missing libxml2.so.5 (as it has become
	libxml2.so.2.)

	This made me recall the contortions a co-worker seemed to go through in making static executables for cfengine2.
	Fortunately, we didn't run into any problems between the different Sun architectures like I had encountered once
	in a previous life.

	After much effort, I came up with a couple of solutions.  The full static or partly static cf-agent.  Where the
	partly static cf-agent has just libxml2 and libpromises linked in statically.  I wasn't able to get tokyocabinet
	to link into partly static, since it doesn't have a libtool file.

	While I was working on this, I figured out the FreeBSD issue with libxml2.  Its an auto-activiation in the
	upstream configure script, while the proper way is to have knob.  So, I added one.

	PR: ports/180896

sysutils/cfengine35/old

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

sysutils/memtest86+

	Doesn't build if ISO option is selected, the distribution Makefile is
	calling make but the port is using gmake.

	Patch the Makefile to use gmake.

	PR: ports/188149

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

        2014-04-13: After updating to 9.2-RELEASE-p4, which has a kernel.symbols file.  Wonder why -p3 didn't.  And, not
	rebuilding a GENERIC debug kernel, decided I needed to fix the logic for using kernel.symbols.

	Decided to do the nasty, and patch /usr/sbin/crashinfo to use kernel.symbols with kgdb (if available, or
	specified.)

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

	But, my terminal window is 80x24, and changing the size doesn't make any difference.  Eventually, I looked deeper
	into the problem, and its having trouble with package comments that contain quotation marks.

	So wiped up an ugly kluge to deal with them, and now its working... ;)

	PR: ports/186904

ports-mgmt/portrac

	For the last few days its been saying that there are no ports needing update.  Seems odd that nothing has been
	changing in for this long, but there were lots of ports I know I hadn't updated at home that I had to deal with
	on work computer.

	Finally, took a peek inside and its not PKGNG aware :(

	Patched it in a similar manner to how ports-mgmt/pkg_cutleaves handles
	PKGNG.

	PR: ports/186963

pr178818

	docs/178818 
		gmirror(8) says to use rc.eary which is no longer available

	proposed fix is to automate this in the dumpon script, which had one small flaw, which I corrected.  And, then tried
	to recall how to include a patch that won't get munged by mail client....

print/hplip

	LIB_DEPENDS is testing for a library that is no longer part of print/cups-base to determine if is installed.
	So, updated to look for a different library.  Update to 3.14.4 while I'm here.  And, patch the deprecation warning
	emitted by foomatic-rip-hplip.

	However, this leaves one remaining problem. All the PPD files appear to contain:

		*cupsFilter: "application/vnd.cups-postscript 100 foomatic-rip-hplip"
		*cupsFilter: "application/vnd.cups-pdf 0 foomatic-rip-hplip"

	But, unlike foomatic-rip (apparently), foomatic-rip-hplip only handles text or postscript.
	Removing the "application/vnd.cups-pdf" line, makes it so I can print again.

		*cupsFilter: "application/vnd.cups-postscript 100 foomatic-rip-hplip"

x11-server/xorg-nestserver

	Options wouldn't stick, causing portmaster to ask for them over and over again.  A quick patch to fix.

	PR: ports/188848

