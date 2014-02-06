beastie-work
============
Somewhere to save the FreeBSD ports that I'm fiddling with...

	2: Well, you've been fiddling with it, haven't you?
	3: It was perfectly all right until you touched it.
	   Now if only you'd leave things to me.
	2: If we were to leave things with you, my dear fellow,
	   we'd be in a fine pickle wouldn't we?

ports
=====
devel/json-c

	This was to form the patch for a PR to get json-c-0.9 updated to
	json-c-0.10, though the latest is json-c-0.11.

devel/ruby-mmap

	Been trying to figure out how to generate the .fw files for
	sysutils/devcpu-data from the latest amd-ucode files.  Found a ruby
	program that looked promising, but getting it to run was a challenge.
	Especially since it wanted 'mmap'.

	I got mmap to build after going through the source and figuring out
	what was needed to get it to work with 1.9.  I was going to fork
	ruby-mmap on github to record my work, when I found that numerous
	other forks exist with similiar/better changes to mmap.c.  I took
	what looked appeared to be the most recent and it showed that it to
	be 0.2.7.

	So I tweaked the Makefile to create PORTVERSION 0.2.7 from DISTVERSION
	0.2.6 :)

	Alas, it didn't extract anything.

print/cups-cloud-print

	While researching options for replacing my old inkjet printer, and
	the apparently obsession of vendors to do GDI and Windows only...
	though some support Mac OS X and some might add some Linx support.

	Since I already ran into this mistake with my laser printer, which
	I hardly use now.  Though I run a small ubuntu VM on one of my
	FreeBSD systems to make use of it.  It appears unlikely that I'll
	have Windows again, and no idea when a Mac will become a reality.

	One of the draws to the idea of working for a University was to
	get discounted Apple hardware, but I've yet to get anything this way
	in over 7 years.

	I had seen printers with direct Google Cloud printing support, but
	the capability disappeared from Chromium on FreeBSD some time ago.
	I do have the capability to print to my printers at home, since I
	own a ChromeBook.  Again it involves running chrome in that small
	ubuntu VM.

	So, I had gotten a version of this to build and run.  And, I
	successfully printed a web page from my browser on FreeBSD, out to
	Google, back to chrome running in VM on same computer, to printer
	sitting next to said computer.  Though probably wouldn't be any
	less weird if I did get a printer with direct cloud printing support.

	The port probably needs a lot more work to get it to FreeBSD standards,
	but this work staled on trying to figure out how MASTER_SITES=GH works.

	Not sure I remember all what I did anymore, so thought I should
	save my work somewhere before it got lost.
	
	http://www.niftiestsoftware.com/cups-cloud-print/

sysutils/grive

	I got grive-0.2.0 to build on FreeBSD, and thought about turning
	it into a port.  But, I think I'm out of my depth on whether this
	is even right.  Plus I don't think I want to be stuck maintain this.

sysutils/grive-devel

	I took a look at the current work on grive-0.3.0 on github, but it
	needs at least 0.10 of devel/json-c and ports stops at 0.9.  I had
	started work on making a patch to update the port, but sounds like
	development is heading towards needing 0.11 now.

	I then ran into it needing a new version of devel/boost-libs than
	currently in ports.  Given the difficulty I've had in buidling as
	a dependency of other ports, I don't feel like tackling this update.

	Plus I still haven't figured out how to do distfiles from GitHub,
	MASTER_SITES=GH

	Otherwise, I might reveal my stab at print/cups-cloud-print.

sysutils/panicmail

	Recently, I had yet another panic.  Couldn't remember how I had
	examined the previous vmcore, while searching online on how again
	I came across this port.

	Almost immediately, I made some tweaks to it.  Like change the
	From: address for autosubmit to use a proper email address rather
	than my internal root account.

	I forget if I ever got around to creating a genericstable config
	for my FreeBSD sendmail servers, which have replaced my Ubuntu
	postfix servers.  Though if I had, the address I would use in
	place of root on my primary desktop would differ from the email
	address I use for FreeBSD related correspondence.
	
	Initially, I made the tweaks by hand and then coded up a CFE3
	promise to recreate it on my other FreeBSD servers.

	Then, I started to set things up for possible PR submission to
	send up my 'enhancements'.  But, as I was doing that, I thought
	up other enhancements and it got a little out of hand.  Now,
	I'm not sure I want to send it up....

	new rc.conf(.local) entries (with defaults):

	panicmail_autonotify="YES"
	panicmail_sendfrom="root"
	panicmail_usecrashinfo="NO"
	panicmail_usekernel=""

www/p5-WWW-Shorten-Yourls

	www/yourls exists in ports, but not this Perl module?

patches
=======
databases/pecl-mysqlnd_qc

	Keep seeing the advise to use memcache instead of query cache in
	mysql, but it stumped me on how would I do this without requiring that
	I change the applications (the big one being cacti.)

	When I finally came across this extension, which sounded like it would
	fit.  Except it didn't, because I use spine. :(

	Anyways, while trying it out, I couldn't figure out how to get it to
	use memcached.  Well, these are enabled at compile time, and the port
	didn't ask if I wanted to enable MEMCACHE or sqlite storage handlers.
	So fiddled with the Makefile to have it ask.

	Wonder if I'll submit it?

	Decided to first remove reference to APC.

databases/pecl-mysqlnd_qc/patch-php_mysqlnd_qc.c

	Going with mysqlnd_qc.cache_by_default, there needed to be away (to me)
	to have it use MEMCACHE by default.  I started trying to add a new INI
	option to do this, but doing development on a 'production' server is
	bad, so I went with the qad way....

mail/dovecot2

	ports/175813: [patch] mail/dovecot2 doesn't detect libstemmer or
		exttextcat

	Patches to get dovecot2 with fts_lucene to work, because
	textproc/libexttextcat has incremented its library name so configure,
	has trouble finding it.

	libstemmer is incorporated into lib-clucene-contribs-lib.so in
	textproc/clucene

	libextextcat has gone from:
	    libexttextcat.so(.*) ->
		libexttextcat-1.0.so(.*)->
			libexttextcat-2.0.so(.*)

mail/davmail

	Update to 4.4.0

net/nxserver

	PORTREVISION got bumped to 9, due to newer libaudiofile.  But, it
	wouldn't build.  audiofile-config has been removed, for pkgconf but
	nxserver (specifically nxesd's) configure doesn't know how to deal
	with this.  Started trying to patch configure, when I went with
	setting AUDIOFILE_CONFIG="/usr/local/bin/pkgconf audiofile" and
	passing --disable-audiofiletest to configure.  It can still get
	the necessary lib flags and cflags using the first part, but the
	process of checking that 0.3.6 is greater than minimum version of
	0.1.5 was easier solved by disabling the test.

	Could have port system check version, but can't do that on a 
	LIB_DEPENDS and went for simple.

sysutils/cfengine35

	Kind of odd making a patch for FreeBSD when the problem is upstream.
	
	So, opted to report it upstream, but in the meantime I rebuild cfengine
	using this patch.

	-------------------------------------------

	Issue #3207 - configure fails to find libxml2

	configure doesn't find -lxml2, because it doesn't call
		"xml2-config --libs" to get the LDFLAGS needed to find it.

	-------------------------------------------

	But, then also reported for FreeBSD.

	ports/180896

	sysutils/cfengine35: can't use edit_xml because libxml2 not found

	========================================================================

	patch-3.5.1-3

	Noticed that the new CFEngine Standard Library for 3.5.1 is missing,
	so that's what 3.5.1-3 is.

	ports/181509

pr178818

	docs/178818 
		gmirror(8) says to use rc.eary which is no longer available

	proposed fix is to automate this in the dumpon script, which had
	one small flaw, which I corrected.  And, then tried to recall how to
	include a patch that won't get munged by mail client....

Other
=====
xpi/accountcolors

	I have a lot of accounts in Thunderbird, and have been looking for
	extensions to find certain accounts easier.  This one seemed helpful,
	except that it wouldn't show all my accounts.

	Probably is hard coded to some maximum number of accounts...

	So, I decided to hack the extension to increase it.  After counting
	the number of accounts visible, I deduced the limit to be 60.  I then
	counted the rest of my accounts and rounded up to 128. ;)

	Replacing the right occurrences of 60 with 128 was easy.  The hard
	part was the options dialog.  Which was ugly, which I added to more.
	You would think there's a cleaner way to generate the rows, either
	staticly or dynamicly.  Oh well, it does what I want it to now :)

	****

	Later I found an extension, called QuickFolders, which was more of
	what I was really looking for.  I did try to match the colors I set
	with accountcolors with the colors I set for the folder bookmarks.

	Most are just the inbox of an account, but its about jumping to where
	I want to be in my folderpane.
